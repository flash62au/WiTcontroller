/**
 * This app turns the ESP32 into a Bluetooth LE keyboard that is intended to act as a dedicated
 * gamepad for the JMRI or any wiThrottle server.

  Instructions:
  - Update WiFi SSIDs and passwords as necessary in config_network.h.
  - Flash the sketch 
 */

#include <WiFi.h>                 // https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi
#include <ESPmDNS.h>              // https://github.com/espressif/arduino-esp32/blob/master/libraries/ESPmDNS (You should be able to download it from here https://github.com/espressif/arduino-esp32 Then unzip it and copy 'just' the ESPmDNS folder to your own libraries folder )
#include <WiThrottleProtocol.h>   // https://github.com/flash62au/WiThrottleProtocol
#include <AiEsp32RotaryEncoder.h> // https://github.com/igorantolic/ai-esp32-rotary-encoder
#include <Keypad.h>               // https://www.arduinolibraries.info/libraries/keypad
#include <U8g2lib.h>              // https://github.com/olikraus/u8g2  (Just get "U8g2" via the Arduino IDE Library Manager)
#include <string>

#include "config_network.h"      // LAN networks (SSIDs and passwords)
#include "config_keypad_etc.h"   // hardware config - GPIOs - keypad, encoder; oled display type
#include "config_buttons.h"      // keypad buttons assignments

#include "static.h"              // change for non-english languages
#include "actions.h"

#include "WiTcontroller.h"

// comment out the following line to stop all console messages
#define DEBUG 1

#ifdef DEBUG
 #define debug_print(...) Serial.print(__VA_ARGS__)
 #define debug_println(...) Serial.print(__VA_ARGS__); Serial.print(" ("); Serial.print(millis()); Serial.println(")")
 #define debug_printf(...) Serial.printf(__VA_ARGS__)
#else
 #define debug_print(...)
 #define debug_println(...)
 #define debug_printf(...)
#endif

// *********************************************************************************

int keypadUseType = KEYPAD_USE_OPERATION;
int encoderUseType = ENCODER_USE_OPERATION;

boolean menuCommandStarted = false;
String menuCommand = "";
boolean menuIsShowing = false;

String oledText[18] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};

int currentSpeed = 0;
Direction currentDirection = Forward;
int speedStepCurrentMultiplier = 1;

TrackPower trackPower = PowerUnknown;
String turnoutPrefix = "";
String routePrefix = "";

// encoder variables
bool circleValues = true;
int encoderValue = 0;
int lastEncoderValue = 0;

// server variables
// boolean ssidConnected = false;
String selectedSsid = "";
String selectedSsidPassword = "";
int ssidConnectionState = CONNECTION_STATE_DISCONNECTED;

// ssid password entry
String ssidPasswordEntered = "";
boolean ssidPasswordChanged = true;
char ssidPasswordCurrentChar = ssidPasswordBlankChar; 

IPAddress selectedWitServerIP;
int selectedWitServerPort = 0;
String selectedWitServerName ="";
int noOfWitServices = 0;
int witConnectionState = CONNECTION_STATE_DISCONNECTED;

//found wiThrottle servers
IPAddress foundWitServersIPs[maxFoundWitServers];
int foundWitServersPorts[maxFoundWitServers];
String foundWitServersNames[maxFoundWitServers];
int foundWitServersCount = 0;

//found ssids
String foundSsids[maxFoundSsids];
boolean foundSsidsOpen[maxFoundSsids];
int foundSsidsCount = 0;
int ssidSelectionSource;

// wit Server ip entry
String witServerIpAndPortConstructed = "###.###.###.###:#####";
String witServerIpAndPortEntered = "";
boolean witServerIpAndPortChanged = true;

// roster variables
int rosterSize = 0;
int rosterIndex[maxRoster]; 
String rosterName[maxRoster]; 
int rosterAddress[maxRoster];
char rosterLength[maxRoster];

int page = 0;

// turnout variables
int turnoutListSize = 0;
int turnoutListIndex[maxTurnoutList]; 
String turnoutListSysName[maxTurnoutList]; 
String turnoutListUserName[maxTurnoutList];
int turnoutListState[maxTurnoutList];

// route variables
int routeListSize = 0;
int routeListIndex[maxRouteList]; 
String routeListSysName[maxRouteList]; 
String routeListUserName[maxRouteList];
int routeListState[maxRouteList];

// function states
boolean functionStates[28];

// speedstep
int currentSpeedStep = speedStep;

int heartBeatPeriod = 10; // default to 10 seconds
long lastServerResponseTime;  // seconds since start of Arduino
boolean heartbeatCheckEnabled = true;

// don't alter the assignments here
// alter them in config_buttons.h

const char* deviceName = DEVICE_NAME;

const boolean encoderRotationClockwiseIsIncreaseSpeed = ENCODER_ROTATION_CLOCKWISE_IS_INCREASE_SPEED;
// false = Counterclockwise  true = clockwise

const boolean toggleDirectionOnEncoderButtonPressWhenStationary = TOGGLE_DIRECTION_ON_ENCODER_BUTTON_PRESSED_WHEN_STATIONAY;
// true = if the locos(s) are stationary, clicking the encoder button will toggle the direction

//4x3 keypad
int buttonActions[10] = { CHOSEN_KEYPAD_0_FUNCTION,
                          CHOSEN_KEYPAD_1_FUNCTION,
                          CHOSEN_KEYPAD_2_FUNCTION,
                          CHOSEN_KEYPAD_3_FUNCTION,
                          CHOSEN_KEYPAD_4_FUNCTION,
                          CHOSEN_KEYPAD_5_FUNCTION,
                          CHOSEN_KEYPAD_6_FUNCTION,
                          CHOSEN_KEYPAD_7_FUNCTION,
                          CHOSEN_KEYPAD_8_FUNCTION,
                          CHOSEN_KEYPAD_9_FUNCTION
};

// text that will appear when you press #
const String directCommandText[4][3] = {
    {CHOSEN_KEYPAD_1_DISPLAY_NAME, CHOSEN_KEYPAD_2_DISPLAY_NAME, CHOSEN_KEYPAD_3_DISPLAY_NAME},
    {CHOSEN_KEYPAD_4_DISPLAY_NAME, CHOSEN_KEYPAD_5_DISPLAY_NAME, CHOSEN_KEYPAD_6_DISPLAY_NAME},
    {CHOSEN_KEYPAD_7_DISPLAY_NAME, CHOSEN_KEYPAD_8_DISPLAY_NAME, CHOSEN_KEYPAD_9_DISPLAY_NAME},
    {"* Menu", CHOSEN_KEYPAD_0_DISPLAY_NAME, "# This"}
};

// in case the values are not defined in config_buttons.h
// DO NOT alter the values here 
#ifndef CHOSEN_ADDITIONAL_BUTTON_0_FUNCTION
  #define CHOSEN_ADDITIONAL_BUTTON_0_FUNCTION FUNCTION_NULL
  #define CHOSEN_ADDITIONAL_BUTTON_1_FUNCTION FUNCTION_NULL
  #define CHOSEN_ADDITIONAL_BUTTON_2_FUNCTION FUNCTION_NULL
  #define CHOSEN_ADDITIONAL_BUTTON_3_FUNCTION FUNCTION_NULL
  #define CHOSEN_ADDITIONAL_BUTTON_4_FUNCTION FUNCTION_NULL
  #define CHOSEN_ADDITIONAL_BUTTON_5_FUNCTION FUNCTION_NULL
  #define CHOSEN_ADDITIONAL_BUTTON_6_FUNCTION FUNCTION_NULL
#endif

//optional additional buttons
int additionalButtonActions[MAX_ADDITIONAL_BUTTONS] = { 
                          CHOSEN_ADDITIONAL_BUTTON_0_FUNCTION,
                          CHOSEN_ADDITIONAL_BUTTON_1_FUNCTION,
                          CHOSEN_ADDITIONAL_BUTTON_2_FUNCTION,
                          CHOSEN_ADDITIONAL_BUTTON_3_FUNCTION,
                          CHOSEN_ADDITIONAL_BUTTON_4_FUNCTION,
                          CHOSEN_ADDITIONAL_BUTTON_5_FUNCTION,
                          CHOSEN_ADDITIONAL_BUTTON_6_FUNCTION
};

// *********************************************************************************

void displayUpdateFromWit() {
  if ( (keypadUseType==KEYPAD_USE_OPERATION) && (!menuIsShowing) ) {
    writeOledSpeed();
  }
}

// WiThrottleProtocol Delegate class
class MyDelegate : public WiThrottleProtocolDelegate {
  
