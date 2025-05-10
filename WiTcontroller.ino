/**
 * This app turns an ESP32 into a dedicated Model Railway Controller for use
 * with JMRI or any wiThrottle server.

  Instructions:
  - Refer to the readme at https://github.com/flash62au/WiTcontroller
 */

#include <string>

// Use the Arduino IDE 'Boards' Manager to get these libraries
// They will be installed with the 'ESP32' Board library
// DO NOT DOWNLOAD THEM DIRECTLY!!!
#include <WiFi.h>                 // https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi     GPL 2.1
#include <ESPmDNS.h>              // https://github.com/espressif/arduino-esp32/blob/master/libraries/ESPmDNS  GPL 2.1
#include <esp_wifi.h>             // https://git.liberatedsystems.co.uk/jacob.eva/arduino-esp32/src/branch/master/tools/sdk/esp32s2/include/esp_wif  GPL 2.0

// ----------------------

#include <Preferences.h>

// use the Arduino IDE 'Library' Manager to get these libraries
#include <Keypad.h>               // https://www.arduinolibraries.info/libraries/keypad                        GPL 3.0
#include <U8g2lib.h>              // https://github.com/olikraus/u8g2  (Just get "U8g2" via the Arduino IDE Library Manager)   new-bsd
#include <WiThrottleProtocol.h>   // https://github.com/flash62au/WiThrottleProtocol                           Creative Commons 4.0  Attribution-ShareAlike
#include <AiEsp32RotaryEncoder.h> // https://github.com/igorantolic/ai-esp32-rotary-encoder                    GPL 2.0

// this library is included with the WiTController code
#include "Pangodream_18650_CL.h"  // https://github.com/pangodream/18650CL                                     Copyright (c) 2019 Pangodream

// create these files by copying the example files and editing them as needed
#include "config_network.h"      // LAN networks (SSIDs and passwords)
#include "config_buttons.h"      // keypad buttons assignments

// DO NOT ALTER these files
#include "config_keypad_etc.h"
#include "static.h"
#include "actions.h"
#include "WiTcontroller.h"

#if WITCONTROLLER_DEBUG == 0
 #define debug_print(params...) Serial.print(params)
 #define debug_println(params...) Serial.print(params); Serial.print(" ("); Serial.print(millis()); Serial.println(")")
 #define debug_printf(params...) Serial.printf(params)
#else
 #define debug_print(...)
 #define debug_println(...)
 #define debug_printf(...)
#endif
int debugLevel = DEBUG_LEVEL;


// *********************************************************************************
// non-volatile storage

Preferences nvsPrefs;
bool nvsInit = false;
bool nvsPrefsSaved = false;
bool preferencesRead = false;

// *********************************************************************************

int keypadUseType = KEYPAD_USE_OPERATION;
int encoderUseType = ENCODER_USE_OPERATION;
int encoderButtonAction = ENCODER_BUTTON_ACTION;

bool menuCommandStarted = false;
String menuCommand = "";
bool menuIsShowing = false;

String startupCommands[4] = {STARTUP_COMMAND_1, STARTUP_COMMAND_2, STARTUP_COMMAND_3, STARTUP_COMMAND_4};

String oledText[18] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};
bool oledTextInvert[18] = {false, false, false, false, false, false, false, false, false, 
                           false, false, false, false, false, false, false, false, false};

int currentSpeed[6];   // set to maximum possible (6)
Direction currentDirection[6];   // set to maximum possible (6)
int speedStepCurrentMultiplier = 1;

TrackPower trackPower = PowerUnknown;
String turnoutPrefix = "";
String routePrefix = "";

// encoder variables
bool circleValues = true;
int encoderValue = 0;
int lastEncoderValue = 0;

// throttle pot values
bool useRotaryEncoderForThrottle = USE_ROTARY_ENCODER_FOR_THROTTLE;
int throttlePotPin = THROTTLE_POT_PIN;
bool throttlePotUseNotches = THROTTLE_POT_USE_NOTCHES;
int throttlePotNotchValues[] = THROTTLE_POT_NOTCH_VALUES; 
int throttlePotNotchSpeeds[] = THROTTLE_POT_NOTCH_SPEEDS;
int throttlePotNotch = 0;
int throttlePotTargetSpeed = 0;
int lastThrottlePotValue = 0;
int lastThrottlePotHighValue = 0;  // highest of the most recent
int lastThrottlePotValues[] = {0, 0, 0, 0, 0};
int lastThrottlePotReadTime = -1;

// battery test values
bool useBatteryTest = USE_BATTERY_TEST;
#if USE_BATTERY_TEST
  #if USE_BATTERY_PERCENT_AS_WELL_AS_ICON
    ShowBattery showBatteryTest = ICON_AND_PERCENT;
  #else 
    ShowBattery showBatteryTest = ICON_ONLY;
  #endif
#else
  ShowBattery showBatteryTest = NONE;
#endif
bool useBatteryPercentAsWellAsIcon = USE_BATTERY_PERCENT_AS_WELL_AS_ICON;
int lastBatteryTestValue = 100; 
int lastBatteryAnalogReadValue = 0;
double lastBatteryCheckTime = -10000;
#if USE_BATTERY_TEST
  Pangodream_18650_CL BL(BATTERY_TEST_PIN,BATTERY_CONVERSION_FACTOR);
#endif

// server variables
// bool ssidConnected = false;
String selectedSsid = "";
String selectedSsidPassword = "";
int ssidConnectionState = CONNECTION_STATE_DISCONNECTED;

// ssid password entry
String ssidPasswordEntered = "";
bool ssidPasswordChanged = true;
char ssidPasswordCurrentChar = ssidPasswordBlankChar; 

IPAddress selectedWitServerIP;
int selectedWitServerPort = 0;
String selectedWitServerName ="";
int noOfWitServices = 0;
int witConnectionState = CONNECTION_STATE_DISCONNECTED;
String serverType = "";

//found wiThrottle servers
IPAddress foundWitServersIPs[maxFoundWitServers];
int foundWitServersPorts[maxFoundWitServers];
String foundWitServersNames[maxFoundWitServers];
int foundWitServersCount = 0;
bool autoConnectToFirstDefinedServer = AUTO_CONNECT_TO_FIRST_DEFINED_SERVER;
bool autoConnectToFirstWiThrottleServer = AUTO_CONNECT_TO_FIRST_WITHROTTLE_SERVER;
int outboundCmdsMininumDelay = OUTBOUND_COMMANDS_MINIMUM_DELAY;
bool commandsNeedLeadingCrLf = SEND_LEADING_CR_LF_FOR_COMMANDS;

//found ssids
String foundSsids[maxFoundSsids];
long foundSsidRssis[maxFoundSsids];
bool foundSsidsOpen[maxFoundSsids];
int foundSsidsCount = 0;
int ssidSelectionSource;
double startWaitForSelection;

// wit Server ip entry
String witServerIpAndPortConstructed = "###.###.###.###:#####";
String witServerIpAndPortEntered = DEFAULT_IP_AND_PORT;
bool witServerIpAndPortChanged = true;

// roster variables
int rosterSize = 0;
int rosterIndex[maxRoster]; 
String rosterName[maxRoster]; 
int rosterAddress[maxRoster];
char rosterLength[maxRoster];
char rosterSortStrings[maxRoster][14]; 
char* rosterSortPointers[maxRoster]; 
int rosterSortedIndex[maxRoster]; 

int page = 0;
int functionPage = 0;
bool functionHasBeenSelected = false;

// Broadcast msessage
String broadcastMessageText = "";
long broadcastMessageTime = 0;
long lastReceivingServerDetailsTime = 0;

// remember oLED state
int lastOledScreen = 0;
String lastOledStringParameter = "";
int lastOledIntParameter = 0;
bool lastOledBoolParameter = false;
TurnoutAction lastOledTurnoutParameter = TurnoutToggle;

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
bool functionStates[6][MAX_FUNCTIONS];   // set to maximum possible (6 throttles)

// function labels
String functionLabels[6][MAX_FUNCTIONS];   // set to maximum possible (6 throttles)

// consist function follow
int functionFollow[6][MAX_FUNCTIONS];   // set to maximum possible (6 throttles)

// speedstep
int currentSpeedStep[6];   // set to maximum possible (6 throttles)

// throttle
int currentThrottleIndex = 0;
char currentThrottleIndexChar = '0';
int maxThrottles = MAX_THROTTLES;

int heartBeatPeriod = 10; // default to 10 seconds
long lastServerResponseTime;  // seconds since start of Arduino
bool heartbeatCheckEnabled = HEARTBEAT_ENABLED;

// used to stop speed bounces
long lastSpeedSentTime = 0;
int lastSpeedSent = 0;
// int lastDirectionSent = -1;
int lastSpeedThrottleIndex = 0;

bool dropBeforeAcquire = DROP_BEFORE_ACQUIRE;

// don't alter the assignments here
// alter them in config_buttons.h

const char* deviceName = DEVICE_NAME;

static unsigned long rotaryEncoderButtonLastTimePressed = 0;
const int rotaryEncoderButtonEncoderDebounceTime = ROTARY_ENCODER_DEBOUNCE_TIME;   // in miliseconds

const bool encoderRotationClockwiseIsIncreaseSpeed = ENCODER_ROTATION_CLOCKWISE_IS_INCREASE_SPEED;
// false = Counterclockwise  true = clockwise

const bool toggleDirectionOnEncoderButtonPressWhenStationary = TOGGLE_DIRECTION_ON_ENCODER_BUTTON_PRESSED_WHEN_STATIONAY;
// true = if the locos(s) are stationary, clicking the encoder button will toggle the direction

//4x3 keypad only uses 0-9
//4x4 uses all 14 
int buttonActions[14] = { CHOSEN_KEYPAD_0_FUNCTION,
                          CHOSEN_KEYPAD_1_FUNCTION,
                          CHOSEN_KEYPAD_2_FUNCTION,
                          CHOSEN_KEYPAD_3_FUNCTION,
                          CHOSEN_KEYPAD_4_FUNCTION,
                          CHOSEN_KEYPAD_5_FUNCTION,
                          CHOSEN_KEYPAD_6_FUNCTION,
                          CHOSEN_KEYPAD_7_FUNCTION,
                          CHOSEN_KEYPAD_8_FUNCTION,
                          CHOSEN_KEYPAD_9_FUNCTION,
                          CHOSEN_KEYPAD_A_FUNCTION,
                          CHOSEN_KEYPAD_B_FUNCTION,
                          CHOSEN_KEYPAD_C_FUNCTION,
                          CHOSEN_KEYPAD_D_FUNCTION
};

// text that will appear when you press #
const String directCommandText[4][3] = {
    {CHOSEN_KEYPAD_1_DISPLAY_NAME, CHOSEN_KEYPAD_2_DISPLAY_NAME, CHOSEN_KEYPAD_3_DISPLAY_NAME},
    {CHOSEN_KEYPAD_4_DISPLAY_NAME, CHOSEN_KEYPAD_5_DISPLAY_NAME, CHOSEN_KEYPAD_6_DISPLAY_NAME},
    {CHOSEN_KEYPAD_7_DISPLAY_NAME, CHOSEN_KEYPAD_8_DISPLAY_NAME, CHOSEN_KEYPAD_9_DISPLAY_NAME},
    {CHOSEN_KEYPAD_DISPLAY_PREFIX, CHOSEN_KEYPAD_0_DISPLAY_NAME, CHOSEN_KEYPAD_DISPLAY_SUFIX}
};

bool oledDirectCommandsAreBeingDisplayed = false;
#ifdef HASH_SHOWS_FUNCTIONS_INSTEAD_OF_KEY_DEFS
  bool hashShowsFunctionsInsteadOfKeyDefs = HASH_SHOWS_FUNCTIONS_INSTEAD_OF_KEY_DEFS;
#else
  bool hashShowsFunctionsInsteadOfKeyDefs = false;
#endif

//optional additional buttons
#if !USE_NEW_ADDITIONAL_BUTTONS_FORMAT
  int maxAdditionalButtons = MAX_ADDITIONAL_BUTTONS;
  int additionalButtonPin[MAX_ADDITIONAL_BUTTONS] =          ADDITIONAL_BUTTONS_PINS;
  int additionalButtonType[MAX_ADDITIONAL_BUTTONS] =         ADDITIONAL_BUTTONS_TYPE;
  int additionalButtonActions[MAX_ADDITIONAL_BUTTONS] = {
                            CHOSEN_ADDITIONAL_BUTTON_0_FUNCTION,
                            CHOSEN_ADDITIONAL_BUTTON_1_FUNCTION,
                            CHOSEN_ADDITIONAL_BUTTON_2_FUNCTION,
                            CHOSEN_ADDITIONAL_BUTTON_3_FUNCTION,
                            CHOSEN_ADDITIONAL_BUTTON_4_FUNCTION,
                            CHOSEN_ADDITIONAL_BUTTON_5_FUNCTION,
                            CHOSEN_ADDITIONAL_BUTTON_6_FUNCTION
  };
  int additionalButtonLatching[MAX_ADDITIONAL_BUTTONS] = {
                            ADDITIONAL_BUTTON_0_LATCHING,
                            ADDITIONAL_BUTTON_1_LATCHING,
                            ADDITIONAL_BUTTON_2_LATCHING,
                            ADDITIONAL_BUTTON_3_LATCHING,
                            ADDITIONAL_BUTTON_4_LATCHING,
                            ADDITIONAL_BUTTON_5_LATCHING,
                            ADDITIONAL_BUTTON_6_LATCHING
  };

  unsigned long lastAdditionalButtonDebounceTime[MAX_ADDITIONAL_BUTTONS];  // the last time the output pin was toggled
  bool additionalButtonRead[MAX_ADDITIONAL_BUTTONS];
  bool additionalButtonLastRead[MAX_ADDITIONAL_BUTTONS];

#else
  #if NEW_MAX_ADDITIONAL_BUTTONS>0
    int maxAdditionalButtons = NEW_MAX_ADDITIONAL_BUTTONS;
    int additionalButtonPin[NEW_MAX_ADDITIONAL_BUTTONS] = NEW_ADDITIONAL_BUTTON_PIN;
    int additionalButtonType[NEW_MAX_ADDITIONAL_BUTTONS] = NEW_ADDITIONAL_BUTTON_TYPE;
    int additionalButtonActions[NEW_MAX_ADDITIONAL_BUTTONS] = NEW_ADDITIONAL_BUTTON_ACTIONS;
    int additionalButtonLatching[NEW_MAX_ADDITIONAL_BUTTONS] = NEW_ADDITIONAL_BUTTON_LATCHING;

    unsigned long lastAdditionalButtonDebounceTime[NEW_MAX_ADDITIONAL_BUTTONS];  // the last time the output pin was toggled
    bool additionalButtonRead[NEW_MAX_ADDITIONAL_BUTTONS];
    bool additionalButtonLastRead[NEW_MAX_ADDITIONAL_BUTTONS];
  #else
    int maxAdditionalButtons = 0;
    int additionalButtonPin[1] = {-1};
    int additionalButtonType[1] = {INPUT_PULLUP};
    int additionalButtonActions[1] = {FUNCTION_NULL};
    int additionalButtonLatching[1] = {false};

    unsigned long lastAdditionalButtonDebounceTime[1];  // the last time the output pin was toggled
    bool additionalButtonRead[1];
    bool additionalButtonLastRead[1];
  #endif
#endif

bool additionalButtonOverrideDefaultLatching = ADDITIONAL_BUTTON_OVERRIDE_DEFAULT_LATCHING;
unsigned long additionalButtonDebounceDelay = ADDITIONAL_BUTTON_DEBOUNCE_DELAY;    // the debounce time

// *********************************************************************************

void displayUpdateFromWit(int multiThrottleIndex) {
  debug_print("displayUpdateFromWit(): keyapdeUseType "); debug_print(keypadUseType); 
  debug_print(" menuIsShowing "); debug_print(menuIsShowing);
  debug_print(" multiThrottleIndex "); debug_print(multiThrottleIndex);
  debug_println("");
  if ( (keypadUseType == KEYPAD_USE_OPERATION) && (!menuIsShowing) 
  && (multiThrottleIndex==currentThrottleIndex) ) {
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
      debug_printf("Received Version: %s\n",version.c_str()); 
    }
    void receivedServerDescription(String description) {
      debug_print("Received Description: "); debug_println(description);
      serverType = description.substring(0,description.indexOf(" "));
      debug_print("ServerType: "); debug_println(serverType);
      if (serverType.equals("DCC-EX")) {
      // if (description.substring(0,6).equals("DCC-EX")) {
        debug_println("resetting prefixes");
        turnoutPrefix = DCC_EX_TURNOUT_PREFIX;
        routePrefix = DCC_EX_ROUTE_PREFIX;
      }
    }
    void receivedMessage(String message) {
      debug_print("Broadcast Message: ");
      debug_println(message);
      if ( (!message.equals("Connected")) && (!message.equals("Connecting..")) ) {
        broadcastMessageText = String(message);
        broadcastMessageTime = millis();
        refreshOled();
      }
    }
    void receivedAlert(String message) {
      debug_print("Broadcast Alert: ");
      debug_println(message);
      if ( (!message.equals("Connected")) 
          && (!message.equals("Connecting.."))
          && (!message.equals("Steal from other WiThrottle or JMRI throttle Required"))
        ) {
        broadcastMessageText = String(message);
        broadcastMessageTime = millis();
        refreshOled();
      }
    }
    void receivedSpeedMultiThrottle(char multiThrottle, int speed) {             // Vnnn
      debug_print("Received Speed: ("); debug_print(millis()); debug_print(") throttle: "); debug_print(multiThrottle);  debug_print(" speed: "); debug_println(speed); 
      int multiThrottleIndex = getMultiThrottleIndex(multiThrottle);

      if (currentSpeed[multiThrottleIndex] != speed) {
        
        // check for bounce. (intermediate speed sent back from the server, but is not up to date with the throttle)
        if ( (lastSpeedThrottleIndex!=multiThrottleIndex)
             || ((millis()-lastSpeedSentTime)>500)
        ) {
          currentSpeed[multiThrottleIndex] = speed;
          displayUpdateFromWit(multiThrottleIndex);
        } else {
          debug_print("Received Speed: skipping response: ("); debug_print(millis()); debug_print(") speed: "); debug_println(speed);
        }
      }
    }
    void receivedDirectionMultiThrottle(char multiThrottle, Direction dir) {     // R{0,1}
      debug_print("Received Direction: "); debug_println(dir); 
      int multiThrottleIndex = getMultiThrottleIndex(multiThrottle);

      if (currentDirection[multiThrottleIndex] != dir) {
        currentDirection[multiThrottleIndex] = dir;
        displayUpdateFromWit(multiThrottleIndex);
      }
    }
    void receivedDirectionMultiThrottle(char multiThrottle, String loco, Direction dir) {     // R{0,1}
      debug_print("Received Direction: loco: "); debug_print(loco); debug_print(" Received Direction: "); debug_println(dir); 
      // int multiThrottleIndex = getMultiThrottleIndex(multiThrottle);

      // if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar) > 0) {
      //   for (int index=0; index < wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar); index++) {  //can only show first 8
      //     if (loco.equals(wiThrottleProtocol.getLocomotiveAtPosition(currentThrottleIndexChar, index));
      //       if (wiThrottleProtocol.getDirection(currentThrottleIndexChar, loco) == Reverse) {
      //         oledTextInvert[j+1] = true;
      //       }
      //   } 
      // }
    }
    void receivedFunctionStateMultiThrottle(char multiThrottle, uint8_t func, bool state) { 
      debug_print("Received Fn: "); debug_print(func); debug_print(" State: "); debug_println( (state) ? "True" : "False" );
      int multiThrottleIndex = getMultiThrottleIndex(multiThrottle);

      if (functionStates[multiThrottleIndex][func] != state) {
        functionStates[multiThrottleIndex][func] = state;
        displayUpdateFromWit(multiThrottleIndex);
      }
    }
    void receivedRosterFunctionListMultiThrottle(char multiThrottle, String functions[MAX_FUNCTIONS]) { 
      debug_println("Received Fn List: "); 
      int multiThrottleIndex = getMultiThrottleIndex(multiThrottle);

      for(int i = 0; i < MAX_FUNCTIONS; i++) {
        functionLabels[multiThrottleIndex][i] = functions[i];
        debug_print(" Function: "); debug_print(i); debug_print(" - "); debug_println( functions[i] );
      }
    }
    void receivedTrackPower(TrackPower state) { 
      debug_print("Received TrackPower: "); debug_println(state);
      if (trackPower != state) {
        trackPower = state;
        displayUpdateFromWit(-1); // dummry multithrottle
        refreshOled();
      }
    }
    void receivedRosterEntries(int size) {
      debug_print("Received Roster Entries. Size: "); debug_println(size);
      rosterSize = (size<maxRoster) ? size : maxRoster;

      if (rosterSize==0) {
        setupPreferences(false);  // if not roster read the prefeences immediately otherwise wait till we get them all
      }
    }
    void receivedRosterEntry(int index, String name, int address, char length) {
      debug_print("Received Roster Entry, index: "); debug_print(index); debug_println(" - " + name);
      if (index < rosterSize) {
        rosterIndex[index] = index; 
        rosterSortedIndex[index] = index; // default to unsorted
        rosterName[index] = name; 
        rosterAddress[index] = address;
        rosterLength[index] = length;

        if (ROSTER_SORT_SEQUENCE == 1) {
          strncpy(rosterSortStrings[index], ((name+"          ").substring(0,10) + ":" + (index < 10 ? "0" : "") + String(index)).c_str(), 13);
          rosterSortPointers[index] = rosterSortStrings[index];
        } else if (ROSTER_SORT_SEQUENCE == 2) { 
          char buf[11];
          sprintf(buf, "%10d", rosterAddress[index]);
          strncpy(rosterSortStrings[index], (String(buf) + ":" + (index < 10 ? "0" : "") + String(index)).c_str(), 13);
          rosterSortPointers[index] = rosterSortStrings[index];
        } 

        if ( (index==(rosterSize-1)) && (ROSTER_SORT_SEQUENCE>0)) { // got them all now.  and we need to sort them
          qsort(rosterSortPointers, rosterSize, sizeof rosterSortPointers[0], compareStrings);
          for (int i=0; i<rosterSize; i++) {
            rosterSortedIndex[i] = (rosterSortPointers[i][11]-'0')*10 + (rosterSortPointers[i][12]-'0');
            debug_print("Roster sorted: "); debug_print(rosterSortPointers[i]); debug_print(" | "); debug_println(rosterName[rosterSortedIndex[i]]);
          }

          setupPreferences(false);  // if there is a roster, we will have waited 
        }
      }
      receivingServerInfoOled(index, rosterSize);

      #if ACQUIRE_ROSTER_ENTRY_IF_ONLY_ONE
        if ( (rosterSize == 1) && (index == 0) ) {
          doOneStartupCommand("*1#0");
        }
      #endif

    }
    void receivedTurnoutEntries(int size) {
      debug_print("Received Turnout Entries. Size: "); debug_println(size);
      turnoutListSize = (size<maxTurnoutList) ? size : maxTurnoutList;
    }
    void receivedTurnoutEntry(int index, String sysName, String userName, int state) {
      if (index < maxTurnoutList) {
        turnoutListIndex[index] = index; 
        turnoutListSysName[index] = sysName; 
        turnoutListUserName[index] = userName;
        turnoutListState[index] = state;
      }
      receivingServerInfoOled(index, turnoutListSize);
    }

    void receivedRouteEntries(int size) {
      debug_print("Received Route Entries. Size: "); debug_println(size);
      routeListSize = (size<maxRouteList) ? size : maxRouteList;
    }
    void receivedRouteEntry(int index, String sysName, String userName, int state) {
      if (index < maxRouteList) {
        routeListIndex[index] = index; 
        routeListSysName[index] = sysName; 
        routeListUserName[index] = userName;
        routeListState[index] = state;
      }
      receivingServerInfoOled(index, routeListSize);
    }

    void addressStealNeeded(String address, String entry) { // MTSaddr<;>addr
      // char multiThrottleIndexChar;
      // for(int i=0;i<MAX_THROTTLES;i++) {
      //   multiThrottleIndexChar = getMultiThrottleChar(i);
      //   for(int j=0;j<wiThrottleProtocol.getNumberOfLocomotives(multiThrottleIndexChar);j++) {
      //     if (wiThrottleProtocol.getLocomotiveAtPosition(multiThrottleIndexChar, j).equals(address)) {
      //       wiThrottleProtocol.stealLocomotive(multiThrottleIndexChar, address);
      //       return;
      //     }
      //   }
      // }
      stealLoco(currentThrottleIndex, address);      
    }

    void addressStealNeededMultiThrottle(char multiThrottle, String address, String entry) {      
      // wiThrottleProtocol.stealLocomotive(multiThrottle, address);
      stealLoco(multiThrottle, address);
    }
    void receivedUnknownCommand(String unknownCommand) {
      debug_print("Received unknown command: "); debug_println(unknownCommand);
    }
};

int getMultiThrottleIndex(char multiThrottle) {
    int mThrottle = multiThrottle - '0';
    if ((mThrottle >= 0) && (mThrottle<=5)) {
        return mThrottle;
    } else {
        return 0;
    }
}

char getMultiThrottleChar(int multiThrottleIndex) {
  return '0' + multiThrottleIndex;
}

WiFiClient client;
WiThrottleProtocol wiThrottleProtocol;
MyDelegate myDelegate;
int deviceId = random(1000,9999);

// *********************************************************************************
// wifi / SSID 
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
  oledText[2] = MSG_BROWSING_FOR_SSIDS;
  writeOledBattery();
  writeOledArray(false, false, true, true);

  WiFi.setScanMethod(WIFI_ALL_CHANNEL_SCAN);
  WiFi.setSortMethod(WIFI_CONNECT_AP_BY_SIGNAL);
  
  int numSsids = WiFi.scanNetworks();
  while ( (numSsids == -1)
    && ((nowTime-startTime) <= 10000) ) { // try for 10 seconds
    delay(250);
    debug_print(".");
    nowTime = millis();
  }

  startWaitForSelection = millis();

  foundSsidsCount = 0;
  if (numSsids == -1) {
    debug_println("Couldn't get a wifi connection");

  } else {
    for (int thisSsid = 0; thisSsid < numSsids; thisSsid++) {
      /// remove duplicates (repeaters and mesh networks)
      bool found = false;
      for (int i=0; i<foundSsidsCount && i<maxFoundSsids; i++) {
        if (WiFi.SSID(thisSsid) == foundSsids[i]) {
          found = true;
          break;
        }
      }
      if (!found) {
        foundSsids[foundSsidsCount] = WiFi.SSID(thisSsid);
        foundSsidRssis[foundSsidsCount] = WiFi.RSSI(thisSsid);
        foundSsidsOpen[foundSsidsCount] = (WiFi.encryptionType(thisSsid) == 7) ? true : false;
        foundSsidsCount++;
      }
    }
    for (int i=0; i<foundSsidsCount; i++) {
      debug_println(foundSsids[i]);      
    }

    clearOledArray(); oledText[10] = MSG_SSIDS_FOUND;

    writeOledFoundSSids("");

    // oledText[5] = menu_select_ssids_from_found;
    setMenuTextForOled(menu_select_ssids_from_found);
    writeOledArray(false, false);

    keypadUseType = KEYPAD_USE_SELECT_SSID_FROM_FOUND;
    ssidConnectionState = CONNECTION_STATE_SELECTION_REQUIRED;

    if ((foundSsidsCount>0) && (autoConnectToFirstDefinedServer)) {
      for (int i=0; i<foundSsidsCount; i++) { 
        if (foundSsids[i] == ssids[0]) {
          ssidConnectionState = CONNECTION_STATE_SELECTED;
          selectedSsid = foundSsids[i];
          getSsidPasswordAndWitIpForFound();
        }
      }
    }
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
  bool found = false;

  selectedSsidPassword = "";
  turnoutPrefix = "";
  routePrefix = "";

  for (int i = 0; i < maxSsids; ++i) {
    if (selectedSsid == ssids[i]) {
      selectedSsidPassword = passwords[i];
      turnoutPrefix = turnoutPrefixes[i];
      routePrefix = routePrefixes[i];
      found = true;
      debug_println("getSsidPasswordAndWitIpForFound() Using configured password");
      break;
    }
  }

  if (!found) {
    if ( (selectedSsid.substring(0,6) == "DCCEX_") && (selectedSsid.length()==12) ) {
      selectedSsidPassword = "PASS_" + selectedSsid.substring(6);
      witServerIpAndPortEntered = "19216800400102560";
      turnoutPrefix = DCC_EX_TURNOUT_PREFIX;
      routePrefix = DCC_EX_ROUTE_PREFIX;
      debug_println("getSsidPasswordAndWitIpForFound() Using guessed DCC-EX password");
    } 
  }
}