  public:
    void heartbeatConfig(int seconds) { 
      debug_print("Received heartbeat. From: "); debug_print(heartBeatPeriod); 
      debug_print(" To: "); debug_println(seconds); 
      heartBeatPeriod = seconds;
    }
    void receivedVersion(String version) {    
      debug_printf("Received Version: %s\n",version); 
    }
    void receivedServerDescription(String description) {
      debug_print("Received Description: ");
      debug_println(description);
    }
    void receivedSpeed(int speed) {             // Vnnn
      debug_print("Received Speed: "); debug_println(speed); 
      if (currentSpeed != speed) {
        currentSpeed = speed;
        displayUpdateFromWit();
      }
    }
    void receivedDirection(Direction dir) {     // R{0,1}
      debug_print("Received Direction: "); debug_println(dir); 
      if (currentDirection != dir) {
        currentDirection = dir;
        displayUpdateFromWit();
      }
    }
    void receivedFunctionState(uint8_t func, bool state) { 
      debug_print("Received Fn: "); debug_print(func); debug_print(" State: "); debug_println( (state) ? "True" : "False" );
      if (functionStates[func] != state) {
        functionStates[func] = state;
        displayUpdateFromWit();
      }
    }
    void receivedTrackPower(TrackPower state) { 
      debug_print("Received TrackPower: "); debug_println(state);
      if (trackPower != state) {
        trackPower = state;
        displayUpdateFromWit();
      }
    }
    void receivedRosterEntries(int size) {
      debug_print("Received Roster Entries. Size: "); debug_println(size);
      rosterSize = size;
    }
    void receivedRosterEntry(int index, String name, int address, char length) {
      debug_print("Received Roster Entry, index: "); debug_print(index); debug_println(" - " + name);
      if (index < maxRoster) {
        rosterIndex[index] = index; 
        rosterName[index] = name; 
        rosterAddress[index] = address;
        rosterLength[index] = length;
      }
    }
    void receivedTurnoutEntries(int size) {
      debug_print("Received Turnout Entries. Size: "); debug_println(size);
      turnoutListSize = size;
    }
    void receivedTurnoutEntry(int index, String sysName, String userName, int state) {
      if (index < maxTurnoutList) {
        turnoutListIndex[index] = index; 
        turnoutListSysName[index] = sysName; 
        turnoutListUserName[index] = userName;
        turnoutListState[index] = state;
      }
    }
    void receivedRouteEntries(int size) {
      debug_print("Received Route Entries. Size: "); debug_println(size);
      routeListSize = size;
    }
    void receivedRouteEntry(int index, String sysName, String userName, int state) {
      if (index < maxRouteList) {
        routeListIndex[index] = index; 
        routeListSysName[index] = sysName; 
        routeListUserName[index] = userName;
        routeListState[index] = state;
      }
    }
};

WiFiClient client;
WiThrottleProtocol wiThrottleProtocol;
MyDelegate myDelegate;
int deviceId = random(1000,9999);

// *********************************************************************************

void ssidsLoop() {
  if (ssidConnectionState == CONNECTION_STATE_DISCONNECTED) {
    if (ssidSelectionSource == SSID_CONNECTION_SOURCE_LIST) {
      showListOfSsids(); 
    } else {
      browseSsids();
    }
  }
  
  if (ssidConnectionState == CONNECTION_STATE_PASSWORD_ENTRY) {
    enterSsidPassword();
  }

  if (ssidConnectionState == CONNECTION_STATE_SELECTED) {
    connectSsid();
  }
}

void browseSsids() { // show the found SSIDs
  debug_println("browseSsids()");

  double startTime = millis();
  double nowTime = startTime;

  debug_println("Browsing for ssids");
  clearOledArray(); 
  setAppnameForOled();
  oledText[2] = msg_browsing_for_ssids;
  writeOledArray(false, false);

  int numSsids = WiFi.scanNetworks();
  while ( (numSsids == -1)
    && ((nowTime-startTime) <= 10000) ) { // try for 10 seconds
    delay(250);
    debug_print(".");
    nowTime = millis();
  }

  foundSsidsCount = 0;
  if (numSsids == -1) {
    debug_println("Couldn't get a wifi connection");

  } else {
    for (int thisSsid = 0; thisSsid < numSsids; thisSsid++) {
      /// remove duplicates (repeaters and mesh networks)
      boolean found = false;
      for (int i=0; i<foundSsidsCount; i++) {
        if (WiFi.SSID(thisSsid) == foundSsids[i]) {
          found = true;
          break;
        }
      }
      if (!found) {
        foundSsids[foundSsidsCount] = WiFi.SSID(thisSsid);
        foundSsidsOpen[foundSsidsCount] = (WiFi.encryptionType(thisSsid) == 7) ? true : false;
        foundSsidsCount++;
      }
    }
    for (int i=0; i<foundSsidsCount; i++) {
      debug_println(foundSsids[i]);      
    }

    clearOledArray(); oledText[10] = msg_ssids_found;

     writeOledFoundSSids("");

    oledText[5] = menu_select_ssids_from_found;
    writeOledArray(false, false);

    keypadUseType = KEYPAD_USE_SELECT_SSID_FROM_FOUND;
    ssidConnectionState = CONNECTION_STATE_SELECTION_REQUIRED;
  }
}

void selectSsidFromFound(int selection) {
  debug_print("selectSsid() "); debug_println(selection);

  if ((selection>=0) && (selection < maxFoundSsids)) {
    ssidConnectionState = CONNECTION_STATE_SELECTED;
    selectedSsid = foundSsids[selection];
    getSsidPasswordAndWitIpForFound();
  }
  if (selectedSsidPassword=="") {
    ssidConnectionState = CONNECTION_STATE_PASSWORD_ENTRY;
  }
}

void getSsidPasswordAndWitIpForFound() {
    selectedSsidPassword = "";
    turnoutPrefix = "";
    routePrefix = "";
    if (selectedSsid.substring(0,6) == "DCCEX_") {
      selectedSsidPassword = "PASS_" + selectedSsid.substring(6);
      witServerIpAndPortEntered = "19216800400102560";
    } else {
      for (int i = 0; i < maxSsids; ++i) {
        if (selectedSsid == ssids[i]) {
          selectedSsidPassword = passwords[i];
          turnoutPrefix = turnoutPrefixes[i];
          routePrefix = routePrefixes[i];
          break;
        }
      }
    }
}

void enterSsidPassword() {
  keypadUseType = KEYPAD_USE_ENTER_SSID_PASSWORD;
  encoderUseType = ENCODER_USE_SSID_PASSWORD;
  if (ssidPasswordChanged) { // don't refresh the screen if nothing nothing has changed
    debug_println("enterWitServer()");
    writeOledEnterPassword();
    ssidPasswordChanged = false;
  }
}
void showListOfSsids() {  // show the list from the specified values in config_network.h
  debug_println("showListOfSsids()");

  clearOledArray(); 
  setAppnameForOled(); 
  writeOledArray(false, false);

  if (maxSsids == 0) {
    oledText[1] = msg_no_ssids_found;
    writeOledArray(false, false);
    debug_println(oledText[1]);
  
  } else {
    debug_print(maxSsids);  debug_println(msg_ssids_listed);
    clearOledArray(); oledText[10] = msg_ssids_listed;

    for (int i = 0; i < maxSsids; ++i) {
      debug_print(i+1); debug_print(": "); debug_println(ssids[i]);
      int j = i;
      if (i>=5) { 
        j=i+1;
      } 
      if (i<=10) {  // only have room for 10
        oledText[j] = String(i) + ": ";
        if (ssids[i].length()<9) {
          oledText[j] = oledText[j] + ssids[i];
        } else {
          oledText[j] = oledText[j] + ssids[i].substring(0,9) + "..";
        }
      }
    }

    if (maxSsids > 0) {
      oledText[5] = menu_select_ssids;
    }
    writeOledArray(false, false);

    if (maxSsids == 1) {
      selectedSsid = ssids[0];
      selectedSsidPassword = passwords[0];
      ssidConnectionState = CONNECTION_STATE_SELECTED;

      turnoutPrefix = turnoutPrefixes[0];
      routePrefix = routePrefixes[0];
      
    } else {
      ssidConnectionState = CONNECTION_STATE_SELECTION_REQUIRED;
      keypadUseType = KEYPAD_USE_SELECT_SSID;
    }
  }
}

void selectSsid(int selection) {
  debug_print("selectSsid() "); debug_println(selection);

  if ((selection>=0) && (selection < maxSsids)) {
    ssidConnectionState = CONNECTION_STATE_SELECTED;
    selectedSsid = ssids[selection];
    selectedSsidPassword = passwords[selection];
    
    turnoutPrefix = turnoutPrefixes[selection];
    routePrefix = routePrefixes[selection];
  }
}

void connectSsid() {
  debug_println("Connecting to ssid...");
  clearOledArray(); 
  setAppnameForOled();
  oledText[1] = selectedSsid; oledText[2] + "connecting...";
  writeOledArray(false, false);

  double startTime = millis();
  double nowTime = startTime;

  const char *cSsid = selectedSsid.c_str();
  const char *cPassword = selectedSsidPassword.c_str();

  if (cSsid!="") {
    debug_print("Trying Network "); debug_println(cSsid);
    clearOledArray(); 
    setAppnameForOled(); 
    for (int i = 0; i < 3; ++i) {  // Try three times
      oledText[1] = selectedSsid; oledText[2] =  msg_trying_to_connect + " (" + String(i) + ")";
      writeOledArray(false, false);

      nowTime = startTime;      
      WiFi.begin(cSsid, cPassword); 

      debug_print("Trying Network ... Checking status "); debug_print(cSsid); debug_print(" :"); debug_print(cPassword); debug_println(":");
      while ( (WiFi.status() != WL_CONNECTED) 
        && ((nowTime-startTime) <= 10000) ) { // wait for X seconds to see if the connection worked
        delay(250);
        debug_print(".");
        nowTime = millis();
      }

      if (WiFi.status() == WL_CONNECTED) {
        break; 
      } else { // if not loop back and try again
        debug_println("");
      }
    }

    debug_println("");
    if (WiFi.status() == WL_CONNECTED) {
      debug_print("Connected. IP address: "); debug_println(WiFi.localIP());
      oledText[2] = msg_connected; 
      oledText[3] = msg_address_label + String(WiFi.localIP());
      writeOledArray(false, false);
      // ssidConnected = true;
      ssidConnectionState = CONNECTION_STATE_CONNECTED;
      keypadUseType = KEYPAD_USE_SELECT_WITHROTTLE_SERVER;

      // setup the bonjour listener
      if (!MDNS.begin("ESP32_Browser")) {
        debug_println("Error setting up MDNS responder!");
        oledText[2] = msg_bounjour_setup_failed;
        writeOledArray(false, false);
        delay(2000);
        ssidConnectionState = CONNECTION_STATE_DISCONNECTED;
      }

    } else {
      debug_println(msg_connection_failed);
      oledText[2] = msg_connection_failed;
      writeOledArray(false, false);
      delay(2000);
      
      WiFi.disconnect();      
      ssidConnectionState = CONNECTION_STATE_DISCONNECTED;
      ssidSelectionSource = SSID_CONNECTION_SOURCE_LIST;
    }
  }
}

// *********************************************************************************

void witServiceLoop() {
  if (witConnectionState == CONNECTION_STATE_DISCONNECTED) {
    browseWitService(); 
  }

  if (witConnectionState == CONNECTION_STATE_ENTRY_REQUIRED) {
    enterWitServer();
  }

  if ( (witConnectionState == CONNECTION_STATE_SELECTED) 
  || (witConnectionState == CONNECTION_STATE_ENTERED) ) {
    connectWitServer();
  }
}

void browseWitService() {
  debug_println("browseWitService()");

  keypadUseType = KEYPAD_USE_SELECT_WITHROTTLE_SERVER;

  double startTime = millis();
  double nowTime = startTime;

  const char * service = "withrottle";
  const char * proto= "tcp";

  debug_printf("Browsing for service _%s._%s.local. on %s ... ", service, proto, selectedSsid);
  clearOledArray(); 
  oledText[0] = appName; oledText[6] = appVersion; 
  oledText[1] = selectedSsid;   oledText[2] = msg_browsing_for_service;
  writeOledArray(false, false);

  noOfWitServices = 0;
  while ( (noOfWitServices == 0) 
    && ((nowTime-startTime) <= 5000) ) { // try for 5 seconds
    noOfWitServices = MDNS.queryService(service, proto);
    if (noOfWitServices == 0 ) {
      delay(500);
      debug_print(".");
    }
    nowTime = millis();
  }

  foundWitServersCount = noOfWitServices;
  if (noOfWitServices > 0) {
    for (int i = 0; ((i < noOfWitServices) && (i<maxFoundWitServers)); ++i) {
      foundWitServersNames[i] = MDNS.hostname(i);
      foundWitServersIPs[i] = MDNS.IP(i);
      foundWitServersPorts[i] = MDNS.port(i);
    }
  }
  if (selectedSsid.substring(0,6) == "DCCEX_") {
    foundWitServersIPs[foundWitServersCount].fromString("192.168.4.1");
    foundWitServersPorts[foundWitServersCount] = 2560;
    foundWitServersNames[foundWitServersCount] = "'Guessed' DCC++ WiT server";
    foundWitServersCount++;
  }

  if (foundWitServersCount == 0) {
    oledText[1] = msg_no_services_found;
    writeOledArray(false, false);
    debug_println(oledText[1]);
    delay(1000);
    buildWitEntry();
    witConnectionState = CONNECTION_STATE_ENTRY_REQUIRED;
  
  } else {
    debug_print(noOfWitServices);  debug_println(msg_services_found);
    clearOledArray(); oledText[1] = msg_services_found;

    for (int i = 0; i < foundWitServersCount; ++i) {
      // Print details for each service found
      debug_print("  "); debug_print(i); debug_print(": "); debug_print(foundWitServersNames[i]);
      debug_print(" ("); debug_print(foundWitServersIPs[i]); debug_print(":"); debug_print(foundWitServersPorts[i]); debug_println(")");
      if (i<5) {  // only have room for 5
        String truncatedIp = ".." + foundWitServersIPs[i].toString().substring(foundWitServersIPs[i].toString().lastIndexOf("."));
        oledText[i] = String(i) + ": " + truncatedIp + ":" + String(foundWitServersPorts[i]) + " " + foundWitServersNames[i];
      }
    }

    if (foundWitServersCount > 0) {
      oledText[5] = menu_select_wit_service;
    }
    writeOledArray(false, false);

    if (foundWitServersCount == 1) {
      debug_println("WiT Selection - only 1");
      selectedWitServerIP = foundWitServersIPs[0];
      selectedWitServerPort = foundWitServersPorts[0];
      selectedWitServerName = foundWitServersNames[0];
      witConnectionState = CONNECTION_STATE_SELECTED;
    } else {
      debug_println("WiT Selection required");
      witConnectionState = CONNECTION_STATE_SELECTION_REQUIRED;
    }
  }
}

void selectWitServer(int selection) {
  debug_print("selectWitServer() "); debug_println(selection);

  if ((selection>=0) && (selection < foundWitServersCount)) {
    witConnectionState = CONNECTION_STATE_SELECTED;
    selectedWitServerIP = foundWitServersIPs[selection];
    selectedWitServerPort = foundWitServersPorts[selection];
    selectedWitServerName = foundWitServersNames[selection];
    keypadUseType = KEYPAD_USE_OPERATION;
  }
}

void connectWitServer() {
  // Pass the delegate instance to wiThrottleProtocol
  wiThrottleProtocol.setDelegate(&myDelegate);
  // Uncomment for logging on Serial
  // wiThrottleProtocol.setLogStream(&Serial);

  debug_println("Connecting to the server...");
  clearOledArray(); 
  setAppnameForOled(); 
  oledText[1] = selectedWitServerIP.toString() + " : " + String(selectedWitServerPort); 
  oledText[2] = selectedWitServerName; oledText[3] + "connecting...";
  writeOledArray(false, false);

  if (!client.connect(selectedWitServerIP, selectedWitServerPort)) {
    debug_println(msg_connection_failed);
    oledText[3] = msg_connection_failed;
    writeOledArray(false, false);
    delay(5000);
    
    witConnectionState = CONNECTION_STATE_DISCONNECTED;
    ssidConnectionState = CONNECTION_STATE_DISCONNECTED;
    ssidSelectionSource = SSID_CONNECTION_SOURCE_LIST;
    witServerIpAndPortChanged = true;

  } else {
    debug_print("Connected to server: ");   debug_println(selectedWitServerIP); debug_println(selectedWitServerPort);

    // Pass the communication to WiThrottle
    wiThrottleProtocol.connect(&client);
    debug_println("WiThrottle connected");

    wiThrottleProtocol.setDeviceName(deviceName);  
    wiThrottleProtocol.setDeviceID(String(deviceId));  

    witConnectionState = CONNECTION_STATE_CONNECTED;
    setLastServerResponseTime(true);

    oledText[3] = msg_connected;
    oledText[5] = menu_menu;
    writeOledArray(false, false);

    keypadUseType = KEYPAD_USE_OPERATION;
  }
}

void enterWitServer() {
  keypadUseType = KEYPAD_USE_ENTER_WITHROTTLE_SERVER;
  if (witServerIpAndPortChanged) { // don't refresh the screen if nothing nothing has changed
    debug_println("enterWitServer()");
    clearOledArray(); 
    setAppnameForOled(); 
    oledText[1] = msg_no_services_found_entry_required;
    oledText[3] = witServerIpAndPortConstructed;
    oledText[5] = menu_select_wit_entry;
    writeOledArray(false, false);
    witServerIpAndPortChanged = false;
  }
}