void enterSsidPassword() {
  keypadUseType = KEYPAD_USE_ENTER_SSID_PASSWORD;
  encoderUseType = ENCODER_USE_SSID_PASSWORD;
  if (ssidPasswordChanged) { // don't refresh the screen if nothing nothing has changed
    debug_println("enterSsidPassword()");
    writeOledEnterPassword();
    ssidPasswordChanged = false;
  }
}
void showListOfSsids() {  // show the list from the specified values in config_network.h
  debug_println("showListOfSsids()");
  startWaitForSelection = millis();

  clearOledArray(); 
  setAppnameForOled(); 
  writeOledBattery();
  writeOledArray(false, false);

  if (maxSsids == 0) {
    oledText[1] = MSG_NO_SSIDS_FOUND;
    writeOledBattery();
    writeOledArray(false, false, true, true);
    debug_println(oledText[1]);
  
  } else {
    debug_print(maxSsids);  debug_println(MSG_SSIDS_LISTED);
    clearOledArray(); oledText[10] = MSG_SSIDS_LISTED;

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
      // oledText[5] = menu_select_ssids;
      setMenuTextForOled(menu_select_ssids);
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
  writeOledBattery();
  writeOledArray(false, false, true, true);

  double startTime = millis();
  double nowTime = startTime;

  const char *cSsid = selectedSsid.c_str();
  const char *cPassword = selectedSsidPassword.c_str();

  if (selectedSsid.length()>0) {
    debug_print("Trying Network "); debug_println(cSsid);
    clearOledArray(); 
    setAppnameForOled(); 
    for (int i = 0; i < 3; ++i) {  // Try three times
      oledText[1] = selectedSsid; oledText[2] =  String(MSG_TRYING_TO_CONNECT) + " (" + String(i) + ")";
      writeOledBattery();
      writeOledArray(false, false, true, true);

      nowTime = startTime;
      debug_print("hostname ");debug_println(WiFi.getHostname());
      WiFi.begin(cSsid, cPassword); 

      int j = 0;
      int tempTimer = millis();
      debug_print("Trying Network ... Checking status "); debug_print(cSsid); debug_print(" :"); debug_print(cPassword); debug_println(":");
      while ( (WiFi.status() != WL_CONNECTED) 
            && ((nowTime-startTime) <= SSID_CONNECTION_TIMEOUT) ) { // wait for X seconds to see if the connection worked
        if (millis() > tempTimer + 250) {
          oledText[3] = getDots(j);
          writeOledBattery();
          writeOledArray(false, false, true, true);
          j++;
          debug_print(".");
          tempTimer = millis();
        }
        nowTime = millis();
      }

      if (WiFi.status() == WL_CONNECTED) {
        if (!commandsNeedLeadingCrLf) { debug_println("Leading CRLF will not be sent for commands"); }
        break; 
      } else { // if not loop back and try again
        debug_println("");
      }
    }

    debug_println("");
    if (WiFi.status() == WL_CONNECTED) {
      debug_print("Connected. IP address: "); debug_println(WiFi.localIP());
      oledText[2] = MSG_CONNECTED; 
      oledText[3] = MSG_ADDRESS_LABEL + String(WiFi.localIP());
      writeOledBattery();
      writeOledArray(false, false, true, true);
      // ssidConnected = true;
      ssidConnectionState = CONNECTION_STATE_CONNECTED;
      keypadUseType = KEYPAD_USE_SELECT_WITHROTTLE_SERVER;

      // setup the bonjour listener
      if (!MDNS.begin("WiTcontroller")) {
        debug_println("Error setting up MDNS responder!");
        oledText[2] = MSG_BOUNJOUR_SETUP_FAILED;
        writeOledBattery();
        writeOledArray(false, false, true, true);
        delay(2000);
        ssidConnectionState = CONNECTION_STATE_DISCONNECTED;
      } else {
        debug_println("MDNS responder started");
      }

    } else {
      debug_println(MSG_CONNECTION_FAILED);
      oledText[2] = MSG_CONNECTION_FAILED;
      writeOledBattery();
      writeOledArray(false, false, true, true);
      delay(2000);
      
      WiFi.disconnect();      
      ssidConnectionState = CONNECTION_STATE_DISCONNECTED;
      ssidSelectionSource = SSID_CONNECTION_SOURCE_LIST;
    }
  }
}

// *********************************************************************************
// WiThrottle 
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

  debug_printf("Browsing for service _%s._%s.local. on %s ... ", service, proto, selectedSsid.c_str());
  clearOledArray(); 
  oledText[0] = appName; oledText[6] = appVersion; 
  oledText[1] = selectedSsid;   oledText[2] = MSG_BROWSING_FOR_SERVICE;
  writeOledBattery();
  writeOledArray(false, false, true, true);
  
  startWaitForSelection = millis();

  noOfWitServices = 0;
  if ( (selectedSsid.substring(0,6) == "DCCEX_") && (selectedSsid.length()==12) ) {
    debug_println(MSG_BYPASS_WIT_SERVER_SEARCH);
    oledText[1] = MSG_BYPASS_WIT_SERVER_SEARCH;
    writeOledBattery();
    writeOledArray(false, false, true, true);
    delay(500);
  } else {
    int j = 0;
    while ( (noOfWitServices == 0) 
    && ((nowTime-startTime) <= 10000)) { // try for 10 seconds 
      noOfWitServices = MDNS.queryService(service, proto);
      oledText[3] = getDots(j);
      writeOledBattery();
      writeOledArray(false, false, true, true);
      j++;
      debug_print(".");
      nowTime = millis();
    }
    debug_println("");
  }
  

  foundWitServersCount = noOfWitServices;
  if (noOfWitServices > 0) {
    for (int i = 0; ((i < noOfWitServices) && (i<maxFoundWitServers)); ++i) {
      foundWitServersNames[i] = MDNS.hostname(i);
      // foundWitServersIPs[i] = MDNS.IP(i);
      foundWitServersIPs[i] = ESPMDNS_IP_ATTRIBUTE_NAME;
      foundWitServersPorts[i] = MDNS.port(i);
      // debug_print("txt 0: key: "); debug_print(MDNS.txtKey(i,0)); debug_print(" value: '"); debug_print(MDNS.txt(i,0)); debug_println("'");
      // debug_print("txt 1: key: "); debug_print(MDNS.txtKey(i,1)); debug_print(" value: '"); debug_print(MDNS.txt(i,1)); debug_println("'");
      // debug_print("txt 2: key: "); debug_print(MDNS.txtKey(i,2)); debug_print(" value: '"); debug_print(MDNS.txt(i,2)); debug_println("'");
      // debug_print("txt 3: key: "); debug_print(MDNS.txtKey(i,3)); debug_print(" value: '"); debug_println(MDNS.txt(i,3)); debug_println("'");
      if (MDNS.hasTxt(i,"jmri")) {
        String node = MDNS.txt(i,"node");
        node.toLowerCase();
        if (foundWitServersNames[i].equals(node)) {
          foundWitServersNames[i] = "JMRI  (v" + MDNS.txt(i,"jmri") + ")";
        }
      }
    }
  }
  if ( (selectedSsid.substring(0,6) == "DCCEX_") && (selectedSsid.length()==12) ) {
    foundWitServersIPs[foundWitServersCount].fromString("192.168.4.1");
    foundWitServersPorts[foundWitServersCount] = 2560;
    foundWitServersNames[foundWitServersCount] = MSG_GUESSED_EX_CS_WIT_SERVER;
    foundWitServersCount++;
  }

  if (foundWitServersCount == 0) {
    oledText[1] = MSG_NO_SERVICES_FOUND;
    writeOledBattery();
    writeOledArray(false, false, true, true);
    debug_println(oledText[1]);
    delay(1000);
    buildWitEntry();
    witConnectionState = CONNECTION_STATE_ENTRY_REQUIRED;
  
  } else {
    debug_print(noOfWitServices);  debug_println(MSG_SERVICES_FOUND);
    clearOledArray(); oledText[3] = MSG_SERVICES_FOUND;

    for (int i = 0; i < foundWitServersCount; ++i) {
      // Print details for each service found
      debug_print("  "); debug_print(i); debug_print(": '"); debug_print(foundWitServersNames[i]);
      debug_print("' ("); debug_print(foundWitServersIPs[i]); debug_print(":"); debug_print(foundWitServersPorts[i]); debug_println(")");
      if (i<5) {  // only have room for 5
        String truncatedIp = ".." + foundWitServersIPs[i].toString().substring(foundWitServersIPs[i].toString().lastIndexOf("."));
        oledText[i] = String(i) + ": " + truncatedIp + ":" + String(foundWitServersPorts[i]) + " " + foundWitServersNames[i];
      }
    }

    if (foundWitServersCount > 0) {
      // oledText[5] = menu_select_wit_service;
      setMenuTextForOled(menu_select_wit_service);
    }
    writeOledArray(false, false);

    if ( (foundWitServersCount == 1) && (autoConnectToFirstWiThrottleServer) ) {
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
#if WITHROTTLE_PROTOCOL_DEBUG == 0
  wiThrottleProtocol.setLogStream(&Serial);
  wiThrottleProtocol.setLogLevel(DEBUG_LEVEL);
#endif

  debug_println("Connecting to the server...");
  clearOledArray(); 
  setAppnameForOled(); 
  oledText[1] = "        " + selectedWitServerIP.toString() + " : " + String(selectedWitServerPort); 
  oledText[2] = "        " + selectedWitServerName; oledText[3] + MSG_CONNECTING;
  writeOledBattery();
  writeOledArray(false, false, true, true);
  
  startWaitForSelection = millis();

  if (!client.connect(selectedWitServerIP, selectedWitServerPort)) {
    debug_println(MSG_CONNECTION_FAILED);
    oledText[3] = MSG_CONNECTION_FAILED;
    writeOledArray(false, false, true, true);
    delay(5000);
    
    witConnectionState = CONNECTION_STATE_DISCONNECTED;
    ssidConnectionState = CONNECTION_STATE_DISCONNECTED;
    ssidSelectionSource = SSID_CONNECTION_SOURCE_LIST;
    witServerIpAndPortChanged = true;

  } else {
    debug_print("Connected to server: ");   debug_println(selectedWitServerIP); debug_println(selectedWitServerPort);

    // Pass the communication to WiThrottle. + Set the mimimum period between sent commands
    wiThrottleProtocol.connect(&client, outboundCmdsMininumDelay);
    debug_println("WiThrottle connected");

    wiThrottleProtocol.setDeviceName(deviceName);  
    wiThrottleProtocol.setDeviceID(String(deviceId));  
    wiThrottleProtocol.setCommandsNeedLeadingCrLf(commandsNeedLeadingCrLf);
    if (HEARTBEAT_ENABLED) {
      wiThrottleProtocol.requireHeartbeat(true);
    }

    witConnectionState = CONNECTION_STATE_CONNECTED;
    setLastServerResponseTime(true);

    oledText[3] = MSG_CONNECTED;
    if (!hashShowsFunctionsInsteadOfKeyDefs) {
      // oledText[5] = menu_menu;
      setMenuTextForOled(menu_menu);
    } else {
      // oledText[5] = menu_menu_hash_is_functions;
      setMenuTextForOled(menu_menu_hash_is_functions);
    }
    writeOledArray(false, false, true, true);
    writeOledBattery();
    u8g2.sendBuffer();

    keypadUseType = KEYPAD_USE_OPERATION;

    doStartupCommands();
  }
}

void enterWitServer() {
  keypadUseType = KEYPAD_USE_ENTER_WITHROTTLE_SERVER;
  if (witServerIpAndPortChanged) { // don't refresh the screen if nothing nothing has changed
    debug_println("enterWitServer()");
    clearOledArray(); 
    setAppnameForOled(); 
    oledText[1] = MSG_NO_SERVICES_FOUND_ENTRY_REQUIRED;
    oledText[3] = witServerIpAndPortConstructed;
    // oledText[5] = menu_select_wit_entry;
      setMenuTextForOled(menu_select_wit_entry);
    writeOledArray(false, false, true, true);
    witServerIpAndPortChanged = false;
  }
}

void disconnectWitServer() {
  debug_println("disconnectWitServer()");
  for (int i=0; i<maxThrottles; i++) {
    releaseAllLocos(i);
  }
  wiThrottleProtocol.disconnect();
  debug_println("Disconnected from wiThrottle server\n");
  clearOledArray(); oledText[0] = MSG_DISCONNECTED;
  writeOledArray(false, false, true, true);
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
//   Non-Volitile storage functions

void setupPreferences(bool forceClear) {
  if (preferencesRead) return;
  debug_println("setupPreferences():");

  nvsPrefs.begin("WitController", true);
  nvsInit = nvsPrefs.isKey("nvsInit");
  if ( (nvsInit == false) || (forceClear) ) {
    debug_println("setupPreferences(): Initialising non-volitile storage ");

    nvsPrefs.end();

    nvsPrefs.begin("WitController", false); // write mode
    nvsPrefs.putBool("nvsInit", true);
    nvsInit = true;
    nvsPrefs.end();

  } else {
    nvsInit = true;
    debug_println("setupPreferences(): Non-volitile storage already initialised");
    readPreferences();
  }
}

void readPreferences() {
  if (preferencesRead) return;
  debug_println("readPreferences()");

  if (!RESTORE_ACQUIRED_LOCOS) return;

  debug_println("readPreferences(): Reading preferences from non-volitile storage ");
  nvsPrefs.begin("WitController", true); // read mode
  nvsInit = nvsPrefs.isKey("nvsInit");
  if (nvsInit) {
    debug_println("readPreferences(): Non-volitile storage is initialised");
    currentThrottleIndex = 0;
    currentThrottleIndexChar = '0';

    int count = 0;  
    char key[4];
    key[3] = 0;

    // int currentThrottle = 0;
    key[0] = 'L';
    for (int i=0; i<MAX_THROTTLES; i++) {
      key[1] = '0' + i;
      for (int j=0; j<10; j++) { // assume a maximum of 10 locos per throttle
        key[2] = '0' + j;
        if (nvsPrefs.isKey(key)) {
          // if ( (currentThrottle != i) && (count>0) ) {
          //   doOneStartupCommand("5"); //nextThrottle
          //   currentThrottle = i;
          // }
          String loco = nvsPrefs.getString(key);
          // doOneStartupCommand("*1" + loco + "#");

          loco = getLocoWithLength(loco);
          debug_print("add Loco: "); debug_println(loco);
          wiThrottleProtocol.addLocomotive(key[1], loco);
          wiThrottleProtocol.getDirection(key[1], loco);
          wiThrottleProtocol.getSpeed(key[1]);
          count++;
        } else {
          debug_print("readPreferences(): Not Found - Key: "); debug_println(key);
        }
      }
    }
    currentThrottleIndex = 0;
    currentThrottleIndexChar = '0';
    resetFunctionStates(currentThrottleIndex);
    writeOledSpeed();
  } else {
    debug_println("readPreferences(): Non-volitile storage not initialised");
  }
  preferencesRead = true;
  nvsPrefs.end();
}

void writePreferences() {
  debug_println("writePreferences(): Writing preferences to non-volitile storage ");
  nvsPrefs.begin("WitController", false); // write mode

  if (nvsInit) {
    nvsPrefs.putBool("nvsInit", true);

    int count = 0;  
    char key[4];
    key[3] = 0;

    key[0] = 'L';
    for (int i=0; i<maxThrottles; i++) {
      key[1] = '0' + i;
      for (int j=0; j<10; j++) {
        key[2] = '0' + j;
        if (j<wiThrottleProtocol.getNumberOfLocomotives(getMultiThrottleChar(i))) {
          String loco = wiThrottleProtocol.getLocomotiveAtPosition(getMultiThrottleChar(i), j);
          String locoNumber = loco.substring(1);
          nvsPrefs.putString(key, locoNumber);
          debug_print("writePreferences(): Key: "); debug_print(key); debug_print(" - "); debug_println(locoNumber);
          count++;
        } else {
          if (nvsPrefs.isKey(key)) {
            nvsPrefs.remove(key);
            debug_print("writePreferences(): Removed Key: "); debug_println(key);
          }
        }
      }
    }
  } else {
    debug_println("writePreferences(): Non-volitile storage not initialised");
  }
  nvsPrefs.end();
}

void clearPreferences() {
  setupPreferences(true);
}


// *********************************************************************************
//   Rotary Encoder
// *********************************************************************************

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
void IRAM_ATTR readEncoderISR(void) {
  rotaryEncoder.readEncoder_ISR();
}

void rotary_onButtonClick() {
   if (encoderUseType == ENCODER_USE_OPERATION) {
    if ( (keypadUseType!=KEYPAD_USE_SELECT_WITHROTTLE_SERVER)
        && (keypadUseType!=KEYPAD_USE_ENTER_WITHROTTLE_SERVER)
        && (keypadUseType!=KEYPAD_USE_SELECT_SSID) 
        && (keypadUseType!=KEYPAD_USE_SELECT_SSID_FROM_FOUND) ) {

      if ( (millis() - rotaryEncoderButtonLastTimePressed) < rotaryEncoderButtonEncoderDebounceTime) {   //ignore multiple press in that specified time
        debug_println("encoder button debounce");
        return;
      }
      rotaryEncoderButtonLastTimePressed = millis();

      // if (encoderButtonAction == SPEED_STOP_THEN_TOGGLE_DIRECTION) {
      //   if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar)>0) {
      //     if (currentSpeed[currentThrottleIndex] != 0) {
      //       // wiThrottleProtocol.setSpeed(currentThrottleIndexChar, 0);
      //       speedSet(currentThrottleIndex,0);
      //     } else {
      //       if (toggleDirectionOnEncoderButtonPressWhenStationary) toggleDirection(currentThrottleIndex);
      //     }
      //     currentSpeed[currentThrottleIndex] = 0;
      //   }
      // } else {
        doDirectAction(encoderButtonAction);
      // }
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

    if ( (millis() - rotaryEncoderButtonLastTimePressed) < rotaryEncoderButtonEncoderDebounceTime) {   //ignore the encoder change if the button was pressed recently
      debug_println("encoder button debounce - in Rotary_loop()");
      return;
    // } else {
    //   debug_print("encoder button debounce - time since last press: ");
    //   debug_println(millis() - rotaryEncoderButtonLastTimePressed);
    }

    if (abs(encoderValue-lastEncoderValue) > 800) { // must have passed through zero
      if (encoderValue > 800) {
        lastEncoderValue = 1001; 
      } else {
        lastEncoderValue = 0; 
      }
      debug_print("Corrected Encoder From: "); debug_print(lastEncoderValue); debug_print(" to: "); debug_println(encoderValue);
    }
 
    if (encoderUseType == ENCODER_USE_OPERATION) {
      if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar)>0) {
        if (encoderValue > lastEncoderValue) {
          if (abs(encoderValue-lastEncoderValue)<50) {
            encoderSpeedChange(true, currentSpeedStep[currentThrottleIndex]);
          } else {
            encoderSpeedChange(true, currentSpeedStep[currentThrottleIndex]*speedStepMultiplier);
          }
        } else {
          if (abs(encoderValue-lastEncoderValue)<50) {
            encoderSpeedChange(false, currentSpeedStep[currentThrottleIndex]);
          } else {
            encoderSpeedChange(false, currentSpeedStep[currentThrottleIndex]*speedStepMultiplier);
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

void encoderSpeedChange(bool rotationIsClockwise, int speedChange) {
  if (encoderRotationClockwiseIsIncreaseSpeed) {
    if (rotationIsClockwise) {
      speedUp(currentThrottleIndex, speedChange);
    } else {
      speedDown(currentThrottleIndex, speedChange);
    }
  } else {
    if (rotationIsClockwise) {
      speedDown(currentThrottleIndex, speedChange);
    } else {
      speedUp(currentThrottleIndex, speedChange);
    }
  }
}

// *********************************************************************************
//   Throttle Pot
// *********************************************************************************

void throttlePot_loop() {
  throttlePot_loop(false);
}
void throttlePot_loop(bool forceRead) {
  // debug_println("throttlePot_loop() start: ");

  if ( (millis() < lastThrottlePotReadTime + 100) 
    && (!forceRead) ) { // only ready it one every x seconds
    return;
  }
  lastThrottlePotReadTime = millis();

  // Read the throttle pot to see what notch it is on.
  int currentThrottlePotNotch = throttlePotNotch;
  int potValue = analogRead(throttlePotPin);  //Reads the analog value on the throttle pin.
  // potValue = analogRead(throttlePotPin);
  debug_println("Pot Value: "); debug_println(potValue);

  // average out the last x values from the pot
  int noElements = sizeof(lastThrottlePotValues) / sizeof(lastThrottlePotValues[0]);
  int avgPotValue = 0;
  for (int i=1; i<noElements; i++) {
    lastThrottlePotValues[i-1] = lastThrottlePotValues[i];
    avgPotValue = avgPotValue + lastThrottlePotValues[i-1];
  }
  lastThrottlePotValues[noElements-1] = potValue;
  avgPotValue = (avgPotValue + potValue) / noElements;

  // get the highest recent value
  lastThrottlePotHighValue = -1;
  for (int i=0; i<noElements; i++) {
    if (lastThrottlePotValues[i] > lastThrottlePotHighValue) 
    lastThrottlePotHighValue = lastThrottlePotValues[i];
  }

  // // save the lowest and highest pot values seen
  // if (avgPotValue<lowestThrottlePotValue) lowestThrottlePotValue = avgPotValue;
  // if (avgPotValue>highestThrottlePotValue) highestThrottlePotValue = avgPotValue;

  // only do something if the pot value is sufficiently different
  // or deliberate read 
  if ( (avgPotValue<lastThrottlePotValue-5) || (avgPotValue>lastThrottlePotValue+5)
  || (forceRead) )  { 
   
    lastThrottlePotValue = avgPotValue;
    debug_print("Avg Pot Value: "); debug_println(avgPotValue);

    if (throttlePotUseNotches) { // use notches
      throttlePotNotch = 0;
      for (int i=0; i<8; i++) {
        if (avgPotValue < throttlePotNotchValues[i]) {    /// Check to see if it is in notch i
          throttlePotTargetSpeed = throttlePotNotchSpeeds[i];
          throttlePotNotch = i;
          break;
        }                
      } 
      if (throttlePotNotch!=currentThrottlePotNotch) {
            speedSet(currentThrottleIndex, throttlePotTargetSpeed);
      }

    } else { // use a linear speed
      double newSpeed = potValue-throttlePotNotchValues[0];
      newSpeed = newSpeed / (throttlePotNotchValues[7]-throttlePotNotchValues[0]);
      newSpeed = newSpeed * 127;
      if (newSpeed<0) { newSpeed = 0; }
      else if (newSpeed>127) { newSpeed = 127; }
      int iSpeed = newSpeed;
      debug_print("newSpeed: "); debug_print(newSpeed); debug_print(" iSpeed: "); debug_println(iSpeed);
      speedSet(currentThrottleIndex, iSpeed);
    }  
  }
}

// *********************************************************************************
//   Battery Test
// *********************************************************************************

void batteryTest_loop() {
  // Read the battery pin
#if USE_BATTERY_TEST
  if(millis()-lastBatteryCheckTime>10000) {
    lastBatteryCheckTime = millis();
    int batteryTestValue = BL.getBatteryChargeLevel();
    lastBatteryAnalogReadValue = BL.getLastAnalogReadValue();
    
    debug_print("BATTERY TestValue: "); debug_println(batteryTestValue); 
    debug_print("BATTERY lastAnalogReadValue: "); debug_println(lastBatteryAnalogReadValue); 
    double analogValue = lastBatteryAnalogReadValue;
    analogValue = 4.2 / analogValue * 1000;
    debug_print("BATTERY If Battery full, BATTERY_CONVERSION_FACTOR should be: "); debug_println(analogValue); 

    if (batteryTestValue!=lastBatteryTestValue) { 
      lastBatteryTestValue = BL.getBatteryChargeLevel();
      if ( (keypadUseType == KEYPAD_USE_OPERATION) && (!menuIsShowing)) {
        writeOledSpeed();
      }
    }
    if (lastBatteryTestValue<USE_BATTERY_SLEEP_AT_PERCENT) { // shutdown if <x% battery
      deepSleepStart(SLEEP_REASON_BATTERY);
    }
  }
#endif
}

// *********************************************************************************
//   keypad
// *********************************************************************************

void keypadEvent(KeypadEvent key) {
  debug_print("keypadEvent((): "); debug_println(key); 
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

  for (int i = 0; i < maxAdditionalButtons; i++) { 
    if (additionalButtonActions[i] != FUNCTION_NULL) { 
      debug_print("Additional Button: "); debug_print(i); debug_print(" pin:"); debug_println(additionalButtonPin[i]);

      if (additionalButtonPin[i]>=0) {
        pinMode(additionalButtonPin[i], additionalButtonType[i]);
        additionalButtonLastRead[i] = LOW;
      }
      lastAdditionalButtonDebounceTime[i] = 0;
    }
  }
}

void additionalButtonLoop() {
  if (witConnectionState != CONNECTION_STATE_CONNECTED) return;

  int buttonRead;
  for (int i = 0; i < maxAdditionalButtons; i++) {   
    if ( (additionalButtonActions[i] != FUNCTION_NULL) && (additionalButtonPin[i]>=0) ) {

      buttonRead = digitalRead(additionalButtonPin[i]);

      if (additionalButtonLastRead[i] != buttonRead) { // on process on a change
        if ((millis() - lastAdditionalButtonDebounceTime[i]) > additionalButtonDebounceDelay) {   // only process if there is sufficent delay since the last read
          lastAdditionalButtonDebounceTime[i] = millis();
          additionalButtonRead[i] = buttonRead;

          if ( ((additionalButtonType[i] == INPUT_PULLUP) && (additionalButtonRead[i] == LOW)) 
              || ((additionalButtonType[i] == INPUT) && (additionalButtonRead[i] == HIGH)) ) {
            debug_print("Additional Button Pressed: "); debug_print(i); debug_print(" pin:"); debug_print(additionalButtonPin[i]); debug_print(" action:"); debug_println(additionalButtonActions[i]); 
            if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar) > 0) { // only process if there are locos aquired
              doDirectAdditionalButtonCommand(i,true);
            } else { // check for actions not releted to a loco
              int buttonAction = additionalButtonActions[i];
              if (buttonAction >= 500) {
                  doDirectAdditionalButtonCommand(i,true);
              }
            }
          } else {
            debug_print("Additional Button Released: "); debug_print(i); debug_print(" pin:"); debug_print(additionalButtonPin[i]); debug_print(" action:"); debug_println(additionalButtonActions[i]); 
            if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar) > 0) { // only process if there are locos aquired
              doDirectAdditionalButtonCommand(i,false);
            } else { // check for actions not releted to a loco
              int buttonAction = additionalButtonActions[i];
              if (buttonAction >= 500) {
                  doDirectAdditionalButtonCommand(i,false);
              }
            }
          }
        } else {
          debug_println("Ignoring Additional Button Press");
        }
      }
      additionalButtonLastRead[i] = additionalButtonRead[i];
    }
  }
}

// *********************************************************************************
//  Setup and Loop
// *********************************************************************************

void setup() {
  Serial.begin(115200);
  // u8g2.setI2CAddress(0x3C * 2);
  // u8g2.setBusClock(100000);
  u8g2.begin();
  u8g2.firstPage();

  delay(1000);
  debug_println("Start"); 
  debug_print("WiTcontroller - Version: "); debug_println(appVersion);

  batteryTest_loop();  // do the battery check once to start

  clearOledArray(); oledText[0] = appName; oledText[6] = appVersion; oledText[2] = MSG_START;
  writeOledBattery();
  writeOledArray(false, false, true, true);

  rotaryEncoder.begin();  //initialize rotary encoder
  rotaryEncoder.setup(readEncoderISR);
  //set boundaries and if values should cycle or not 
  rotaryEncoder.setBoundaries(0, 1000, circleValues); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
  //rotaryEncoder.disableAcceleration(); //acceleration is now enabled by default - disable if you don't need it
  rotaryEncoder.setAcceleration(100); //or set the value - larger number = more acceleration; 0 or 1 means disabled acceleration

  //if EC11 is used in hardware build WITHOUT physical pullup resistore, then make then enable GPIO pullups on EC11 A and B inputs
  if (EC11_PULLUPS_REQUIRED) {
    // debug_println("EC11 A and B input pins, enabling GPIO pullups " );
    pinMode(ROTARY_ENCODER_A_PIN, INPUT_PULLUP);
    pinMode(ROTARY_ENCODER_B_PIN, INPUT_PULLUP);
  }

  keypad.addEventListener(keypadEvent); // Add an event listener for this keypad
  keypad.setDebounceTime(KEYPAD_DEBOUNCE_TIME);
  keypad.setHoldTime(KEYPAD_HOLD_TIME);

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13,0); //1 = High, 0 = Low

  keypadUseType = KEYPAD_USE_SELECT_SSID;
  encoderUseType = ENCODER_USE_OPERATION;
  ssidSelectionSource = SSID_CONNECTION_SOURCE_BROWSE;

  initialiseAdditionalButtons();

  resetAllFunctionLabels();
  resetAllFunctionFollow();

  for (int i=0; i< 6; i++) {
    currentSpeed[i] = 0;
    currentDirection[i] = Forward;
    currentSpeedStep[i] = speedStep;
  }
  
  WiFi.setHostname(DEVICE_NAME);
  #if USE_COUNTRY_CODE
    esp_wifi_set_country_code("01", false);
  #endif
}

void loop() {
  
  if (ssidConnectionState != CONNECTION_STATE_CONNECTED) {
    // connectNetwork();
    ssidsLoop();
    checkForShutdownOnNoResponse();
  } else {  
    if (witConnectionState != CONNECTION_STATE_CONNECTED) {
      witServiceLoop();
      checkForShutdownOnNoResponse();
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
  // char key = keypad.getKey();
  keypad.getKey(); 
  if (useRotaryEncoderForThrottle) { rotary_loop(); }
  else { throttlePot_loop(); }
  additionalButtonLoop(); 

  if (useBatteryTest) { batteryTest_loop(); }

	// debug_println("loop:" );
}

// *********************************************************************************
//  Key press and Menu
// *********************************************************************************

void doKeyPress(char key, bool pressed) {
    debug_print("doKeyPress(): key: "); debug_print(key); debug_print(" keypadUseType: ");debug_println(keypadUseType);

  if (pressed)  { //pressed
    debug_println("doKeyPress(): pressed"); 
    switch (keypadUseType) {
      case KEYPAD_USE_OPERATION:
        debug_print("doKeyPress(): key operation... "); debug_println(key);

        switch (key) {
          case '*':  // menu command
            menuCommand = "";
            if (menuCommandStarted) { // then cancel the menu
              resetMenu();
              writeOledSpeed();
            } else {
              menuCommandStarted = true;
              debug_println("doKeyPress(): Command started");
              writeOledMenu("", true);
            }
            break;

          case '#': // end of command
            debug_print("doKeyPress(): end of command... "); debug_print(key); debug_print ("  :  "); debug_println(menuCommand);
            if ((menuCommandStarted) && (menuCommand.length()>=1)) {
              doMenu();
            } else {
              if (!hashShowsFunctionsInsteadOfKeyDefs) {
                if (!oledDirectCommandsAreBeingDisplayed) {
                  writeOledDirectCommands();
                } else {
                  oledDirectCommandsAreBeingDisplayed = false;
                  writeOledSpeed();
                }
              } else {
                writeOledFunctionList(""); 
              }
            }
            break;

          case '0': case '1': case '2': case '3': case '4': 
          case '5': case '6': case '7': case '8': case '9': 
          {
            debug_print("doKeyPress(): number... "); debug_print(key); 
            debug_print ("  cmd: '"); debug_println(menuCommand); 
            debug_print("' : "); debug_println(menuCharsRequired[menuCommand.substring(0,1).toInt()]);

            int index0 = key-48;
            int index1 = 0;
            if (menuCommand.length() >= 0) index1 = menuCommand.charAt(0)-48;
            int index2 = 0;
            if (menuCommand.length() >= 1) index2 = menuCommand.charAt(1)-48;

            if (menuCommandStarted) { // append to the string

              // if ( ((menuCommand.length() == 0) && (menuCharsRequired[key-48] == MENU_ITEM_TYPE_DIRECT_COMMAND))
              // || ((menuCommand.length() == 1) && (menuCharsRequired[menuCommand.charAt(0)-48] == MENU_ITEM_TYPE_SUB_MENU))
              // || ((menuCommand.length() == 1) && (menuCharsRequired[menuCommand.charAt(0)-48] == MENU_ITEM_TYPE_DIRECT_COMMAND))
              // || ((menuCommand.length() == 2) && (menuCharsRequired[menuCommand.charAt(1)-38] == MENU_ITEM_TYPE_DIRECT_COMMAND)) ) { 

              if ( ((menuCommand.length() == 0) 
                    && (menuCharsRequired[index0] == MENU_ITEM_TYPE_DIRECT_COMMAND))
              || ((menuCommand.length() == 1) 
                    &&  ( (menuCharsRequired[index1] == MENU_ITEM_TYPE_SUB_MENU)
                       || (menuCharsRequired[index1] == MENU_ITEM_TYPE_DIRECT_COMMAND) ))
              || ((menuCommand.length() == 2) 
                    &&  (menuCharsRequired[index1] != MENU_ITEM_TYPE_ONE_OR_MORE_CHARS)
                    &&  (menuCharsRequired[index2] == MENU_ITEM_TYPE_DIRECT_COMMAND)) 
              ) { 
                debug_println("doKeyPress(): MENU_ITEM_TYPE_DIRECT_COMMAND : ");
                menuCommand += key;
                doMenu();
              } else {
                if ((menuCharsRequired[index0] == MENU_ITEM_TYPE_SUB_MENU) 
                && (menuCommand.length() == 0)) {  // menu type needs only one char
                  debug_println("doKeyPress(): MENU_ITEM_TYPE_SUB_MENU : "); 
                  menuCommand += key;
                  writeOledMenu(menuCommand, false);
                // } else if ( (menuCharsRequired[index0] == MENU_ITEM_TYPE_SELECT_FROM_LIST) 
                //           && (menuCommand.length() == 0)) {  // menu type needs only one char
                //   debug_println("doKeyPress(): MENU_ITEM_TYPE_SELECT_FROM_LIST : "); 
                //   menuCommand += key;
                //   writeOledMenu(menuCommand, false);
                } else {  //menu type allows/requires more than one char
                  debug_println("doKeyPress(): MENU_ITEM_TYPE_ONE_OR_MORE_CHARS");
                  menuCommand += key;
                  writeOledMenu(menuCommand, true);
                }
              }
            } else {
              doDirectCommand(key, true);
            }
            break;
          }
          default:  // A, B, C, D
            doDirectCommand(key, true);
            break;
        }
        break;

      case KEYPAD_USE_ENTER_WITHROTTLE_SERVER:
        debug_print("doKeyPress(): key: Enter wit... "); debug_println(key);
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
        debug_print("doKeyPress(): key: Enter password... "); debug_println(key);
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
        debug_print("doKeyPress(): key: Select wit... "); debug_println(key);
        switch (key){
          case '0': case '1': case '2': case '3': case '4':
            selectWitServer(key - '0');
            break;
          case '#': // show ip address entry screen
            witConnectionState = CONNECTION_STATE_ENTRY_REQUIRED;
            buildWitEntry();
            enterWitServer();
            break;
          default:  // do nothing 
            break;
        }
        break;

      case KEYPAD_USE_SELECT_SSID:
        debug_print("doKeyPress(): key ssid... "); debug_println(key);
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
        debug_print("doKeyPress(): key ssid from found... "); debug_println(key);
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
        debug_print("doKeyPress(): key Roster... "); debug_println(key);
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
        debug_print("doKeyPress(): key turnouts... "); debug_println(key);
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
        debug_print("doKeyPress(): key routes... "); debug_println(key);
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

      case KEYPAD_USE_SELECT_FUNCTION:
        debug_print("doKeyPress(): key function... "); debug_println(key);
        switch (key){
          case '0': case '1': case '2': case '3': case '4': 
          case '5': case '6': case '7': case '8': case '9':
            selectFunctionList((key - '0')+(functionPage*10));
            break;
          case '#':  // next page
            if ( (functionPage+1)*10 < MAX_FUNCTIONS ) {
              functionPage++;
              writeOledFunctionList(""); 
            } else {
              functionPage = 0;
              keypadUseType = KEYPAD_USE_OPERATION;
              writeOledDirectCommands();
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

      case KEYPAD_USE_EDIT_CONSIST:
        debug_print("key Edit Consist... "); debug_println(key);
        switch (key){
          case '0': case '1': case '2': case '3': case '4': 
          case '5': case '6': case '7': case '8': case '9':
            if ( (key-'0') <= wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar)) {
              selectEditConsistList(key - '0');
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
    debug_println("doKeyPress(): released"); 
    switch (keypadUseType) {
      case KEYPAD_USE_OPERATION:
        if ( (!menuCommandStarted) && (key>='0') && (key<='D')) { // only process releases for the numeric keys + A,B,C,D and only if a menu command has not be started
          debug_println("doKeyPress(): Operation - Process key release KEYPAD_USE_OPERATION");
          doDirectCommand(key, false);
        } else {
          debug_println("doKeyPress(): Non-Operation - Don't process key release");
        }
        break;
    
      case KEYPAD_USE_SELECT_FUNCTION:
        if (functionHasBeenSelected) {
          debug_println("doKeyPress(): Operation - Process key release KEYPAD_USE_SELECT_FUNCTION");
          doFunction(currentThrottleIndex, (key - '0')+(functionPage*10), false);
          keypadUseType = KEYPAD_USE_OPERATION;
          functionHasBeenSelected = false;
        }
        break;
    }
  }

  debug_println("doKeyPress(): end"); 
}

void doDirectCommand(char key, bool pressed) {
  debug_print("doDirectCommand(): key: "); debug_println(key);
  int buttonAction = 0 ;
  if (key<=57) {
    buttonAction = buttonActions[(key - '0')];
  } else {
    buttonAction = buttonActions[(key - 55)]; // A, B, C, D
  }
  debug_print("doDirectCommand(): Action: "); debug_println(buttonAction);
  if (buttonAction!=FUNCTION_NULL) {
    if ( (buttonAction>=FUNCTION_0) && (buttonAction<=FUNCTION_31) ) {
      doDirectFunction(currentThrottleIndex, buttonAction, pressed);
  } else {
      if (pressed) { // only process these on the key press, not the release
        doDirectAction(buttonAction);
      }
    }
  }
  // debug_println("doDirectCommand(): end"); 
}

void doDirectAdditionalButtonCommand (int buttonIndex, bool pressed) {
  debug_print("doDirectAdditionalButtonCommand(): index: "); debug_println(buttonIndex);
  int buttonAction = additionalButtonActions[buttonIndex];

  if (buttonAction!=FUNCTION_NULL) {
    if ( (buttonAction>=FUNCTION_0) && (buttonAction<=FUNCTION_31) ) {

      if (additionalButtonOverrideDefaultLatching) {
        bool latch = additionalButtonLatching[buttonIndex];
        bool currentlyOn = functionStates[currentThrottleIndex][buttonAction];

        if (!latch) {
          doDirectFunction(currentThrottleIndex, buttonAction, pressed, true);
        } else {
          if ( (!currentlyOn) && (pressed) ) {
            doDirectFunction(currentThrottleIndex, buttonAction, true, true);
          } else if ( (currentlyOn) && (pressed) ) {
            doDirectFunction(currentThrottleIndex, buttonAction, false, true);
          }
        }
      } else {
        doDirectFunction(currentThrottleIndex, buttonAction, pressed, false);
      }
    } else { // not a function
      if (pressed) { // only process these on the key press, not the release
        doDirectAction(buttonAction);
      }
    }
  }
  // debug_println("doDirectAdditionalButtonCommand(): end ");
}

void doDirectAction(int buttonAction) {
  debug_println("doDirectAction(): ");
  switch (buttonAction) {
      case DIRECTION_FORWARD: {
        changeDirection(currentThrottleIndex, Forward);
        break; 
      }
      case DIRECTION_REVERSE: {
        changeDirection(currentThrottleIndex, Reverse);
        break; 
      }
      case DIRECTION_TOGGLE: {
        toggleDirection(currentThrottleIndex);
        break; 
      }
      case SPEED_STOP: {
        speedSet(currentThrottleIndex, 0);
        break; 
      }
      case SPEED_UP: {
        speedUp(currentThrottleIndex, currentSpeedStep[currentThrottleIndex]);
        break; 
      }
      case SPEED_DOWN: {
        speedDown(currentThrottleIndex, currentSpeedStep[currentThrottleIndex]);
        break; 
      }
      case SPEED_UP_FAST: {
        speedUp(currentThrottleIndex, currentSpeedStep[currentThrottleIndex]*speedStepMultiplier);
        break; 
      }
      case SPEED_DOWN_FAST: {
        speedDown(currentThrottleIndex, currentSpeedStep[currentThrottleIndex]*speedStepMultiplier);
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
      case E_STOP_CURRENT_LOCO: {
        speedEstopCurrentLoco();
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
      case SHOW_HIDE_BATTERY: {
        batteryShowToggle();
        break; 
      }
      case SLEEP: {
        deepSleepStart();
        break; 
      }
      case NEXT_THROTTLE: {
        nextThrottle();
        break; 
      }
      case THROTTLE_1: {
        throttle(0);
        break; 
      }
      case THROTTLE_2: {
        throttle(1);
        break; 
      }
      case THROTTLE_3: {
        throttle(2);
        break; 
      }
      case THROTTLE_4: {
        throttle(3);
        break; 
      }
      case THROTTLE_5: {
        throttle(4);
        break; 
      }
      case THROTTLE_6: {
        throttle(5);
        break; 
      }
      case SPEED_STOP_THEN_TOGGLE_DIRECTION: {
        stopThenToggleDirection();
        break; 
      }
      case MAX_THROTTLE_INCREASE: {
        changeNumberOfThrottles(true);
        break; 
      }
      case MAX_THROTTLE_DECREASE: {
        changeNumberOfThrottles(false);
        break; 
      }
      case CUSTOM_1: {
        wiThrottleProtocol.sendCommand(CUSTOM_COMMAND_1);
        break; 
      }
      case CUSTOM_2: {
        wiThrottleProtocol.sendCommand(CUSTOM_COMMAND_2);
        break; 
      }
      case CUSTOM_3: {
        wiThrottleProtocol.sendCommand(CUSTOM_COMMAND_3);
        break; 
      }
      case CUSTOM_4: {
        wiThrottleProtocol.sendCommand(CUSTOM_COMMAND_4);
        break; 
      }
      case CUSTOM_5: {
        wiThrottleProtocol.sendCommand(CUSTOM_COMMAND_5);
        break; 
      }
      case CUSTOM_6: {
        wiThrottleProtocol.sendCommand(CUSTOM_COMMAND_6);
        break; 
      }
      case CUSTOM_7: {
        wiThrottleProtocol.sendCommand(CUSTOM_COMMAND_7);
        break; 
      }
  }
  // debug_println("doDirectAction(): end");
}

void doMenu() {
  String loco = "";
  String function = "";
  bool menuCommandStartedTemp = false;
  debug_print("doMenu(): "); debug_println(menuCommand);
  
  switch (menuCommand[0]) {
    case MENU_ITEM_EXTRAS: { // Extra menu 
        char subCommand = menuCommand.charAt(1);
        if (menuCommand.length() > 1) { // must be a submenu command
          // debug_println("doMenu(): length>1");
          doMenuCommand(subCommand+65-48); // convert '0'-'9' to 'A' - 'J'
          menuCommandStartedTemp = true;
        } else {
          // debug_println("doMenu(): else");
          writeOledSpeed();
        }
        break;
      }
      default: {
        // debug_println("doMenu(): default");
        doMenuCommand(menuCommand[0]);
        // menuCommandStartedTemp = false;
      }
  }
  menuCommandStarted = menuCommandStartedTemp; 
  debug_println("doMenu(): end");
}

void doMenuCommand(char menuItem) {
  debug_print("doMenuCommand(): "); debug_print(menuCommand); debug_print(" : "); debug_println(menuItem);
  String loco = "";
  String function = "";
  int startAt = 1;               // in main menu
  if (menuItem>'9') startAt = 2; // in the submenu

    switch (menuItem) {
    case MENU_ITEM_ADD_LOCO: { // select loco
        if (menuCommand.length()>startAt) {
          if ( (dropBeforeAcquire) && (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar)>0) ) {
            wiThrottleProtocol.releaseLocomotive(currentThrottleIndexChar, "*");
          }
          loco = menuCommand.substring(startAt, menuCommand.length());
          loco = getLocoWithLength(loco);
          debug_print("add Loco: "); debug_println(loco);
          wiThrottleProtocol.addLocomotive(currentThrottleIndexChar, loco);
          wiThrottleProtocol.getDirection(currentThrottleIndexChar, loco);
          wiThrottleProtocol.getSpeed(currentThrottleIndexChar);
          resetFunctionStates(currentThrottleIndex);
          writeOledSpeed();
        } else {
          page = 0;
          writeOledRoster("");
        }
        break;
      }
    case MENU_ITEM_DROP_LOCO: { // de-select loco
        loco = menuCommand.substring(startAt, menuCommand.length());
        if (loco!="") { // a loco is specified
          if (!CONSIST_RELEASE_BY_INDEX) {
            loco = getLocoWithLength(loco);
            releaseOneLoco(currentThrottleIndex, loco);
          } else {
            releaseOneLocoByIndex(currentThrottleIndex, loco.toInt());
          }
        } else { //not loco specified so release all
          releaseAllLocos(currentThrottleIndex);
        }
        writeOledSpeed();
        break;
      }
    case MENU_ITEM_TOGGLE_DIRECTION: { // change direction
        toggleDirection(currentThrottleIndex);
        break;
      }
     case MENU_ITEM_SPEED_STEP_MULTIPLIER: { // toggle speed step additional Multiplier
        toggleAdditionalMultiplier();
        break;
      }
   case MENU_ITEM_THROW_POINT: {  // throw point
        if (menuCommand.length()>startAt) {
          String turnout = turnoutPrefix + menuCommand.substring(startAt, menuCommand.length());
          // if (!turnout.equals("")) { // a turnout is specified
            debug_print("throw point: "); debug_println(turnout);
            wiThrottleProtocol.setTurnout(turnout, TurnoutThrow);
          // }
          writeOledSpeed();
        } else {
          page = 0;
          writeOledTurnoutList("", TurnoutThrow);
        }
        break;
      }
    case MENU_ITEM_CLOSE_POINT: {  // close point
        if (menuCommand.length()>startAt) {
          String turnout = turnoutPrefix + menuCommand.substring(startAt, menuCommand.length());
          // if (!turnout.equals("")) { // a turnout is specified
            debug_print("close point: "); debug_println(turnout);
            wiThrottleProtocol.setTurnout(turnout, TurnoutClose);
          // }
          writeOledSpeed();
        } else {
          page = 0;
          writeOledTurnoutList("",TurnoutClose);
        }
        break;
      }
    case MENU_ITEM_ROUTE: {  // route
        if (menuCommand.length()>startAt) {
          String route = routePrefix + menuCommand.substring(startAt, menuCommand.length());
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
    case MENU_ITEM_TRACK_POWER: {
        powerToggle();
        break;
      }
    case MENU_ITEM_FUNCTION_KEY_TOGGLE: { // toggle showing Def Keys vs Function labels
        hashShowsFunctionsInsteadOfKeyDefs = !hashShowsFunctionsInsteadOfKeyDefs;
        writeOledSpeed();
        break;
      } 
    case MENU_ITEM_EDIT_CONSIST: { // edit consist - loco facings
        // writeOledEditConsist();
        // break;
        char key = menuCommand.charAt(startAt);
        if (menuCommand.length()>startAt) {
          if ( ((key-'0') > 0) // can't change lead
          && ((key-'0') <= wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar)) ) {
            selectEditConsistList(key - '0');
          }
          writeOledSpeed();
        } else {
          writeOledEditConsist();
        }
        break;
      } 
    case MENU_ITEM_HEARTBEAT_TOGGLE: { // disable/enable the heartbeat Check
        toggleHeartbeatCheck();
        writeOledSpeed();
        break;
      }
    case MENU_ITEM_DROP_BEFORE_ACQUIRE_TOGGLE: { // disable/enable drop before Acquire
        toggleDropBeforeAquire();
        writeOledSpeed();
        break;
      }
    case MENU_ITEM_SAVE_CURRENT_LOCOS: {
        writePreferences();
        writeOledSpeed();
        break;
      }
    case MENU_ITEM_INCREASE_MAX_THROTTLES: { //increase number of Throttles
        changeNumberOfThrottles(true);
        break;
      }
    case MENU_ITEM_DECREASE_MAX_THROTTLES: { // decrease numbe rof throttles
        changeNumberOfThrottles(false);
        break;
      }
    case MENU_ITEM_DISCONNECT: { // disconnect   
        if (witConnectionState == CONNECTION_STATE_CONNECTED) {
          witConnectionState = CONNECTION_STATE_DISCONNECTED;
          // clearPreferences();
          // writePreferences();
          preferencesRead = false;
          disconnectWitServer();
        } else {
          connectWitServer();
        }
        break;
      }
    case MENU_ITEM_OFF_SLEEP: { // sleep/off
        // clearPreferences();
        // writePreferences();
        deepSleepStart();
        break;
      }
    case MENU_ITEM_FUNCTION: { // function button
        if (menuCommand.length()>startAt) {
          function = menuCommand.substring(startAt, menuCommand.length());
          int functionNumber = function.toInt();
          if (function != "") { // a function is specified
            doFunction(currentThrottleIndex, functionNumber, true, true);  // always act like latching i.e. pressed
          }
          writeOledSpeed();
        } else {
          functionPage = 0;
          writeOledFunctionList("");
        }
        break;
      }
  }
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

void resetFunctionStates(int multiThrottleIndex) {
  debug_println("resetFunctionStates()");
  for (int i=0; i<MAX_FUNCTIONS; i++) {
    functionStates[multiThrottleIndex][i] = false;
  }
}

void resetFunctionLabels(int multiThrottleIndex) {
  debug_print("resetFunctionLabels(): "); debug_println(multiThrottleIndex);
  for (int i=0; i<MAX_FUNCTIONS; i++) {
    functionLabels[multiThrottleIndex][i] = "";
  }
  functionPage = 0;
}

void resetAllFunctionLabels() {
  for (int i=0; i<maxThrottles; i++) {
    resetFunctionLabels(i);
  }
}

void resetAllFunctionFollow() {
  for (int i=0; i<6; i++) {
    functionFollow[i][0] = CONSIST_FUNCTION_FOLLOW_F0;
    functionFollow[i][1] = CONSIST_FUNCTION_FOLLOW_F1;
    functionFollow[i][2] = CONSIST_FUNCTION_FOLLOW_F2;
    functionFollow[i][3] = CONSIST_FUNCTION_FOLLOW_F3;
    functionFollow[i][4] = CONSIST_FUNCTION_FOLLOW_F4;
    functionFollow[i][5] = CONSIST_FUNCTION_FOLLOW_F5;
    functionFollow[i][6] = CONSIST_FUNCTION_FOLLOW_F6;
    functionFollow[i][7] = CONSIST_FUNCTION_FOLLOW_F7;
    functionFollow[i][8] = CONSIST_FUNCTION_FOLLOW_F8;
    functionFollow[i][9] = CONSIST_FUNCTION_FOLLOW_F9;
    functionFollow[i][10] = CONSIST_FUNCTION_FOLLOW_F10;
    functionFollow[i][11] = CONSIST_FUNCTION_FOLLOW_F11;
    functionFollow[i][12] = CONSIST_FUNCTION_FOLLOW_F12;
    functionFollow[i][13] = CONSIST_FUNCTION_FOLLOW_F13;
    functionFollow[i][14] = CONSIST_FUNCTION_FOLLOW_F14;
    functionFollow[i][15] = CONSIST_FUNCTION_FOLLOW_F15;
    functionFollow[i][16] = CONSIST_FUNCTION_FOLLOW_F16;
    functionFollow[i][17] = CONSIST_FUNCTION_FOLLOW_F17;
    functionFollow[i][18] = CONSIST_FUNCTION_FOLLOW_F18;
    functionFollow[i][19] = CONSIST_FUNCTION_FOLLOW_F19;
    functionFollow[i][20] = CONSIST_FUNCTION_FOLLOW_F20;
    functionFollow[i][21] = CONSIST_FUNCTION_FOLLOW_F21;
    functionFollow[i][22] = CONSIST_FUNCTION_FOLLOW_F22;
    functionFollow[i][23] = CONSIST_FUNCTION_FOLLOW_F23;
    functionFollow[i][24] = CONSIST_FUNCTION_FOLLOW_F24;
    functionFollow[i][25] = CONSIST_FUNCTION_FOLLOW_F25;
    functionFollow[i][26] = CONSIST_FUNCTION_FOLLOW_F26;
    functionFollow[i][27] = CONSIST_FUNCTION_FOLLOW_F27;
    functionFollow[i][28] = CONSIST_FUNCTION_FOLLOW_F28;
    functionFollow[i][29] = CONSIST_FUNCTION_FOLLOW_F29;
    functionFollow[i][30] = CONSIST_FUNCTION_FOLLOW_F30;
    functionFollow[i][31] = CONSIST_FUNCTION_FOLLOW_F31;
  }
}

String getLocoWithLength(String loco) {
  int locoNo = loco.toInt();
  String locoWithLength = "";
  if ( (locoNo > SHORT_DCC_ADDRESS_LIMIT) 
  || ( (locoNo <= SHORT_DCC_ADDRESS_LIMIT) && (loco.charAt(0)=='0') && (!serverType.equals("DCC-EX" ) ) ) 
  ) {
    locoWithLength = "L" + loco;
  } else {
    locoWithLength = "S" + loco;
  }
  return locoWithLength;
}

void speedEstop() {
  debug_println("Speed EStop"); 
  wiThrottleProtocol.emergencyStop();
  for (int i=0; i<maxThrottles; i++) {
    speedSet(getMultiThrottleChar(i),0);
    currentSpeed[i] = 0;
  }
  writeOledSpeed();
}

void speedEstopCurrentLoco() {
  debug_println("Speed EStop Curent Loco"); 
  wiThrottleProtocol.emergencyStop(currentThrottleIndexChar);
  speedSet(currentThrottleIndexChar,0);
  writeOledSpeed();
}

void speedDown(int multiThrottleIndex, int amt) {
  if (wiThrottleProtocol.getNumberOfLocomotives(getMultiThrottleChar(multiThrottleIndex)) > 0) {
    int newSpeed = currentSpeed[multiThrottleIndex] - amt;
    debug_print("Speed Down: "); debug_println(amt);
    speedSet(multiThrottleIndex, newSpeed);
  }
}

void speedUp(int multiThrottleIndex, int amt) {
  if (wiThrottleProtocol.getNumberOfLocomotives(getMultiThrottleChar(multiThrottleIndex)) > 0) {
    int newSpeed = currentSpeed[multiThrottleIndex] + amt;
    debug_print("Speed Up: "); debug_println(amt);
    speedSet(multiThrottleIndex, newSpeed);
  }
}

void speedSet(int multiThrottleIndex, int amt) {
  debug_println("setSpeed()");
  char multiThrottleIndexChar = getMultiThrottleChar(multiThrottleIndex);
  if (wiThrottleProtocol.getNumberOfLocomotives(multiThrottleIndexChar) > 0) {
    int newSpeed = amt;
    if (newSpeed >126) { newSpeed = 126; }
    if (newSpeed <0) { newSpeed = 0; }
    wiThrottleProtocol.setSpeed(multiThrottleIndexChar, newSpeed);
    currentSpeed[multiThrottleIndex] = newSpeed;
    debug_print("Speed Set: "); debug_println(newSpeed);

    // used to avoid bounce
    lastSpeedSentTime = millis();
    lastSpeedSent = newSpeed;
    // lastDirectionSent = -1;
    lastSpeedThrottleIndex = multiThrottleIndex;

    if ( (keypadUseType == KEYPAD_USE_OPERATION) && (!menuIsShowing) 
    && (multiThrottleIndex==currentThrottleIndex) ) {
      writeOledSpeed();
    }
  }
}

int getDisplaySpeed(int multiThrottleIndex) {
  if (speedDisplayAsPercent) {
    float speed = currentSpeed[multiThrottleIndex];
    speed = speed / 126 *100;
    int iSpeed = speed;
    if (iSpeed-speed >= 0.5) {
      iSpeed = iSpeed + 1;
    }
    return iSpeed;
  } else {
    if (speedDisplayAs0to28) {
      float speed = currentSpeed[multiThrottleIndex];
      speed = speed / 126 *28;
      int iSpeed = speed;
      if (iSpeed-speed >= 0.5) {
        iSpeed = iSpeed + 1;
      }
      return iSpeed;
    } else {
      return currentSpeed[multiThrottleIndex];
    }
  }
}

void stealLoco(int multiThrottleIndex, String loco) {
  wiThrottleProtocol.stealLocomotive(multiThrottleIndex, loco);  
}

void toggleLocoFacing(int multiThrottleIndex, String loco) {
  debug_println("toggleLocoFacing()");
  char multiThrottleIndexChar = getMultiThrottleChar(multiThrottleIndex);
  debug_print("toggleLocoFacing(): "); debug_println(loco); 
  for(int i=0;i<wiThrottleProtocol.getNumberOfLocomotives(multiThrottleIndexChar);i++) {
    if (wiThrottleProtocol.getLocomotiveAtPosition(multiThrottleIndexChar, i).equals(loco)) {
      Direction currentDirection = wiThrottleProtocol.getDirection(multiThrottleIndexChar, loco);
      debug_print("toggleLocoFacing(): loco: ");  debug_print(loco);  debug_print(" current direction: "); debug_println(currentDirection);
      if (wiThrottleProtocol.getDirection(multiThrottleIndexChar, loco) == Forward) {
        wiThrottleProtocol.setDirection(multiThrottleIndexChar, loco, Reverse, true);
      } else {
        wiThrottleProtocol.setDirection(multiThrottleIndexChar, loco, Forward, true);
      }
      break;
    }
  } 
}

int getLocoFacing(int multiThrottleIndex, String loco) {
  char multiThrottleIndexChar = getMultiThrottleChar(multiThrottleIndex);
  int result = Forward;
  for(int i=0;i<wiThrottleProtocol.getNumberOfLocomotives(multiThrottleIndexChar);i++) {
    if (wiThrottleProtocol.getLocomotiveAtPosition(multiThrottleIndexChar, i).equals(loco)) {
      result = wiThrottleProtocol.getDirection(multiThrottleIndexChar, loco);
      break;
    }
  }
  return result;
}

String getDisplayLocoString(int multiThrottleIndex, int index) {
  char multiThrottleIndexChar = getMultiThrottleChar(multiThrottleIndex);
  String loco = wiThrottleProtocol.getLocomotiveAtPosition(multiThrottleIndexChar, index);
  String locoNumber = loco.substring(1);
  
  #ifdef DISPLAY_LOCO_NAME
    for (int i = 0; i < maxRoster; i++) {
      if (String(rosterAddress[i]) == locoNumber) {
        if (rosterName[i] != "") {
          locoNumber = rosterName[i];
        }
        break;
      }
    }
  #endif
  
  if (!wiThrottleProtocol.getLocomotiveAtPosition(multiThrottleIndexChar, 0).equals(loco)) { // not the lead loco
    Direction leadLocoDirection 
        = wiThrottleProtocol.getDirection(multiThrottleIndexChar, 
                                          wiThrottleProtocol.getLocomotiveAtPosition(multiThrottleIndexChar, 0));
    // Direction locoDirection = leadLocoDirection;

    for(int i=0;i<wiThrottleProtocol.getNumberOfLocomotives(multiThrottleIndexChar);i++) {
      if (wiThrottleProtocol.getLocomotiveAtPosition(multiThrottleIndexChar, i).equals(loco)) {
        if (wiThrottleProtocol.getDirection(multiThrottleIndexChar, loco) != leadLocoDirection) {
          locoNumber = locoNumber + DIRECTION_REVERSE_INDICATOR;
        }
        break;
      }
    }
  }
  return locoNumber;
}

void releaseAllLocos(int multiThrottleIndex) {
  char multiThrottleIndexChar = getMultiThrottleChar(multiThrottleIndex);
  String loco;
  if (wiThrottleProtocol.getNumberOfLocomotives(multiThrottleIndexChar)>0) {
    for(int index=wiThrottleProtocol.getNumberOfLocomotives(multiThrottleIndexChar)-1;index>=0;index--) {
      loco = wiThrottleProtocol.getLocomotiveAtPosition(multiThrottleIndexChar, index);
      wiThrottleProtocol.releaseLocomotive(multiThrottleIndexChar, loco);
      writeOledSpeed();  // note the released locos may not be visible
    } 
    resetFunctionLabels(multiThrottleIndex);
  }
}

void releaseOneLoco(int multiThrottleIndex, String loco) {
  debug_print("releaseOneLoco(): "); debug_print(multiThrottleIndex); debug_print(": "); debug_println(loco);
  char multiThrottleIndexChar = getMultiThrottleChar(multiThrottleIndex);
  wiThrottleProtocol.releaseLocomotive(multiThrottleIndexChar, loco);
  resetFunctionLabels(multiThrottleIndex);
  debug_println("releaseOneLoco(): end"); 
}

void releaseOneLocoByIndex(int multiThrottleIndex, int index) {
  debug_print("releaseOneLocoByIndex(): "); debug_print(multiThrottleIndex); debug_print(": "); debug_println(index);
  char multiThrottleIndexChar = getMultiThrottleChar(multiThrottleIndex);
  if (index <= wiThrottleProtocol.getNumberOfLocomotives(multiThrottleIndexChar)) {
    String loco = wiThrottleProtocol.getLocomotiveAtPosition(multiThrottleIndexChar, index);
    wiThrottleProtocol.releaseLocomotive(multiThrottleIndexChar, loco);
    resetFunctionLabels(multiThrottleIndex);
  }
  debug_println("releaseOneLocoByIndex(): end");
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

  for (int i=0; i<maxThrottles; i++) {
    currentSpeedStep[i] = speedStep * speedStepCurrentMultiplier;
  }
  writeOledSpeed();
}

void toggleHeartbeatCheck() {
  heartbeatCheckEnabled = !heartbeatCheckEnabled;
  debug_print("Heartbeat Check: "); 
  if (heartbeatCheckEnabled) {
    debug_println("Enabled");
    wiThrottleProtocol.requireHeartbeat(true);
  } else {
    debug_println("Disabled");
    wiThrottleProtocol.requireHeartbeat(false);
  }
  writeHeartbeatCheck();
}

void toggleDropBeforeAquire() {
  dropBeforeAcquire = !dropBeforeAcquire;
  debug_print("Drop Before Acquire: "); 
  debug_println(dropBeforeAcquire ? "Enabled" : "Disabled"); 
}

void toggleDirection(int multiThrottleIndex) {
  if (wiThrottleProtocol.getNumberOfLocomotives(getMultiThrottleChar(multiThrottleIndex)) > 0) {
    changeDirection(multiThrottleIndex, (currentDirection[multiThrottleIndex] == Forward) ? Reverse : Forward );
    writeOledSpeed();
  }
}

void changeDirection(int multiThrottleIndex, Direction direction) {
  String loco; String leadLoco; 
  Direction leadLocoCurrentDirection;
  char multiThrottleChar = getMultiThrottleChar(multiThrottleIndex);
  int locoCount = wiThrottleProtocol.getNumberOfLocomotives(multiThrottleChar);

  if (locoCount > 0) {
    currentDirection[multiThrottleIndex] = direction;
    debug_print("changeDirection(): "); debug_println( (direction==Forward) ? "Forward" : "Reverse");

    if (locoCount == 1) {
      debug_println("changeDirection(): one loco");
      wiThrottleProtocol.setDirection(multiThrottleChar, direction);  // change all

    } else {
      debug_println("changeDirection(): multiple locos");
      leadLoco = wiThrottleProtocol.getLeadLocomotive(multiThrottleChar);
      leadLocoCurrentDirection = wiThrottleProtocol.getDirection(multiThrottleChar, leadLoco);

      for (int i=1; i<locoCount; i++) {
        loco = wiThrottleProtocol.getLocomotiveAtPosition(multiThrottleChar, i);
        Direction currentDirection = wiThrottleProtocol.getDirection(multiThrottleChar, loco);
        if (currentDirection == leadLocoCurrentDirection) {
          wiThrottleProtocol.setDirection(multiThrottleChar, loco, direction);
        } else {
          if (direction == Reverse) {
            wiThrottleProtocol.setDirection(multiThrottleChar, loco, Forward);
          } else {
            wiThrottleProtocol.setDirection(multiThrottleChar, loco, Reverse);
          }
        }
      }
      wiThrottleProtocol.setDirection(multiThrottleChar, leadLoco, direction);
    } 
  }
  writeOledSpeed();
  // debug_println("changeDirection(): end "); 
}

void doDirectFunction(int multiThrottleIndex, int functionNumber, bool pressed) {
  doDirectFunction(multiThrottleIndex, functionNumber, pressed, false);
}
void doDirectFunction(int multiThrottleIndex, int functionNumber, bool pressed, bool force) {
  char multiThrottleIndexChar = getMultiThrottleChar(multiThrottleIndex);
  debug_println("doDirectFunction(): "); 
  if (wiThrottleProtocol.getNumberOfLocomotives(multiThrottleIndexChar) > 0) {
    debug_print("direct fn: "); debug_print(functionNumber); debug_println( pressed ? " Pressed" : " Released");
    doFunctionWhichLocosInConsist(multiThrottleIndex, functionNumber, pressed, force);
    writeOledSpeed(); 
  }
  // debug_print("doDirectFunction(): end"); 
}

void doFunction(int multiThrottleIndex, int functionNumber, bool pressed) {
  doFunction(multiThrottleIndex, functionNumber, pressed, false);
}
void doFunction(int multiThrottleIndex, int functionNumber, bool pressed, bool force) {
  char multiThrottleIndexChar = getMultiThrottleChar(multiThrottleIndex);
  debug_print("doFunction(): multiThrottleIndex "); debug_println(multiThrottleIndex);
  if (wiThrottleProtocol.getNumberOfLocomotives(multiThrottleIndexChar)>0) {
    if (force) {
      doFunctionWhichLocosInConsist(multiThrottleIndex, functionNumber, true, force);
      if (!functionStates[multiThrottleIndex][functionNumber]) {
        debug_print("fn: "); debug_print(functionNumber); debug_println(" Pressed FORCED");
        // functionStates[functionNumber] = true;
      } else {
        doFunctionWhichLocosInConsist(multiThrottleIndex, functionNumber, false, force);
        debug_print("fn: "); debug_print(functionNumber); debug_println(" Released FORCED");
        // functionStates[functionNumber] = false;
      }
    } else {
      doFunctionWhichLocosInConsist(multiThrottleIndex, functionNumber, pressed, false);
      debug_print("fn: "); debug_print(functionNumber); debug_println(" NOT FORCED");
    }
    writeOledSpeed(); 
  }
  // debug_println("doFunction(): ");
}

// Work out which locos in a consist should get the function
//
void doFunctionWhichLocosInConsist(int multiThrottleIndex, int functionNumber, bool pressed) {
  doFunctionWhichLocosInConsist(multiThrottleIndex, functionNumber, pressed, false);
}
void doFunctionWhichLocosInConsist(int multiThrottleIndex, int functionNumber, bool pressed, bool force) {
  char multiThrottleIndexChar = getMultiThrottleChar(multiThrottleIndex);
  if (functionFollow[multiThrottleIndex][functionNumber]==CONSIST_LEAD_LOCO) {
    wiThrottleProtocol.setFunction(multiThrottleIndexChar, "", functionNumber, pressed, force);
  } else {  // at the momemnt the only other option in CONSIST_ALL_LOCOS
    wiThrottleProtocol.setFunction(multiThrottleIndexChar, "*", functionNumber, pressed, force);
  }
  debug_print("doFunctionWhichLocosInConsist(): fn: "); debug_print(functionNumber); debug_println(" Released");
}

void powerOnOff(TrackPower powerState) {
  debug_println("powerOnOff()");
  wiThrottleProtocol.setTrackPower(powerState);
  trackPower = powerState;
  writeOledSpeed();
}

void powerToggle() {
  debug_println("PowerToggle()");
  if (trackPower==PowerOn) {
    powerOnOff(PowerOff);
  } else {
    powerOnOff(PowerOn);
  }
}

void nextThrottle() {
  debug_print("nextThrottle(): "); 
  int wasThrottle = currentThrottleIndex;
  currentThrottleIndex++;
  if (currentThrottleIndex >= maxThrottles) {
    currentThrottleIndex = 0;
  }
  currentThrottleIndexChar = getMultiThrottleChar(currentThrottleIndex);

  if (currentThrottleIndex!=wasThrottle) {
    writeOledSpeed();
  }
}

void throttle(int throttleIndex) {
  debug_print("throttle(): "); 
  int wasThrottle = currentThrottleIndex;
  currentThrottleIndex = throttleIndex;
  currentThrottleIndexChar = getMultiThrottleChar(currentThrottleIndex);

  if (currentThrottleIndex!=wasThrottle) {
    writeOledSpeed();
  }
}

void changeNumberOfThrottles(bool increase) {
  if (increase) {
    maxThrottles++;
    if (maxThrottles>6) maxThrottles = 6;   /// can't have more than 6
  } else {
    maxThrottles--;
    if (maxThrottles<1) {   /// can't have less than 1
      maxThrottles = 1;
    } else {
      releaseAllLocos(maxThrottles+1);
      if (currentThrottleIndex>=maxThrottles) {
        nextThrottle();
      }
    }
  }
  writeOledSpeed();
}

void batteryShowToggle() {
  debug_println("batteryShowToggle()");
  switch (showBatteryTest) {
    case ICON_ONLY: 
      showBatteryTest = ICON_AND_PERCENT;
      break;
    case ICON_AND_PERCENT: 
      showBatteryTest = NONE;
      break;
    case NONE: 
    default:
      showBatteryTest = ICON_ONLY;
      break;
  }
  writeOledSpeed();
}

void stopThenToggleDirection() {
  if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar)>0) {
    if (currentSpeed[currentThrottleIndex] != 0) {
      // wiThrottleProtocol.setSpeed(currentThrottleIndexChar, 0);
      speedSet(currentThrottleIndex,0);
    } else {
      if (toggleDirectionOnEncoderButtonPressWhenStationary) toggleDirection(currentThrottleIndex);
    }
    currentSpeed[currentThrottleIndex] = 0;
  }
}

void reconnect() {
  clearOledArray(); 
  oledText[0] = appName; oledText[6] = appVersion; 
  oledText[2] = MSG_DISCONNECTED;
  writeOledArray(false, false);
  delay(5000);
  disconnectWitServer();
}

void setLastServerResponseTime(bool force) {
  // debug_print("setLastServerResponseTime "); debug_println((force) ? "True": "False");
  lastServerResponseTime = wiThrottleProtocol.getLastServerResponseTime();
  if ( (lastServerResponseTime==0) || (force) ) lastServerResponseTime = millis() /1000;
  // debug_print("setLastServerResponseTime "); debug_println(lastServerResponseTime);
}

void checkForShutdownOnNoResponse() {
  if (millis()-startWaitForSelection > MAX_HEARTBEAT_PERIOD) {  // default is 4 minutes
      debug_println("Waited too long for a selection. Shutting down");
      deepSleepStart();
  }
}

String getDots(int howMany) {
  //             123456789_123456789_123456789_123456789_123456789_123456789_
  String dots = "............................................................";
  if (howMany>dots.length()) howMany = dots.length();
  return dots.substring(0,howMany);
}

int compareStrings( const void *str1, const void *str2 ) {
    const char *rec1 = *(char**)str1;
    const char *rec2 = *(char**)str2;
    int val = strcmp(rec1, rec2);

    return val;
}

void doStartupCommands() {
      debug_println("doStartupCommands()");
  for(int i=0; i<4; i++) {
    doOneStartupCommand(startupCommands[i]);
  }
}

void doOneStartupCommand(String cmd) {
  if (cmd.length()>0) {
    menuCommandStarted = false;
    menuCommand = "";
    char firstKey = cmd.charAt(0);
    for(int j=0; j<cmd.length(); j++) {
      char jKey = cmd.charAt(j);
      doKeyPress(jKey, true);
      if (firstKey != '*') { 
        doKeyPress(jKey, false);
      }
    }
  }
}

// *********************************************************************************
//  Select functions
// *********************************************************************************

void selectRoster(int selection) {
  debug_print("selectRoster() "); debug_println(selection);

  if ((selection>=0) && (selection < rosterSize)) {
    if ( (dropBeforeAcquire) && (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar)>0) ) {
      wiThrottleProtocol.releaseLocomotive(currentThrottleIndexChar, "*");
    }
    int index = rosterSortedIndex[selection];
    String loco = String(rosterLength[index]) + rosterAddress[index];
    
    // String loco = String(rosterLength[selection]) + rosterAddress[selection];
    debug_print("add Loco: "); debug_println(loco);
    wiThrottleProtocol.addLocomotive(currentThrottleIndexChar, loco);
    wiThrottleProtocol.getDirection(currentThrottleIndexChar, loco);
    wiThrottleProtocol.getSpeed(currentThrottleIndexChar);
    resetFunctionStates(currentThrottleIndex);
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

void selectFunctionList(int selection) {
  debug_print("selectFunctionList() "); debug_println(selection);

  if ((selection>=0) && (selection < MAX_FUNCTIONS)) {
    String function = functionLabels[currentThrottleIndex][selection];
    debug_print("Function Selected: "); debug_println(function);
    doFunction(currentThrottleIndex, selection, true,false);
    functionHasBeenSelected = true;    
    writeOledSpeed();
    // keypadUseType = KEYPAD_USE_OPERATION;   // don't reset it now.  Do so on the release.
  }
}

void selectEditConsistList(int selection) {
  debug_print("selectEditConsistList() "); debug_println(selection);

  if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar) > 1 ) {
    String loco = wiThrottleProtocol.getLocomotiveAtPosition(currentThrottleIndexChar, selection);
    toggleLocoFacing(currentThrottleIndex, loco);
    writeOledSpeed();
    keypadUseType = KEYPAD_USE_OPERATION;
    menuCommandStarted = false;
  }
}

// *********************************************************************************
//  oLED functions
// *********************************************************************************

void setAppnameForOled() {
  oledText[0] = appName; oledText[6] = appVersion; 
}

void receivingServerInfoOled(int index, int maxExpected) {
  debug_print("receivingServerInfoOled(): LastSent: ");
  debug_println(lastReceivingServerDetailsTime);
  if (index < (maxExpected-1) ) {
    if (millis()-lastReceivingServerDetailsTime >= 2000) {  // refresh it every X seconds if needed
      if (broadcastMessageText == "") broadcastMessageText = MSG_RECEIVING_SERVER_DETAILS;
      lastReceivingServerDetailsTime = millis();
      broadcastMessageTime = millis();
      setMenuTextForOled(menu_menu);
      refreshOled();
    } // else do nothing
  } else {
    lastReceivingServerDetailsTime = 0;
    broadcastMessageTime = 0;
    broadcastMessageText = "";
    refreshOled();
  }
}

void setMenuTextForOled(int menuTextIndex) {
  debug_print("setMenuTextForOled(): ");
  debug_println(menuTextIndex);
  oledText[5] = menu_text[menuTextIndex];
  if (broadcastMessageText != "") {
    if (millis()-broadcastMessageTime < 10000) {
      oledText[5] = broadcastMessageText;
    } else {
      broadcastMessageText = "";
      broadcastMessageTime = 0;
    }
  }
}

void refreshOled() {
     debug_print("refreshOled(): ");
     debug_println(lastOledScreen);
  switch (lastOledScreen) {
    case last_oled_screen_speed:
      writeOledSpeed();
      break;
    case last_oled_screen_turnout_list:
      writeOledTurnoutList(lastOledStringParameter, lastOledTurnoutParameter);
      break;
    case last_oled_screen_route_list:
      writeOledRouteList(lastOledStringParameter);
      break;
    case last_oled_screen_function_list:
      writeOledFunctionList(lastOledStringParameter);
      break;
    case last_oled_screen_menu:
      writeOledMenu(lastOledStringParameter, true);
      break;
    case last_oled_screen_extra_submenu:
      writeOledMenu(lastOledStringParameter, false);
      break;
    case last_oled_screen_all_locos:
      writeOledAllLocos(lastOledBoolParameter);
      break;
    case last_oled_screen_edit_consist:
      writeOledEditConsist();
      break;
    case last_oled_screen_direct_commands:
      writeOledDirectCommands();
      break;
  }
}


void writeOledFoundSSids(String soFar) {
  menuIsShowing = true;
  keypadUseType = KEYPAD_USE_SELECT_SSID_FROM_FOUND;
  if (soFar == "") { // nothing entered yet
    clearOledArray();
    for (int i=0; i<5 && i<foundSsidsCount; i++) {
      if (foundSsids[(page*5)+i].length()>0) {
        oledText[i] = String(i) + ": " + foundSsids[(page*5)+i] + "   (" + foundSsidRssis[(page*5)+i] + ")" ;
      }
    }
    oledText[5] = "(" + String(page+1) +  ") " + menu_text[menu_select_ssids_from_found];
    writeOledArray(false, false);
  // } else {
  //   int cmd = menuCommand.substring(0, 1).toInt();
  }
}

void writeOledRoster(String soFar) {
  lastOledScreen = last_oled_screen_roster;
  lastOledStringParameter = soFar;

  menuIsShowing = true;
  keypadUseType = KEYPAD_USE_SELECT_ROSTER;
  if (soFar == "") { // nothing entered yet
    clearOledArray();
    for (int i=0; i<5 && ((page*5)+i<rosterSize); i++) {
      int index = rosterSortedIndex[(page*5)+i];
      if (rosterAddress[index] != 0) {
        oledText[i] = String(i) + ": " + rosterName[index] + " (" + rosterAddress[index] + ")" ;
      }
    }
    oledText[5] = "(" + String(page+1) +  ") " + menu_text[menu_roster];
    writeOledArray(false, false);
  // } else {
  //   int cmd = menuCommand.substring(0, 1).toInt();
  }
}

void writeOledTurnoutList(String soFar, TurnoutAction action) {
  lastOledScreen = last_oled_screen_turnout_list;
  lastOledStringParameter = soFar;
  lastOledTurnoutParameter = action;

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
      j = (i<5) ? i : i+1;
      if (turnoutListUserName[(page*10)+i].length()>0) {
        oledText[j] = String(turnoutListIndex[i]) + ": " + turnoutListUserName[(page*10)+i].substring(0,10);
      }
    }
    oledText[5] = "(" + String(page+1) +  ") " + menu_text[menu_turnout_list];
    writeOledArray(false, false);
  // } else {
  //   int cmd = menuCommand.substring(0, 1).toInt();
  }
}

void writeOledRouteList(String soFar) {
  lastOledScreen = last_oled_screen_route_list;
  lastOledStringParameter = soFar;

  menuIsShowing = true;
  keypadUseType = KEYPAD_USE_SELECT_ROUTES;
  if (soFar == "") { // nothing entered yet
    clearOledArray();
    int j = 0;
    for (int i=0; i<10 && i<routeListSize; i++) {
      j = (i<5) ? i : i+1;
      if (routeListUserName[(page*10)+i].length()>0) {
        oledText[j] = String(routeListIndex[i]) + ": " + routeListUserName[(page*10)+i].substring(0,10);
      }
    }
    oledText[5] =  "(" + String(page+1) +  ") " + menu_text[menu_route_list];
    writeOledArray(false, false);
  // } else {
  //   int cmd = menuCommand.substring(0, 1).toInt();
  }
}

void writeOledFunctionList(String soFar) {
  lastOledScreen = last_oled_screen_function_list;
  lastOledStringParameter = soFar;

  menuIsShowing = true;
  keypadUseType = KEYPAD_USE_SELECT_FUNCTION;
  functionHasBeenSelected = false;
  
  if (soFar == "") { // nothing entered yet
    clearOledArray();
    if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar) > 0 ) {
      int j = 0; int k = 0;
      for (int i=0; i<10; i++) {
        k = (functionPage*10) + i;
        if (k < MAX_FUNCTIONS) {
          j = (i<5) ? i : i+1;
            oledText[j] = String(i) + ": " 
            + ((k<10) ? functionLabels[currentThrottleIndex][k].substring(0,10) : String(k) 
            + "-" + functionLabels[currentThrottleIndex][k].substring(0,7)) ;
            
            if (functionStates[currentThrottleIndex][k]) {
              oledTextInvert[j] = true;
            }
        }
      }
      oledText[5] = "(" + String(functionPage) +  ") " + menu_text[menu_function_list];
    } else {
      oledText[0] = MSG_NO_FUNCTIONS;
      oledText[2] = MSG_THROTTLE_NUMBER + String(currentThrottleIndex+1);
      oledText[3] = MSG_NO_LOCO_SELECTED;
      // oledText[5] = menu_cancel;
      setMenuTextForOled(menu_cancel);
    }
    writeOledArray(false, false);
  // } else {
  //   int cmd = menuCommand.substring(0, 1).toInt();
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
  oledText[0] = MSG_ENTER_PASSWORD;
  oledText[2] = tempSsidPasswordEntered;
  // oledText[5] = menu_enter_ssid_password;
  setMenuTextForOled(menu_enter_ssid_password);
  writeOledArray(false, true);
}

void writeOledMenu(String soFar, bool primeMenu) {
  debug_print("writeOledMenu() : "); debug_print(primeMenu); debug_print(" : "); debug_println(soFar);
  lastOledStringParameter = soFar;

  int offset = 0;
  lastOledScreen = last_oled_screen_menu;
  if (!primeMenu) {  //Extra Menu
    offset = 10;
    lastOledScreen = last_oled_screen_extra_submenu;
  }

  menuIsShowing = true;
  bool drawTopLine = false;
  if ( (soFar == "")
  || ((!primeMenu) && (soFar.length()==1))) { // nothing entered yet
    clearOledArray();
    int j = 0;
    for (int i=1+offset; i<10+offset; i++) {
      j = (i<6+offset) ? i-offset : i+1-offset;
      oledText[j-1] = String(i-offset) + ": " + menuText[i][0];
    }
    oledText[10] = "0: " + menuText[0+offset][0];
    // oledText[5] = menu_cancel;
    setMenuTextForOled(menu_cancel);
    writeOledArray(false, false);
  } else {
    int cmd = menuCommand.substring(0, 1).toInt();

    clearOledArray();

    oledText[0] = ">> " + menuText[cmd][0] +":"; oledText[6] =  menuCommand.substring(1, menuCommand.length());
    oledText[5] = menuText[cmd+offset][1];

    switch (soFar.charAt(0)) {
      case MENU_ITEM_DROP_LOCO: {
            if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar) > 0) {
              writeOledAllLocos(false);
              drawTopLine = true;
            }
          } // fall through
      case MENU_ITEM_FUNCTION:
      case MENU_ITEM_TOGGLE_DIRECTION: {
          if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar) <= 0 ) {
            oledText[2] = MSG_THROTTLE_NUMBER + String(currentThrottleIndex+1);
            oledText[3] = MSG_NO_LOCO_SELECTED;
            // oledText[5] = menu_cancel;
            setMenuTextForOled(menu_cancel);
          } 
          break;
        }
    }

    writeOledArray(false, false, true, drawTopLine);
  }
}

void writeOledAllLocos(bool hideLeadLoco) {
  lastOledScreen = last_oled_screen_all_locos;
  lastOledBoolParameter = hideLeadLoco;

  int startAt = (hideLeadLoco) ? 1 :0;  // for the loco heading menu, we don't want to show the loco 0 (lead) as an option.
  debug_println("writeOledAllLocos(): ");
  String loco;
  int j = 0; int i = 0;
  if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar) > 0) {
    for (int index=0; ((index < wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar)) && (i < 8)); index++) {  //can only show first 8
      j = (i<4) ? i : i+2;
      loco = wiThrottleProtocol.getLocomotiveAtPosition(currentThrottleIndexChar, index);
      if (i>=startAt) {
        oledText[j+1] = String(i) + ": " + loco;
        if (wiThrottleProtocol.getDirection(currentThrottleIndexChar, loco) == Reverse) {
          oledTextInvert[j+1] = true;
        }
      }
      i++;      
    } 
  }
}