void disconnectWitServer() {
  debug_println("disconnectWitServer()");
  releaseAllLocos();
  wiThrottleProtocol.disconnect();
  debug_println("Disconnected from wiThrottle server\n");
  clearOledArray(); oledText[0] = msg_disconnected;
  writeOledArray(false, false);
  witConnectionState = CONNECTION_STATE_DISCONNECTED;
  witServerIpAndPortChanged = true;
}

void witEntryAddChar(char key) {
  if (witServerIpAndPortEntered.length() < 17) {
    witServerIpAndPortEntered = witServerIpAndPortEntered + key;
    debug_print("wit entered: ");
    debug_println(witServerIpAndPortEntered);
    buildWitEntry();
    witServerIpAndPortChanged = true;
  }
}

void witEntryDeleteChar(char key) {
  if (witServerIpAndPortEntered.length() > 0) {
    witServerIpAndPortEntered = witServerIpAndPortEntered.substring(0, witServerIpAndPortEntered.length()-1);
    debug_print("wit deleted: ");
    debug_println(witServerIpAndPortEntered);
    buildWitEntry();
    witServerIpAndPortChanged = true;
  }
}

void ssidPasswordAddChar(char key) {
  ssidPasswordEntered = ssidPasswordEntered + key;
  debug_print("password entered: ");
  debug_println(ssidPasswordEntered);
  ssidPasswordChanged = true;
  ssidPasswordCurrentChar = ssidPasswordBlankChar;
}

void ssidPasswordDeleteChar(char key) {
  if (ssidPasswordEntered.length() > 0) {
    ssidPasswordEntered = ssidPasswordEntered.substring(0, ssidPasswordEntered.length()-1);
    debug_print("password char deleted: ");
    debug_println(ssidPasswordEntered);
    ssidPasswordChanged = true;
    ssidPasswordCurrentChar = ssidPasswordBlankChar;
  }
}

void buildWitEntry() {
  debug_println("buildWitEntry()");
  witServerIpAndPortConstructed = "";
  for (int i=0; i < witServerIpAndPortEntered.length(); i++) {
    if ( (i==3) || (i==6) || (i==9) ) {
      witServerIpAndPortConstructed = witServerIpAndPortConstructed + ".";
    } else if (i==12) {
      witServerIpAndPortConstructed = witServerIpAndPortConstructed + ":";
    }
    witServerIpAndPortConstructed = witServerIpAndPortConstructed + witServerIpAndPortEntered.substring(i,i+1);
  }
  debug_print("wit Constructed: ");
  debug_println(witServerIpAndPortConstructed);
  if (witServerIpAndPortEntered.length() < 17) {
    witServerIpAndPortConstructed = witServerIpAndPortConstructed + witServerIpAndPortEntryMask.substring(witServerIpAndPortConstructed.length());
  }
  debug_print("wit Constructed: ");
  debug_println(witServerIpAndPortConstructed);

  if (witServerIpAndPortEntered.length() == 17) {
     selectedWitServerIP.fromString( witServerIpAndPortConstructed.substring(0,15));
     selectedWitServerPort = witServerIpAndPortConstructed.substring(16).toInt();
  }
}

// *********************************************************************************
//   Encoder
// *********************************************************************************

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
void IRAM_ATTR readEncoderISR() {
  rotaryEncoder.readEncoder_ISR();
}

void rotary_onButtonClick() {
   if (encoderUseType == ENCODER_USE_OPERATION) {
    if ( (keypadUseType!=KEYPAD_USE_SELECT_WITHROTTLE_SERVER)
        && (keypadUseType!=KEYPAD_USE_ENTER_WITHROTTLE_SERVER)
        && (keypadUseType!=KEYPAD_USE_SELECT_SSID) 
        && (keypadUseType!=KEYPAD_USE_SELECT_SSID_FROM_FOUND) ) {
      static unsigned long lastTimePressed = 0;
      if (millis() - lastTimePressed < encoderDebounceTime) {   //ignore multiple press in that time milliseconds
        debug_println("encoder button debounce");
        return;
      }
      lastTimePressed = millis();
      if (wiThrottleProtocol.getNumberOfLocomotives()>0) {
        if (currentSpeed!=0) {
          wiThrottleProtocol.setSpeed(0);
        } else {
          if (toggleDirectionOnEncoderButtonPressWhenStationary) toggleDirection();
        }
        currentSpeed = 0;
      }
      debug_println("encoder button pressed");
      writeOledSpeed();
    }  else {
      deepSleepStart();
    }
   } else {
    if (ssidPasswordCurrentChar!=ssidPasswordBlankChar) {
      ssidPasswordEntered = ssidPasswordEntered + ssidPasswordCurrentChar;
      ssidPasswordCurrentChar = ssidPasswordBlankChar;
      writeOledEnterPassword();
    }
   }
}

void rotary_loop() {
  if (rotaryEncoder.encoderChanged()) {   //don't print anything unless value changed
    encoderValue = rotaryEncoder.readEncoder();
     debug_print("Encoder From: "); debug_print(lastEncoderValue);  debug_print(" to: "); debug_println(encoderValue);
    if (abs(encoderValue-lastEncoderValue) > 800) { // must have passed through zero
      if (encoderValue > 800) {
        lastEncoderValue = 1001; 
      } else {
        lastEncoderValue = 0; 
      }
      debug_print("Corrected Encoder From: "); debug_print(lastEncoderValue); debug_print(" to: "); debug_println(encoderValue);
    }
 
    if (encoderUseType == ENCODER_USE_OPERATION) {
      if (wiThrottleProtocol.getNumberOfLocomotives()>0) {
        if (encoderValue > lastEncoderValue) {
          if (abs(encoderValue-lastEncoderValue)<50) {
            encoderSpeedChange(true, currentSpeedStep);
          } else {
            encoderSpeedChange(true, currentSpeedStep*speedStepMultiplier);
          }
        } else {
          if (abs(encoderValue-lastEncoderValue)<50) {
            encoderSpeedChange(false, currentSpeedStep);
          } else {
            encoderSpeedChange(false, currentSpeedStep*speedStepMultiplier);
          }
        } 
      }
    } else { // (encoderUseType == ENCODER_USE_SSID_PASSWORD) 
        if (encoderValue > lastEncoderValue) {
          if (ssidPasswordCurrentChar==ssidPasswordBlankChar) {
            ssidPasswordCurrentChar = 66; // 'B'
          } else {
            ssidPasswordCurrentChar = ssidPasswordCurrentChar - 1;
            if ((ssidPasswordCurrentChar < 32) ||(ssidPasswordCurrentChar > 126) ) {
              ssidPasswordCurrentChar = 126;  // '~'
            }
          }
        } else {
          if (ssidPasswordCurrentChar==ssidPasswordBlankChar) {
            ssidPasswordCurrentChar = 64; // '@'
          } else {
            ssidPasswordCurrentChar = ssidPasswordCurrentChar + 1;
            if (ssidPasswordCurrentChar > 126) {
              ssidPasswordCurrentChar = 32; // ' ' space
            }
          }
        }
        ssidPasswordChanged = true;
        writeOledEnterPassword();
    }
    lastEncoderValue = encoderValue;
  }
  
  if (rotaryEncoder.isEncoderButtonClicked()) {
    rotary_onButtonClick();
  }
}

void encoderSpeedChange(boolean rotationIsClockwise, int speedChange) {
  if (encoderRotationClockwiseIsIncreaseSpeed) {
    if (rotationIsClockwise) {
      speedUp(speedChange);
    } else {
      speedDown(speedChange);
    }
  } else {
    if (rotationIsClockwise) {
      speedDown(speedChange);
    } else {
      speedUp(speedChange);
    }
  }
}

// *********************************************************************************
//   keypad
// *********************************************************************************

void keypadEvent(KeypadEvent key) {
  switch (keypad.getState()){
  case PRESSED:
    debug_print("Button "); debug_print(String(key - '0')); debug_println(" pushed.");
    doKeyPress(key, true);
    break;
  case RELEASED:
    doKeyPress(key, false);
    debug_print("Button "); debug_print(String(key - '0')); debug_println(" released.");
    break;
  case HOLD:
    debug_print("Button "); debug_print(String(key - '0')); debug_println(" hold.");
    break;
  case IDLE:
    debug_print("Button "); debug_print(String(key - '0')); debug_println(" idle.");
    break;
  default:
    debug_print("Button "); debug_print(String(key - '0')); debug_println(" unknown.");
  }
}


// *********************************************************************************
//   Optional Additional Buttons
// *********************************************************************************

void initialiseAdditionalButtons() {
    for (int i = 0; i < MAX_ADDITIONAL_BUTTONS; i++) { 
      if (additionalButtonActions[i] != FUNCTION_NULL) { 
        debug_print("Additional Button: "); debug_print(i); debug_print(" pin:"); debug_println(additionalButtonPin[i]);
        pinMode(additionalButtonPin[i], additionalButtonType[i]);
        if (additionalButtonType[i] == INPUT_PULLUP) {
          additionalButtonLastRead[i] = HIGH;
        } else {
          additionalButtonLastRead[i] = LOW;
        }
      }
  }
}

void additionalButtonLoop() {
  if (wiThrottleProtocol.getNumberOfLocomotives()>0) { // only process if there are locos aquired
    for (int i = 0; i < MAX_ADDITIONAL_BUTTONS; i++) {    
      if (additionalButtonActions[i] != FUNCTION_NULL) {
        additionalButtonRead[i] = digitalRead(additionalButtonPin[i]);
        // if ( ( ((additionalButtonType[i] == INPUT_PULLUP) && (additionalButtonRead[i] == LOW)) 
        //       || ((additionalButtonType[i] == INPUT) && (additionalButtonRead[i] == HIGH)) )
        //     && (additionalButtonLastRead[i] != additionalButtonRead[i]) )     { // on the first press only

        if (additionalButtonLastRead[i] != additionalButtonRead[i]) { // on procces on a change
          if ( ((additionalButtonType[i] == INPUT_PULLUP) && (additionalButtonRead[i] == LOW)) 
              || ((additionalButtonType[i] == INPUT) && (additionalButtonRead[i] == HIGH)) ) {
            debug_print("Additional Button Pressed: "); debug_print(i); debug_print(" pin:"); debug_print(additionalButtonPin[i]); debug_print(" action:"); debug_println(additionalButtonActions[i]); 
            doDirectAdditionalButtonCommand(i,true);
          }else {
            debug_print("Additional Button Released: "); debug_print(i); debug_print(" pin:"); debug_print(additionalButtonPin[i]); debug_print(" action:"); debug_println(additionalButtonActions[i]); 
            doDirectAdditionalButtonCommand(i,false);
          }
        }

        additionalButtonLastRead[i] = additionalButtonRead[i];
      }
    }
  }
}

// *********************************************************************************
//  Setup and Loop
// *********************************************************************************

void setup() {
  Serial.begin(115200);
  u8g2.begin();
  // i2cSetClock(0,400000);

  clearOledArray(); oledText[0] = appName; oledText[6] = appVersion; oledText[2] = msg_start;
  writeOledArray(false, false);

  delay(1000);
  debug_println("Start"); 

  rotaryEncoder.begin();  //initialize rotary encoder
  rotaryEncoder.setup(readEncoderISR);
  //set boundaries and if values should cycle or not 
  rotaryEncoder.setBoundaries(0, 1000, circleValues); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
  //rotaryEncoder.disableAcceleration(); //acceleration is now enabled by default - disable if you don't need it
  rotaryEncoder.setAcceleration(100); //or set the value - larger number = more acceleration; 0 or 1 means disabled acceleration

  keypad.addEventListener(keypadEvent); // Add an event listener for this keypad
  keypad.setDebounceTime(keypadDebounceTime);
  keypad.setHoldTime(keypadHoldTime);

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13,0); //1 = High, 0 = Low

  keypadUseType = KEYPAD_USE_SELECT_SSID;
  encoderUseType = ENCODER_USE_OPERATION;
  ssidSelectionSource = SSID_CONNECTION_SOURCE_BROWSE;

  initialiseAdditionalButtons();
}

void loop() {
  
  if (ssidConnectionState != CONNECTION_STATE_CONNECTED) {
    // connectNetwork();
    ssidsLoop();
  } else {  
    if (witConnectionState != CONNECTION_STATE_CONNECTED) {
      witServiceLoop();
    } else {
      wiThrottleProtocol.check();    // parse incoming messages

      setLastServerResponseTime(false);

      if ( (lastServerResponseTime+(heartBeatPeriod*4) < millis()/1000) 
      && (heartbeatCheckEnabled) ) {
        debug_print("Disconnected - Last:");  debug_print(lastServerResponseTime); debug_print(" Current:");  debug_println(millis()/1000);
        reconnect();
      }
    }
  }
  char key = keypad.getKey();
  rotary_loop();

  additionalButtonLoop(); 

	// debug_println("loop:" );
}

// *********************************************************************************
//  Key press and Menu
// *********************************************************************************

void doKeyPress(char key, boolean pressed) {
  if (pressed)  { //pressed
    switch (keypadUseType) {
      case KEYPAD_USE_OPERATION:
        debug_print("key operation... "); debug_println(key);
        switch (key) {
          case '*':  // menu command
            menuCommand = "";
            if (menuCommandStarted) { // then cancel the menu
              resetMenu();
              writeOledSpeed();
            } else {
              menuCommandStarted = true;
              debug_println("Command started");
              writeOledMenu("");
            }
            break;

          case '#': // end of command
            if ((menuCommandStarted) && (menuCommand.length()>=1)) {
              doMenu();
            } else {
              writeOledDirectCommands();
            }
            break;

          case '0': case '1': case '2': case '3': case '4': 
          case '5': case '6': case '7': case '8': case '9':
            if (menuCommandStarted) { // append to the string
              menuCommand += key;
              writeOledMenu(menuCommand);
            } else {
              doDirectCommand(key, true);
            }
            break;

          default:  //A, B, C, D
            doDirectCommand(key, true);
            break;
        }
        break;

      case KEYPAD_USE_ENTER_WITHROTTLE_SERVER:
        debug_print("key: Enter wit... "); debug_println(key);
        switch (key){
          case '0': case '1': case '2': case '3': case '4': 
          case '5': case '6': case '7': case '8': case '9':
            witEntryAddChar(key);
            break;
          case '*': // backspace
            witEntryDeleteChar(key);
            break;
          case '#': // end of command
            if (witServerIpAndPortEntered.length() == 17) {
              witConnectionState = CONNECTION_STATE_ENTERED;
            }
            break;
          default:  // do nothing 
            break;
        }

        break;

      case KEYPAD_USE_ENTER_SSID_PASSWORD:
        debug_print("key: Enter password... "); debug_println(key);
        switch (key){
          case '0': case '1': case '2': case '3': case '4': 
          case '5': case '6': case '7': case '8': case '9':
            ssidPasswordAddChar(key);
            break;
          case '*': // backspace
            ssidPasswordDeleteChar(key);
            break;
          case '#': // end of command
              selectedSsidPassword = ssidPasswordEntered;
              encoderUseType = ENCODER_USE_OPERATION;
              keypadUseType = KEYPAD_USE_OPERATION;
              ssidConnectionState = CONNECTION_STATE_SELECTED;
            break;
          default:  // do nothing 
            break;
        }

        break;

      case KEYPAD_USE_SELECT_WITHROTTLE_SERVER:
        debug_print("key: Select wit... "); debug_println(key);
        switch (key){
          case '0': case '1': case '2': case '3': case '4':
            selectWitServer(key - '0');
            break;
          case '#': // show ip address entry screen
            witConnectionState = CONNECTION_STATE_ENTRY_REQUIRED;
            enterWitServer();
            break;
          default:  // do nothing 
            break;
        }
        break;

      case KEYPAD_USE_SELECT_SSID:
        debug_print("key ssid... "); debug_println(key);
        switch (key){
          case '0': case '1': case '2': case '3': case '4': 
          case '5': case '6': case '7': case '8': case '9':
            selectSsid(key - '0');
            break;
          case '#': // show found SSIds
            ssidConnectionState = CONNECTION_STATE_DISCONNECTED;
            keypadUseType = KEYPAD_USE_SELECT_SSID_FROM_FOUND;
            ssidSelectionSource = SSID_CONNECTION_SOURCE_BROWSE;
            // browseSsids();
            break;
          default:  // do nothing 
            break;
        }
        break;

      case KEYPAD_USE_SELECT_SSID_FROM_FOUND:
        debug_print("key ssid from found... "); debug_println(key);
        switch (key){
          case '0': case '1': case '2': case '3': case '4': 
            selectSsidFromFound(key - '0'+(page*5));
            break;
          case '#': // next page
            if (foundSsidsCount>5) {
              if ( (page+1)*5 < foundSsidsCount ) {
                page++;
              } else {
                page = 0;
              }
              writeOledFoundSSids(""); 
            }
            break;
          case '9': // show in code list of SSIDs
            ssidConnectionState = CONNECTION_STATE_DISCONNECTED;
            keypadUseType = KEYPAD_USE_SELECT_SSID;
            ssidSelectionSource = SSID_CONNECTION_SOURCE_LIST;
            break;
          default:  // do nothing 
            break;
        }
        break;

      case KEYPAD_USE_SELECT_ROSTER:
        debug_print("key Roster... "); debug_println(key);
        switch (key){
          case '0': case '1': case '2': case '3': case '4': 
          case '5': case '6': case '7': case '8': case '9':
            selectRoster((key - '0')+(page*5));
            break;
          case '#':  // next page
            if ( rosterSize > 5 ) {
              if ( (page+1)*5 < rosterSize ) {
                page++;
              } else {
                page = 0;
              }
              writeOledRoster(""); 
            }
            break;
          case '*':  // cancel
            resetMenu();
            writeOledSpeed();
            break;
          default:  // do nothing 
            break;
        }
        break;

      case KEYPAD_USE_SELECT_TURNOUTS_THROW:
      case KEYPAD_USE_SELECT_TURNOUTS_CLOSE:
        debug_print("key turnouts... "); debug_println(key);
        switch (key){
          case '0': case '1': case '2': case '3': case '4': 
          case '5': case '6': case '7': case '8': case '9':
            selectTurnoutList((key - '0')+(page*10), (keypadUseType == KEYPAD_USE_SELECT_TURNOUTS_THROW) ? TurnoutThrow : TurnoutClose);
            break;
          case '#':  // next page
            if ( turnoutListSize > 10 ) {
              if ( (page+1)*10 < turnoutListSize ) {
                page++;
              } else {
                page = 0;
              }
              writeOledTurnoutList("", (keypadUseType == KEYPAD_USE_SELECT_TURNOUTS_THROW) ? TurnoutThrow : TurnoutClose); 
            }
            break;
          case '*':  // cancel
            resetMenu();
            writeOledSpeed();
            break;
          default:  // do nothing 
            break;
        }
        break;

      case KEYPAD_USE_SELECT_ROUTES:
        debug_print("key routes... "); debug_println(key);
        switch (key){
          case '0': case '1': case '2': case '3': case '4': 
          case '5': case '6': case '7': case '8': case '9':
            selectRouteList((key - '0')+(page*10));
            break;
          case '#':  // next page
            if ( routeListSize > 10 ) {
              if ( (page+1)*10 < routeListSize ) {
                page++;
              } else {
                page = 0;
              }
              writeOledRouteList(""); 
            }
            break;
          case '*':  // cancel
            resetMenu();
            writeOledSpeed();
            break;
          default:  // do nothing 
            break;
        }
        break;

      default:  // do nothing 
        break;
    }

  } else {  // released
    if (keypadUseType == KEYPAD_USE_OPERATION) {
      if ( (!menuCommandStarted) && (key>='0') && (key<='D')) { // only process releases for the numeric keys + A,B,C,D and only if a menu command has not be started
        debug_println("Operation - Process key release");
        doDirectCommand(key, false);
      } else {
        debug_println("Non-Operation - Process key release");
      }
    }
  }
}