void writeOledEditConsist() {
  lastOledScreen = last_oled_screen_edit_consist;

  menuIsShowing = false;
  clearOledArray();
  debug_println("writeOledEditConsist(): ");
  keypadUseType = KEYPAD_USE_EDIT_CONSIST;
  writeOledAllLocos(true);
  oledText[0] = MENU_ITEM_TEXT_TITLE_EDIT_CONSIST;
  oledText[5] = MENU_ITEM_TEXT_MENU_EDIT_CONSIST;
  writeOledArray(false, false);
}

void writeHeartbeatCheck() {
  menuIsShowing = false;
  clearOledArray();
  oledText[0] = MENU_ITEM_TEXT_TITLE_HEARTBEAT;
  if (heartbeatCheckEnabled) {
    oledText[1] = MSG_HEARTBEAT_CHECK_ENABLED; 
  } else {
    oledText[1] = MSG_HEARTBEAT_CHECK_DISABLED; 
  }
  oledText[5] = MENU_ITEM_TEXT_MENU_HEARTBEAT;
  writeOledArray(false, false);
}

void writeOledSpeed() {
  lastOledScreen = last_oled_screen_speed;

  // debug_println("writeOledSpeed() ");
  
  menuIsShowing = false;
  String sLocos = "";
  String sSpeed = "";
  String sDirection = "";
  String sSpaceBetweenLocos = " ";

  bool foundNextThrottle = false;
  String sNextThrottleNo = "";
  String sNextThrottleSpeedAndDirection = "";

  clearOledArray();
  
  bool drawTopLine = false;

  if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar) > 0 ) {
    // oledText[0] = label_locos; oledText[2] = label_speed;
  
    for (int i=0; i < wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar); i++) {
      // sLocos = sLocos + sSpaceBetweenLocos + wiThrottleProtocol.getLocomotiveAtPosition(currentThrottleIndexChar, i);
      sLocos = sLocos + sSpaceBetweenLocos + getDisplayLocoString(currentThrottleIndex, i);
      sSpaceBetweenLocos = CONSIST_SPACE_BETWEEN_LOCOS;
    }
    // sSpeed = String(currentSpeed[currentThrottleIndex]);
    sSpeed = String(getDisplaySpeed(currentThrottleIndex));
    sDirection = (currentDirection[currentThrottleIndex]==Forward) ? DIRECTION_FORWARD_TEXT : DIRECTION_REVERSE_TEXT;

    //find the next Throttle that has any locos selected - if there is one
    if (maxThrottles > 1) {
      int nextThrottleIndex = currentThrottleIndex + 1;

      for (int i = nextThrottleIndex; i<maxThrottles; i++) {
        if (wiThrottleProtocol.getNumberOfLocomotives(getMultiThrottleChar(i)) > 0 ) {
          foundNextThrottle = true;
          nextThrottleIndex = i;
          break;
        }
      }
      if ( (!foundNextThrottle) && (currentThrottleIndex>0) ) {
        for (int i = 0; i<currentThrottleIndex; i++) {
          if (wiThrottleProtocol.getNumberOfLocomotives(getMultiThrottleChar(i)) > 0 ) {
            foundNextThrottle = true;
            nextThrottleIndex = i;
            break;
          }
        }
      }
      if (foundNextThrottle) {
        sNextThrottleNo =  String(nextThrottleIndex+1);
        int speed = getDisplaySpeed(nextThrottleIndex);
        sNextThrottleSpeedAndDirection = String(speed);
        // if (speed>0) {
          if (currentDirection[nextThrottleIndex]==Forward) {
            sNextThrottleSpeedAndDirection = sNextThrottleSpeedAndDirection + DIRECTION_FORWARD_TEXT_SHORT;
          } else {
            sNextThrottleSpeedAndDirection = DIRECTION_REVERSE_TEXT_SHORT + sNextThrottleSpeedAndDirection;
          }
        // }
        // + " " + ((currentDirection[nextThrottleIndex]==Forward) ? DIRECTION_FORWARD_TEXT_SHORT : DIRECTION_REVERSE_TEXT_SHORT);
        sNextThrottleSpeedAndDirection = "     " + sNextThrottleSpeedAndDirection ;
        sNextThrottleSpeedAndDirection = sNextThrottleSpeedAndDirection.substring(sNextThrottleSpeedAndDirection.length()-5);
      }
    }

    oledText[0] = "   "  + sLocos; 
    //oledText[7] = "     " + sDirection;  // old function state format

    drawTopLine = true;

  } else {
    setAppnameForOled();
    oledText[2] = MSG_THROTTLE_NUMBER + String(currentThrottleIndex+1);
    oledText[3] = MSG_NO_LOCO_SELECTED;
    drawTopLine = true;
  }

  if (!hashShowsFunctionsInsteadOfKeyDefs) {
      // oledText[5] = menu_menu;
      setMenuTextForOled(menu_menu);
    } else {
    // oledText[5] = menu_menu_hash_is_functions;
    setMenuTextForOled(menu_menu_hash_is_functions);
  }

  writeOledArray(false, false, false, drawTopLine);

  if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar) > 0 ) {
    writeOledFunctions();

     // throttle number
    u8g2.setDrawColor(0);
    u8g2.drawBox(0,0,12,16);
    u8g2.setDrawColor(1);
    u8g2.setFont(FONT_THROTTLE_NUMBER); // medium
    u8g2.drawStr(2,15, String(currentThrottleIndex+1).c_str());
  }

  writeOledBattery();
  writeOledSpeedStepMultiplier();

  if (trackPower == PowerOn) {
    // u8g2.drawBox(0,41,15,8);
    u8g2.drawRBox(0,40,9,9,1);
    u8g2.setDrawColor(0);
  }
  u8g2.setFont(FONT_GLYPHS);
  // u8g2.drawStr(0, 48, label_track_power.c_str());
  u8g2.drawGlyph(1, 48, glyph_track_power);
  u8g2.setDrawColor(1);

  if (!heartbeatCheckEnabled) {
    u8g2.setFont(FONT_GLYPHS);
    u8g2.drawGlyph(13, 49, glyph_heartbeat_off);
    u8g2.setDrawColor(2);
    u8g2.drawLine(13, 48, 20, 41);
    // u8g2.drawLine(13, 48, 21, 40);
    u8g2.setDrawColor(1);
  }

  // direction
  // needed for new function state format
  u8g2.setFont(FONT_DIRECTION); // medium
  u8g2.drawStr(79,36, sDirection.c_str());

  // speed
  const char *cSpeed = sSpeed.c_str();
  // u8g2.setFont(u8g2_font_inb21_mn); // big
  u8g2.setFont(FONT_SPEED); // big
  int width = u8g2.getStrWidth(cSpeed);
  u8g2.drawStr(22+(55-width),45, cSpeed);

  // speed and direction of next throttle
  if ( (maxThrottles > 1) && (foundNextThrottle) ) {
    u8g2.setFont(FONT_NEXT_THROTTLE);
    u8g2.drawStr(85+34,38, sNextThrottleNo.c_str() );
    u8g2.drawStr(85+12,48, sNextThrottleSpeedAndDirection.c_str() );
  }

  u8g2.sendBuffer();

  // debug_println("writeOledSpeed(): end");
}