void doDirectCommand (char key, boolean pressed) {
  debug_print("Direct command: "); debug_println(key);
  int buttonAction = buttonActions[(key - '0')];
  if (buttonAction!=FUNCTION_NULL) {
    if ( (buttonAction>=FUNCTION_0) && (buttonAction<=FUNCTION_28) ) {
      doDirectFunction(buttonAction, pressed);
  } else {
      if (pressed) { // only process these on the key press, not the release
        doDirectAction(buttonAction);
      }
    }
  }
}

void doDirectAdditionalButtonCommand (int buttonIndex, boolean pressed) {
  debug_print("Direct Additional Button command: "); debug_println(buttonIndex);
  int buttonAction = additionalButtonActions[buttonIndex];
  if (buttonAction!=FUNCTION_NULL) {
    if ( (buttonAction>=FUNCTION_0) && (buttonAction<=FUNCTION_28) ) {
      doDirectFunction(buttonAction, pressed);
  } else {
      if (pressed) { // only process these on the key press, not the release
        doDirectAction(buttonAction);
      }
    }
  }
}

void doDirectAction(int buttonAction) {
  switch (buttonAction) {
    case DIRECTION_FORWARD: {
      changeDirection(Forward);
      break; 
    }
    case DIRECTION_REVERSE: {
      changeDirection(Reverse);
      break; 
    }
    case DIRECTION_TOGGLE: {
      toggleDirection();
      break; 
    }
    case SPEED_STOP: {
      speedSet(0);
      break; 
    }
    case SPEED_UP: {
      speedUp(currentSpeedStep);
      break; 
    }
    case SPEED_DOWN: {
      speedDown(currentSpeedStep);
      break; 
    }
    case SPEED_UP_FAST: {
      speedUp(currentSpeedStep*speedStepMultiplier);
      break; 
    }
    case SPEED_DOWN_FAST: {
      speedUp(currentSpeedStep*speedStepMultiplier);
      break; 
    }
    case SPEED_MULTIPLIER: {
      toggleAdditionalMultiplier();
      break; 
    }
    case E_STOP: {
      speedEstop();
      break; 
    }
    case POWER_ON: {
      powerOnOff(PowerOn);
      break; 
    }
    case POWER_OFF: {
      powerOnOff(PowerOff);
      break; 
    }
    case POWER_TOGGLE: {
      powerToggle();
      break; 
    }
}
}

void doMenu() {
  String loco = "";
  String function = "";
  boolean result = false;
  debug_print("Menu: "); debug_println(menuCommand);
  
  switch (menuCommand[0]) {
    case '1': { // select loco
        if (menuCommand.length()>1) {
          loco = menuCommand.substring(1, menuCommand.length());
          loco = getLocoWithLength(loco);
          debug_print("add Loco: "); debug_println(loco);
          wiThrottleProtocol.addLocomotive(loco);
          resetFunctionStates();
          writeOledSpeed();
        } else {
          page = 0;
          writeOledRoster("");
        }
        break;
      }
    case '2': { // de-select loco
        loco = menuCommand.substring(1, menuCommand.length());
        if (loco!="") { // a loco is specified
          loco = getLocoWithLength(loco);
          debug_print("release Loco: "); debug_println(loco);
          wiThrottleProtocol.releaseLocomotive(loco);
        } else { //not loco specified so release all
          releaseAllLocos();
        }
        writeOledSpeed();
        break;
      }
    case '3': { // change direction
        toggleDirection();
        break;
      }
     case '4': { // toggle additional Multiplier
        toggleAdditionalMultiplier();
        break;
      }
   case '5': {  // throw turnout
        if (menuCommand.length()>1) {
          String turnout = turnoutPrefix + menuCommand.substring(1, menuCommand.length());
          // if (!turnout.equals("")) { // a turnout is specified
            debug_print("throw turnout: "); debug_println(turnout);
            wiThrottleProtocol.setTurnout(turnout, TurnoutThrow);
          // }
          writeOledSpeed();
        } else {
          page = 0;
          writeOledTurnoutList("", TurnoutThrow);
        }
        break;
      }
    case '6': {  // close turnout
        if (menuCommand.length()>1) {
          String turnout = turnoutPrefix + menuCommand.substring(1, menuCommand.length());
          // if (!turnout.equals("")) { // a turnout is specified
            debug_print("close turnout: "); debug_println(turnout);
            wiThrottleProtocol.setTurnout(turnout, TurnoutClose);
          // }
          writeOledSpeed();
        } else {
          page = 0;
          writeOledTurnoutList("",TurnoutClose);
        }
        break;
      }
    case '7': {  // route
        if (menuCommand.length()>1) {
          String route = routePrefix + menuCommand.substring(1, menuCommand.length());
          // if (!route.equals("")) { // a loco is specified
            debug_print("route: "); debug_println(route);
            wiThrottleProtocol.setRoute(route);
          // }
          writeOledSpeed();
        } else {
          page = 0;
          writeOledRouteList("");
        }
        break;
      }
    case '8': {
        powerToggle();
        debug_println("Power toggle");
        writeOledSpeed();
        break;
      }
    case '9': { // disconnect/reconnect/sleep
        String subcommand = menuCommand.substring(1, menuCommand.length());
        if (subcommand.equals("")) { // no subcommand is specified   
          if (witConnectionState == CONNECTION_STATE_CONNECTED) {
            witConnectionState == CONNECTION_STATE_DISCONNECTED;
            disconnectWitServer();
          } else {
            connectWitServer();
          }
        } else { // subcommand
          if (subcommand.equals("9")) { // sleep
            deepSleepStart();
          } else if (subcommand.equals("8")) { // disable/enable the heartbeat Check
            toggleHeartbeatCheck();
          }
        }
        break;
      }
    case '0': { // function button
        if (menuCommand.length()>1) {
          function = menuCommand.substring(1, menuCommand.length());
          int functionNumber = function.toInt();
          if (function != "") { // a function is specified
            doFunction(functionNumber, true);  // always act like latching i.e. pressed
          }
        }
        writeOledSpeed();
        break;
      }
  }
  menuCommandStarted = result; 
}

// *********************************************************************************
//  Actions
// *********************************************************************************

void resetMenu() {
  debug_println("resetMenu()");
  page = 0;
  menuCommand = "";
  menuCommandStarted = false;
  if ( (keypadUseType != KEYPAD_USE_SELECT_SSID) 
    && (keypadUseType != KEYPAD_USE_SELECT_WITHROTTLE_SERVER) ) {
    keypadUseType = KEYPAD_USE_OPERATION; 
  }
}