void writeOledSpeedStepMultiplier() {
  if (speedStep != currentSpeedStep[currentThrottleIndex]) {
    // oledText[3] = "X " + String(speedStepCurrentMultiplier);
    u8g2.setDrawColor(1);
    u8g2.setFont(FONT_GLYPHS);
    u8g2.drawGlyph(1, 38, glyph_speed_step);
    u8g2.setFont(FONT_DEFAULT);
    // u8g2.drawStr(0, 37, ("X " + String(speedStepCurrentMultiplier)).c_str());
    u8g2.drawStr(9, 37, String(speedStepCurrentMultiplier).c_str());
  }
}

void writeOledBattery() {
  // debug_print("writeOledBattery(): time: "); debug_println(lastBatteryCheckTime);
  if ( (useBatteryTest) && (showBatteryTest!=NONE) && (lastBatteryCheckTime>0)) {
    // debug_println("writeOledBattery(): do it"); 
    //int lastBatteryTestValue = random(0,100);
    u8g2.setFont(FONT_GLYPHS);
    u8g2.setDrawColor(1);
    // int x = 13; int y = 28;
    int x = 120; int y = 11;
    // if (useBatteryPercentAsWellAsIcon) x = 102;
    if (showBatteryTest==ICON_AND_PERCENT) x = 102;
    u8g2.drawStr(x, y, String("Z").c_str());
    if (lastBatteryTestValue>10) u8g2.drawLine(x+1, y-6, x+1, y-3);
    if (lastBatteryTestValue>25) u8g2.drawLine(x+2, y-6, x+2, y-3);
    if (lastBatteryTestValue>50) u8g2.drawLine(x+3, y-6, x+3, y-3);
    if (lastBatteryTestValue>75) u8g2.drawLine(x+4, y-6, x+4, y-3);
    if (lastBatteryTestValue>90) u8g2.drawLine(x+5, y-6, x+5, y-3);
    
    // if (useBatteryPercentAsWellAsIcon) {
    if (showBatteryTest==ICON_AND_PERCENT) {
      // x = 13; y = 36;
      x = 112; y = 10;
      u8g2.setFont(FONT_FUNCTION_INDICATORS);
      if(lastBatteryTestValue<5) {
        u8g2.drawStr(x,y, String("LOW").c_str());
      } else {
        u8g2.drawStr(x,y, String(String(lastBatteryTestValue)+"%").c_str());
      }
    }
  }
}

void writeOledFunctions() {
  lastOledScreen = last_oled_screen_speed;

  debug_println("writeOledFunctions():");
  //  int x = 99;
  // bool anyFunctionsActive = false;
   for (int i=0; i < MAX_FUNCTIONS; i++) {
     if (functionStates[currentThrottleIndex][i]) {
      // old function state format
  //     //  debug_print("Fn On "); debug_println(i);
  //     if (i < 12) {
  //     int y = (i+2)*10-8;
  //     if ((i>=4) && (i<8)) { 
  //       x = 109; 
  //       y = (i-2)*10-8;
  //     } else if (i>=8) { 
  //       x = 119; 
  //       y = (i-6)*10-8;
  //     }
      
  //     u8g2.drawBox(x,y,8,8);
  //     u8g2.setDrawColor(0);
  //     u8g2.setFont(u8g2_font_profont10_tf);
  //     u8g2.drawStr( x+2, y+7, String( (i<10) ? i : i-10 ).c_str());
  //     u8g2.setDrawColor(1);
  //   //  } else {
  //   //    debug_print("Fn Off "); debug_println(i);

      // new function state format
      // anyFunctionsActive = true;
      // u8g2.drawBox(i*4+12,12,5,7);
      u8g2.drawRBox(i*4+12,12+1,5,7,2);
      u8g2.setDrawColor(0);
      u8g2.setFont(FONT_FUNCTION_INDICATORS);   
      u8g2.drawUTF8( i*4+1+12, 18+1, String( (i<10) ? i : ((i<20) ? i-10 : i-20)).c_str());
      u8g2.setDrawColor(1);
     }
    //  if (anyFunctionsActive) {
    //     u8g2.drawStr( 0, 18, (function_states).c_str());
    // //     u8g2.drawHLine(0,19,128);
    //  }
   }
  debug_println("writeOledFunctions(): end");
}