void resetFunctionStates() {
  for (int i=0; i<28; i++) {
    functionStates[i] = false;
  }
}

String getLocoWithLength(String loco) {
  int locoNo = loco.toInt();
  String locoWithLength = "";
  if ( (locoNo <= SHORT_DCC_ADDESS_LIMIT) && (loco.charAt(0)!='0') ) {
    locoWithLength = "S" + loco;
  } else {
    locoWithLength = "L" + loco;
  }
  return locoWithLength;
}

void speedEstop() {
  wiThrottleProtocol.emergencyStop();
  currentSpeed = 0;
  debug_println("Speed EStop"); 
  writeOledSpeed();
}

void speedDown(int amt) {
  if (wiThrottleProtocol.getNumberOfLocomotives()>0) {
    int newSpeed = currentSpeed - amt;
    debug_print("Speed Down: "); debug_println(amt);
    speedSet(newSpeed);
  }
}

void speedUp(int amt) {
  if (wiThrottleProtocol.getNumberOfLocomotives()>0) {
    int newSpeed = currentSpeed + amt;
    debug_print("Speed Up: "); debug_println(amt);
    speedSet(newSpeed);
  }
}

void speedSet(int amt) {
  if (wiThrottleProtocol.getNumberOfLocomotives()>0) {
    int newSpeed = amt;
    if (newSpeed >126) { newSpeed = 126; }
    if (newSpeed <0) { newSpeed = 0; }
    wiThrottleProtocol.setSpeed(newSpeed);
    currentSpeed = newSpeed;
    debug_print("Speed Set: "); debug_println(newSpeed);
    writeOledSpeed();
  }
}

void releaseAllLocos() {
  if (wiThrottleProtocol.getNumberOfLocomotives()>0) {
    for(int index=wiThrottleProtocol.getNumberOfLocomotives()-1;index>=0;index--) {
      wiThrottleProtocol.releaseLocomotive(wiThrottleProtocol.getLocomotiveAtPosition(index));
      writeOledSpeed();
    } 
  }
}

void toggleAdditionalMultiplier() {
  // if (speedStep != currentSpeedStep) {
  //   currentSpeedStep = speedStep;
  // } else {
  //   currentSpeedStep = speedStep * speedStepAdditionalMultiplier;
  // }
  switch (speedStepCurrentMultiplier) {
    case 1: 
      speedStepCurrentMultiplier = speedStepAdditionalMultiplier;
      break;
    case speedStepAdditionalMultiplier:
      speedStepCurrentMultiplier = speedStepAdditionalMultiplier*2;
      break;
    case speedStepAdditionalMultiplier*2:
      speedStepCurrentMultiplier = 1;
      break;
  }

  currentSpeedStep = speedStep * speedStepCurrentMultiplier;
  writeOledSpeed();
}

void toggleHeartbeatCheck() {
  heartbeatCheckEnabled = !heartbeatCheckEnabled;
  debug_print("Heartbeat Check: "); 
  if (heartbeatCheckEnabled) {
    debug_println("Enabled");
  } else {
    debug_println("Disabled");
  }
  writeHeartbeatCheck();
}

void toggleDirection() {
  if (wiThrottleProtocol.getNumberOfLocomotives()>0) {
  //   Direction direction = Reverse;
  //   if (currentDirection == Reverse) {
  //     direction = Forward;
  //   }
    changeDirection( (currentDirection == Forward) ? Reverse : Forward );
    writeOledSpeed();
  }
}

void changeDirection(Direction direction) {
  if (wiThrottleProtocol.getNumberOfLocomotives()>0) {
    wiThrottleProtocol.setDirection(direction);
    currentDirection = direction;
    debug_print("Change direction: "); debug_println( (direction==Forward) ? "Forward" : "Reverse");
    writeOledSpeed(); 
  }
}

void doDirectFunction(int functionNumber, boolean pressed) {
  if (wiThrottleProtocol.getNumberOfLocomotives()>0) {
    debug_print("direct fn: "); debug_print(functionNumber); debug_println( pressed ? " Pressed" : " Released");
    wiThrottleProtocol.setFunction(functionNumber, pressed);
    writeOledSpeed(); 
  }
}

void doFunction(int functionNumber, boolean pressed) {   // currently ignoring the pressed value
  if (wiThrottleProtocol.getNumberOfLocomotives()>0) {
    wiThrottleProtocol.setFunction(functionNumber, true);
    if (!functionStates[functionNumber]) {
      debug_print("fn: "); debug_print(functionNumber); debug_println(" Pressed");
      // functionStates[functionNumber] = true;
    } else {
      delay(20);
      wiThrottleProtocol.setFunction(functionNumber, false);
      debug_print("fn: "); debug_print(functionNumber); debug_println(" Released");
      // functionStates[functionNumber] = false;
    }
    writeOledSpeed(); 
  }
}

void powerOnOff(TrackPower powerState) {
  wiThrottleProtocol.setTrackPower(powerState);
  trackPower = powerState;
}

void powerToggle() {
  if (trackPower==PowerOn) {
    powerOnOff(PowerOff);
  } else {
    powerOnOff(PowerOn);
  }
}

void reconnect() {
  clearOledArray(); 
  oledText[0] = appName; oledText[6] = appVersion; 
  oledText[2] = msg_disconnected;
  writeOledArray(false, false);
  delay(5000);
  disconnectWitServer();
}

void setLastServerResponseTime(boolean force) {
  // debug_print("setLastServerResponseTime "); debug_println((force) ? "True": "False");
  lastServerResponseTime = wiThrottleProtocol.getLastServerResponseTime();
  if ( (lastServerResponseTime==0) || (force) ) lastServerResponseTime = millis() /1000;
  // debug_print("setLastServerResponseTime "); debug_println(lastServerResponseTime);
}

// *********************************************************************************
//  Select functions
// *********************************************************************************

void selectRoster(int selection) {
  debug_print("selectRoster() "); debug_println(selection);

  if ((selection>=0) && (selection < rosterSize)) {
    String loco = String(rosterLength[selection]) + rosterAddress[selection];
    debug_print("add Loco: "); debug_println(loco);
    wiThrottleProtocol.addLocomotive(loco);
    resetFunctionStates();
    writeOledSpeed();
    keypadUseType = KEYPAD_USE_OPERATION;
  }
}

void selectTurnoutList(int selection, TurnoutAction action) {
  debug_print("selectTurnoutList() "); debug_println(selection);

  if ((selection>=0) && (selection < turnoutListSize)) {
    String turnout = turnoutListSysName[selection];
    debug_print("Turnout Selected: "); debug_println(turnout);
    wiThrottleProtocol.setTurnout(turnout,action);
    writeOledSpeed();
    keypadUseType = KEYPAD_USE_OPERATION;
  }
}

void selectRouteList(int selection) {
  debug_print("selectRouteList() "); debug_println(selection);

  if ((selection>=0) && (selection < routeListSize)) {
    String route = routeListSysName[selection];
    debug_print("Route Selected: "); debug_println(route);
    wiThrottleProtocol.setRoute(route);
    writeOledSpeed();
    keypadUseType = KEYPAD_USE_OPERATION;
  }
}

// *********************************************************************************
//  oLED functions
// *********************************************************************************

void setAppnameForOled() {
  oledText[0] = appName; oledText[6] = appVersion; 
}

void writeOledFoundSSids(String soFar) {
  menuIsShowing = true;
  keypadUseType = KEYPAD_USE_SELECT_SSID_FROM_FOUND;
  if (soFar == "") { // nothing entered yet
    clearOledArray();
    for (int i=0; i<5 && i<foundSsidsCount; i++) {
      oledText[i] = String(i) + ": " + foundSsids[(page*5)+i];
    }
    oledText[5] = menu_select_ssids_from_found;
    writeOledArray(false, false);
  } else {
    int cmd = menuCommand.substring(0, 1).toInt();
  }
}

void writeOledRoster(String soFar) {
  menuIsShowing = true;
  keypadUseType = KEYPAD_USE_SELECT_ROSTER;
  if (soFar == "") { // nothing entered yet
    clearOledArray();
    for (int i=0; i<5 && i<rosterSize; i++) {
      if (rosterAddress[(page*5)+i] != 0) {
        oledText[i] = String(rosterIndex[i]) + ": " + rosterName[(page*5)+i] + " (" + rosterAddress[(page*5)+i] + ")" ;
      }
    }
    oledText[5] = menu_roster;
    writeOledArray(false, false);
  } else {
    int cmd = menuCommand.substring(0, 1).toInt();
  }
}

void writeOledTurnoutList(String soFar, TurnoutAction action) {
  menuIsShowing = true;
  if (action == TurnoutThrow) {
    keypadUseType = KEYPAD_USE_SELECT_TURNOUTS_THROW;
  } else {
    keypadUseType = KEYPAD_USE_SELECT_TURNOUTS_CLOSE;
  }
  if (soFar == "") { // nothing entered yet
    clearOledArray();
    int j = 0;
    for (int i=0; i<10 && i<turnoutListSize; i++) {
      j = (i<5) ? j=i : j = i+1;
      if (turnoutListUserName[(page*10)+i].length()>0) {
        oledText[j] = String(turnoutListIndex[i]) + ": " + turnoutListUserName[(page*10)+i].substring(0,10);
      }
    }
    oledText[5] = menu_turnout_list;
    writeOledArray(false, false);
  } else {
    int cmd = menuCommand.substring(0, 1).toInt();
  }
}

void writeOledRouteList(String soFar) {
  menuIsShowing = true;
  keypadUseType = KEYPAD_USE_SELECT_ROUTES;
  if (soFar == "") { // nothing entered yet
    clearOledArray();
    int j = 0;
    for (int i=0; i<10 && i<routeListSize; i++) {
      j = (i<5) ? j=i : j = i+1;
      if (routeListUserName[(page*10)+i].length()>0) {
        oledText[j] = String(routeListIndex[i]) + ": " + routeListUserName[(page*10)+i].substring(0,10);
      }
    }
    oledText[5] = menu_route_list;
    writeOledArray(false, false);
  } else {
    int cmd = menuCommand.substring(0, 1).toInt();
  }
}


void writeOledEnterPassword() {
  keypadUseType = KEYPAD_USE_ENTER_SSID_PASSWORD;
  encoderUseType = KEYPAD_USE_ENTER_SSID_PASSWORD;
  clearOledArray(); 
  String tempSsidPasswordEntered;
  tempSsidPasswordEntered = ssidPasswordEntered+ssidPasswordCurrentChar;
  if (tempSsidPasswordEntered.length()>12) {
    tempSsidPasswordEntered = "\253"+tempSsidPasswordEntered.substring(tempSsidPasswordEntered.length()-12);
  } else {
    tempSsidPasswordEntered = " "+tempSsidPasswordEntered;
  }
  oledText[0] = msg_enter_password;
  oledText[2] = tempSsidPasswordEntered;
  oledText[5] = menu_enter_ssid_password;
  writeOledArray(false, true);
}


void writeOledMenu(String soFar) {
  menuIsShowing = true;
  if (soFar == "") { // nothing entered yet
    clearOledArray();
    int j = 0;
    for (int i=1; i<10; i++) {
      j = (i<6) ? j=i : j = i+1;
      oledText[j-1] = String(i) + ": " + menuText[i][0];
    }
    oledText[10] = "0: " + menuText[0][0];
    oledText[5] = menu_cancel;
    writeOledArray(false, false);
  } else {
    int cmd = menuCommand.substring(0, 1).toInt();

    clearOledArray();

    oledText[0] = "Menu: " + menuText[cmd][0]; oledText[1] =  menuCommand.substring(1, menuCommand.length());
    oledText[5] = menuText[cmd][1];
    writeOledArray(false, false);
  }
}

void writeHeartbeatCheck() {
  menuIsShowing = false;
  clearOledArray();
  oledText[0] = menuText[10][0];
  if (heartbeatCheckEnabled) {
    oledText[1] = msg_heartbeatCheckEnabled; 
  } else {
    oledText[1] = msg_heartbeatCheckDisabled; 
  }
  oledText[5] = menuText[10][1];
  writeOledArray(false, false);
}

void writeOledSpeed() {
  menuIsShowing = false;
  String sLocos = "";
  String sSpeed = "";
  String sDirection = "";

  clearOledArray();
  
  boolean drawTopLine = false;

  if (wiThrottleProtocol.getNumberOfLocomotives() > 0 ) {
    // oledText[0] = label_locos; oledText[2] = label_speed;
  
    for (int i=0; i < wiThrottleProtocol.getNumberOfLocomotives(); i++) {
      sLocos = sLocos + " " + wiThrottleProtocol.getLocomotiveAtPosition(i);
    }
    sSpeed = String(currentSpeed);
    sDirection = (currentDirection==Forward) ? direction_forward : direction_reverse;

    oledText[0] = sLocos; oledText[7] = sDirection;

    drawTopLine = true;

  } else {
    setAppnameForOled();
    oledText[2] = msg_no_loco_selected;
  }

  if (speedStep != currentSpeedStep) {
    oledText[3] = "X " + String(speedStepCurrentMultiplier);
  }

  oledText[5] = menu_menu;
  writeOledArray(false, false, false, drawTopLine);

  if (wiThrottleProtocol.getNumberOfLocomotives() > 0 ) {
    writeOledFunctions();
  }

  if (trackPower == PowerOn) {
    u8g2.drawBox(0,41,15,8);
    u8g2.setDrawColor(1);
    u8g2.setDrawColor(0);
  }
  u8g2.setFont(u8g2_font_profont10_tf);
  u8g2.drawStr(0, 48, label_track_power.c_str());
  u8g2.setDrawColor(1);


  const char *cSpeed = sSpeed.c_str();
  u8g2.setFont(u8g2_font_inb21_mn); // big
  int width = u8g2.getStrWidth(cSpeed);
  u8g2.drawStr(25+(55-width),45, cSpeed);
  u8g2.sendBuffer();
}

void writeOledFunctions() {
   int x = 99;
   for (int i=0; i < 12; i++) {
     if (functionStates[i]) {
      //  debug_print("Fn On "); debug_println(i);
      int y = (i+2)*10-8;
      if ((i>=4) && (i<8)) { 
        x = 109; 
        y = (i-2)*10-8;
      } else if (i>=8) { 
        x = 119; 
        y = (i-6)*10-8;
      }
      
      u8g2.drawBox(x,y,8,8);
      u8g2.setDrawColor(0);
      u8g2.setFont(u8g2_font_profont10_tf);
      u8g2.drawStr( x+2, y+7, String( (i<10) ? i : i-10 ).c_str());
      u8g2.setDrawColor(1);
    //  } else {
    //    debug_print("Fn Off "); debug_println(i);
     }
   }
}

void writeOledArray(boolean isThreeColums, boolean isPassword) {
  writeOledArray(isThreeColums, isPassword, true, false);
}

void writeOledArray(boolean isThreeColums, boolean isPassword, boolean sendBuffer) {
  writeOledArray(isThreeColums, isPassword, sendBuffer, false);
}

void writeOledArray(boolean isThreeColums, boolean isPassword, boolean sendBuffer, boolean drawTopLine) {
  // debug_println("Start writeOledArray()");
  u8g2.clearBuffer();					// clear the internal memory

  // u8g2.setFont(u8g2_font_ncenB08_tr);	// serif bold
  // u8g2.setFont(u8g2_font_helvB08_te);	// helv bold
  // u8g2.setFont(u8g2_font_helvB08_tf);	// helv
  // u8g2.setFont(u8g2_font_likeminecraft_te); // bit wider
  u8g2.setFont(u8g2_font_NokiaSmallPlain_tf); // small
  
  int x=0;
  int y=10;
  int xInc = 64; 
  int max = 12;
  if (isThreeColums) {
    xInc = 42;
    max = 18;
  }

  for (int i=0; i < max; i++) {
    const char *cLine1 = oledText[i].c_str();
    if ((isPassword) && (i==2)) u8g2.setFont(u8g2_font_9x15_tf); 
    u8g2.drawStr(x,y, cLine1);
    if ((isPassword) && (i==2)) u8g2.setFont(u8g2_font_NokiaSmallPlain_tf); 
    y = y + 10;
    if ((i==5) || (i==11)) {
      x = x + xInc;
      y = 10;
    }
  }
  if (drawTopLine) u8g2.drawHLine(0,11,128);
  u8g2.drawHLine(0,51,128);

  if (sendBuffer) u8g2.sendBuffer();					// transfer internal memory to the display
  // debug_println("End writeOledArray()");
}

void clearOledArray() {
  for (int i=0; i < 15; i++) {
    oledText[i] = "";
  }
}

void writeOledDirectCommands() {
  clearOledArray();
  oledText[0] = direct_command_list;
  for (int i=0; i < 4; i++) {
    oledText[i+1] = directCommandText[i][0];
  }
  int j = 0;
  for (int i=6; i < 10; i++) {
    oledText[i+1] = directCommandText[j][1];
    j++;
  }
  j=0;
  for (int i=12; i < 16; i++) {
    oledText[i+1] = directCommandText[j][2];
    j++;
  }
  writeOledArray(true, false);
  menuCommandStarted = false;
}

// *********************************************************************************

void deepSleepStart() {
  clearOledArray(); 
  setAppnameForOled();
  oledText[2] = msg_start_sleep;
  writeOledArray(false, false);
  delay(2000);

  u8g2.setPowerSave(1);
  esp_deep_sleep_start();
}