void writeOledArray(bool isThreeColums, bool isPassword) {
  writeOledArray(isThreeColums, isPassword, true, false);
}

void writeOledArray(bool isThreeColums, bool isPassword, bool sendBuffer) {
  writeOledArray(isThreeColums, isPassword, sendBuffer, false);
}

void writeOledArray(bool isThreeColums, bool isPassword, bool sendBuffer, bool drawTopLine) {
  // debug_println("Start writeOledArray()");
  u8g2.clearBuffer();					// clear the internal memory

  u8g2.setFont(FONT_DEFAULT); // small
  
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
    if ((isPassword) && (i==2)) u8g2.setFont(FONT_PASSWORD); 

    if (oledTextInvert[i]) {
      u8g2.drawBox(x,y-8,62,10);
      u8g2.setDrawColor(0);
    }
    u8g2.drawUTF8(x,y, cLine1);
    u8g2.setDrawColor(1);

    if ((isPassword) && (i==2)) u8g2.setFont(FONT_DEFAULT); 
    y = y + 10;
    if ((i==5) || (i==11)) {
      x = x + xInc;
      y = 10;
    }
  }

  if (drawTopLine) {
    u8g2.drawHLine(0,11,128);
    writeOledBattery();
  }
  u8g2.drawHLine(0,51,128);

  if (sendBuffer) u8g2.sendBuffer();					// transfer internal memory to the display
  // debug_println("writeOledArray(): end ");
}

void clearOledArray() {
  for (int i=0; i < 15; i++) {
    oledText[i] = "";
    oledTextInvert[i] = false;
  }
}

void writeOledDirectCommands() {
  lastOledScreen = last_oled_screen_direct_commands;

  oledDirectCommandsAreBeingDisplayed = true;
  clearOledArray();
  oledText[0] = DIRECT_COMMAND_LIST;
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
  deepSleepStart(SLEEP_REASON_COMMAND);
}

void deepSleepStart(int shutdownReason) {
  clearOledArray(); 
  setAppnameForOled();
  int delayPeriod = 2000;
  if (shutdownReason==SLEEP_REASON_INACTIVITY) {
    oledText[2] = MSG_AUTO_SLEEP;
    delayPeriod = 10000;
  } else if (shutdownReason==SLEEP_REASON_BATTERY) {
    oledText[2] = MSG_BATTERY_SLEEP;
    delayPeriod = 10000;
  }
  oledText[3] = MSG_START_SLEEP;
  writeOledBattery();
  writeOledArray(false, false, true, true);
  delay(delayPeriod);

  u8g2.setPowerSave(1);
  esp_deep_sleep_start();
}
