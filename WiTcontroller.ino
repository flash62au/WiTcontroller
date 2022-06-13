/**
 * This app turns the ESP32 into a Bluetooth LE keyboard that is intended to act as a dedicated
 * gamepad for the JMRI or any wiThrottle server.

  Instructions:
  - Update WiFi SSIDs and passwords as necessary in config_network.h.
  - Flash the sketch 
 */

#include <WiFi.h>                 // https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi
#include <ESPmDNS.h>              // https://github.com/espressif/arduino-esp32/blob/master/libraries/ESPmDNS
#include <WiThrottleProtocol.h>   // https://github.com/flash62au/WiThrottleProtocol
#include <AiEsp32RotaryEncoder.h> // https://github.com/igorantolic/ai-esp32-rotary-encoder
#include <Keypad.h>               // https://www.arduinolibraries.info/libraries/keypad
#include <U8g2lib.h>
#include <string>

// #include "static.h"   // included in WiTcontroller.h
#include "WiTcontroller.h"
#include "actions.h"
#include "config_network.h"
#include "config_keypad_etc.h"   // keypad, encoder, oled display
#include "config.h"

// WiThrottleProtocol Delegate class
class MyDelegate : public WiThrottleProtocolDelegate {
  
  public:
    void receivedVersion(String version) {    
      Serial.printf("Received Version: %s\n",version); 
    }
    void receivedServerDescription(String description) {
      Serial.print("Received Description: ");
      Serial.println(description);
    }
    void receivedSpeed(int speed) {             // Vnnn
      currentSpeed = speed;
      Serial.print("Received Speed: "); Serial.println(speed); 
    }
    void receivedDirection(Direction dir) {     // R{0,1}
      currentDirection = dir;
      Serial.print("Received Direction: "); Serial.println(dir); 
    }
    void receivedFunctionState(uint8_t func, bool state) { 
      Serial.print("Received Fn: "); Serial.print(func); Serial.print(" State: "); Serial.println( (state) ? "True" : "False" );
    }
    void receivedTrackPower(TrackPower state) { 
      Serial.print("Received TrackPower: "); Serial.println(state);
      trackPower = state;
    }
    void receivedRosterEntries(int size) {
      Serial.print("Received Roster Entries. Size: "); Serial.println(size);
      rosterSize = size;
    }
    void receivedRosterEntry(int index, String name, int address, char length) {
      // Serial.print("Received Roster Entry: "); Serial.print(index); 
      // Serial.print(". name: "); Serial.println(name); 
      // Serial.pr
      // int(" addr: "); Serial.println(address); 
      // Serial.print(" len: "); Serial.println(length);
      if (index < 10) {
        rosterIndex[index] = index; 
        rosterName[index] = name; 
        rosterAddress[index] = address;
        rosterLength[index] = length;
      }
    }
    void receivedTurnoutEntries(int size) {
      Serial.print("Received Turnout Entries. Size: "); Serial.println(size);
      turnoutListSize = size;
    }
    void receivedTurnoutEntry(int index, String sysName, String userName, int state) {
      // Serial.print("Received Turnout Entry: "); Serial.print(index); 
      // Serial.print(". sysName: "); Serial.print(sysName); 
      // Serial.print(" userName: "); Serial.print(userName); 
      // Serial.print(" state: "); Serial.println(state);
    }
    void receivedRouteEntries(int size) {
      Serial.print("Received Route Entries. Size: "); Serial.println(size);
      routeListSize = size;
    }
    void receivedRouteEntry(int index, String sysName, String userName, int state) {
      Serial.print("Received Route Entry: "); Serial.print(index); 
      Serial.print(". sysName: "); Serial.print(sysName); 
      Serial.print(" userName: "); Serial.print(userName); 
      Serial.print(" state: "); Serial.println(state);
    }
};

WiFiClient client;
WiThrottleProtocol wiThrottleProtocol;
MyDelegate myDelegate;
int deviceId = random(1000,9999);

void connectNetwork() {
  for (int i=0; i<10; i++) {
    Serial.println(); 
  } 
  Serial.println("Start - Network Connection "); 

  // connect to the wifi.  loop through the configured SSIDs till we get a connection
  boolean connected = false;
  while (!connected) {
    ssidIndex++;
    // if (index >= 3) index = 0;  // go back to the first and try again
    if (ssidIndex >= maxSsids) ssidIndex = 0;  // go back to the first and try again

    Serial.println("");  Serial.print("Trying SSID number: "); Serial.println(ssidIndex);

    double startTime = millis();
    double nowTime = startTime;

    const char *cSsid = ssids[ssidIndex].c_str();
    const char *cPassword = passwords[ssidIndex].c_str();

    if (cSsid!="") {
      Serial.print("Trying Network "); Serial.println(cSsid);
      clearOledArray(); oledText[0] = ssids[ssidIndex]; oledText[1] =  msg_trying_to_connect;
      writeOledArray(false);

      WiFi.begin(cSsid, cPassword); 
      turnoutPrefix = turnoutPrefixes[1];
      routePrefix = routePrefixes[1];

      while ( (WiFi.status() != WL_CONNECTED) && ((nowTime-startTime) <= 10000) ) { // try for 10 seconds
        delay(250);
        Serial.print(".");
        nowTime = millis();
      }

      Serial.println("");
      if (WiFi.status() == WL_CONNECTED) {
        Serial.print("Connected. IP address: "); Serial.println(WiFi.localIP());
        oledText[1] = msg_connected; 
        oledText[2] = msg_address_label + String(WiFi.localIP());
        writeOledArray(false);
        connected = true;
      } else {
        WiFi.disconnect();
      }
    }
  }

  // setup the bonjour listener
  if (!MDNS.begin("ESP32_Browser")) {
    Serial.println("Error setting up MDNS responder!");
    while(1){
      delay(1000);
    }
  }
}

void witService() {
  keypadUseType = KEYPAD_USE_SELECT_WITHROTTLE_SERVER;

  if (witConnectionState == WIT_CONNECTION_STATE_DISCONNECTED) {
    browseWitService(); 
  }
  
  if (witConnectionState == WIT_CONNECTION_STATE_SELECTED) {
    connectWitServer();
  }
}

void browseWitService(){
  Serial.println("browseWitService()");

  const char * service = "withrottle";
  const char * proto= "tcp";

  Serial.printf("Browsing for service _%s._%s.local. on %s ... ", service, proto, ssids[ssidIndex]);
  clearOledArray(); oledText[0] = ssids[ssidIndex];   oledText[1] = msg_browsing_for_service;
  writeOledArray(false);

  noOfWitServices = MDNS.queryService(service, proto);
  if (noOfWitServices == 0) {
    oledText[1] = msg_no_services_found;
    writeOledArray(false);
    Serial.println(oledText[1]);
  
  } else {
    Serial.print(noOfWitServices);  Serial.println(msg_services_found);
    clearOledArray(); oledText[0] = msg_services_found;

    for (int i = 0; i < noOfWitServices; ++i) {
      // Print details for each service found
      Serial.print("  "); Serial.print(i+1); Serial.print(": "); Serial.print(MDNS.hostname(i));
      Serial.print(" ("); Serial.print(MDNS.IP(i)); Serial.print(":"); Serial.print(MDNS.port(i)); Serial.println(")");
      if (i<5) {  // only have room for 5
        oledText[i] = String(i+1) + ": " + MDNS.IP(i).toString() + ":" + String(MDNS.port(i));
      }
    }

    if (noOfWitServices > 0) {
      oledText[11] = msg_select_wit_service;
    }
    writeOledArray(false);

    if (noOfWitServices == 1) {
      selectedWitServerIP = MDNS.IP(0);
      selectedWitServerPort = MDNS.port(0);
      witConnectionState = WIT_CONNECTION_STATE_SELECTED;
    } else {
      witConnectionState = WIT_CONNECTION_STATE_SELECTION_REQUIRED;
    }
  }
}

void selectWitServer(int selection) {
  Serial.print("selectWitServer() "); Serial.println(selection);

  int correctedCollection = selection - 1; 
  if ((correctedCollection>=0) && (correctedCollection < noOfWitServices)) {
    witConnectionState = WIT_CONNECTION_STATE_SELECTED;
    selectedWitServerIP = MDNS.IP(correctedCollection);
    selectedWitServerPort = MDNS.port(correctedCollection);
    keypadUseType = KEYPAD_USE_OPERATION;
  }
}

void connectWitServer() {
  // Pass the delegate instance to wiThrottleProtocol
  wiThrottleProtocol.setDelegate(&myDelegate);
  // Uncomment for logging on Serial
  wiThrottleProtocol.setLogStream(&Serial);

  Serial.println("Connecting to the server...");
  clearOledArray(); oledText[0] = selectedWitServerIP.toString() + " " + String(selectedWitServerPort); oledText[1] + "connecting...";
  writeOledArray(false);

  if (!client.connect(selectedWitServerIP, selectedWitServerPort)) {
    Serial.println(msg_connection_failed);
    oledText[1] = msg_connection_failed;
    writeOledArray(false);
    while(1) delay(1000);
  }
  Serial.print("Connected to server: ");   Serial.println(selectedWitServerIP); Serial.println(selectedWitServerPort);

  // Pass the communication to WiThrottle
  wiThrottleProtocol.connect(&client);
  Serial.println("WiThrottle connected");

  wiThrottleProtocol.setDeviceName(deviceName);  
  wiThrottleProtocol.setDeviceID(String(deviceId));  

  witConnectionState = WIT_CONNECTION_STATE_CONNECTED;

  oledText[1] = msg_connected;
  oledText[11] = menu_menu;
  writeOledArray(false);

  keypadUseType = KEYPAD_USE_OPERATION;
}

void disconnectWitServer() {
  releaseAllLocos();
  wiThrottleProtocol.disconnect();
  Serial.println("Disconnected from wiThrottle server\n");
  clearOledArray(); oledText[0] = msg_disconnected;
  writeOledArray(false);
  witConnectionState = WIT_CONNECTION_STATE_DISCONNECTED;
}

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
void IRAM_ATTR readEncoderISR() {
  rotaryEncoder.readEncoder_ISR();
}

void rotary_onButtonClick() {
  static unsigned long lastTimePressed = 0;
  if (millis() - lastTimePressed < 500) {   //ignore multiple press in that time milliseconds
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
  Serial.println("encoder button pressed");
  writeOledSpeed();
}

void rotary_loop() {
  if (rotaryEncoder.encoderChanged()) {   //don't print anything unless value changed
    encoderValue = rotaryEncoder.readEncoder();
     Serial.print("Encoder From: "); Serial.print(lastEncoderValue);  Serial.print(" to: "); Serial.println(encoderValue);
    if (abs(encoderValue-lastEncoderValue) > 800) { // must have passed through zero
      if (encoderValue > 800) {
        lastEncoderValue = 1001; 
      } else {
        lastEncoderValue = 0; 
      }
      Serial.print("Corrected Encoder From: "); Serial.print(lastEncoderValue); Serial.print(" to: "); Serial.println(encoderValue);
    }
    if (wiThrottleProtocol.getNumberOfLocomotives()>0) {
      if (encoderValue > lastEncoderValue) {
        if (abs(encoderValue-lastEncoderValue)<50) {
          // speedDown(speedStep);
          encoderSpeedChange(true, speedStep);
        } else {
          // speedDown(speedStep*speedStepMultiplier);
          encoderSpeedChange(true, speedStep*speedStepMultiplier);
        }
      } else {
        if (abs(encoderValue-lastEncoderValue)<50) {
          // speedUp(speedStep);
          encoderSpeedChange(false, speedStep);
        } else {
          // speedUp(speedStep*speedStepMultiplier);
          encoderSpeedChange(false, speedStep*speedStepMultiplier);
        }
      } 
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
 
void keypadEvent(KeypadEvent key){
  switch (keypad.getState()){
  case PRESSED:
    Serial.print("Button "); Serial.print(String(key - '0')); Serial.println(" pushed.");
    doKeyPress(key, true);
    break;
  case RELEASED:
    doKeyPress(key, false);
    Serial.print("Button "); Serial.print(String(key - '0')); Serial.println(" released.");
    break;
//    case HOLD:
//        break;
  }
}

void setup() {
  Serial.begin(115200);
  u8g2.begin();

  clearOledArray(); oledText[0] = appName; oledText[1] = appVersion; oledText[2] = msg_start;
  writeOledArray(false);

  delay(1000);
  Serial.println("Start"); 

  rotaryEncoder.begin();  //initialize rotary encoder
  rotaryEncoder.setup(readEncoderISR);
  //set boundaries and if values should cycle or not 
  rotaryEncoder.setBoundaries(0, 1000, circleValues); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
  //rotaryEncoder.disableAcceleration(); //acceleration is now enabled by default - disable if you dont need it
  rotaryEncoder.setAcceleration(100); //or set the value - larger number = more accelearation; 0 or 1 means disabled acceleration

  keypad.addEventListener(keypadEvent); // Add an event listener for this keypad
  keypad.setDebounceTime(keypadDebounceTime);

  connectNetwork();

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13,0); //1 = High, 0 = Low
}

void loop() {
  
  if (witConnectionState != WIT_CONNECTION_STATE_CONNECTED) {
    witService();
  } else {
    wiThrottleProtocol.check();    // parse incoming messages
  }
  char key = keypad.getKey();
  rotary_loop();

  delay(100);
}


void doKeyPress(char key, boolean pressed) {
  if (pressed)  { //pressed
    switch (keypadUseType) {
      case KEYPAD_USE_OPERATION:
        Serial.print("key operation... "); Serial.println(key);
        switch (key) {
          case '*':  // menu command
            menuCommand = "";
            if (menuCommandStarted) { // then cancel the menu
              resetMenu();
              writeOledSpeed();
            } else {
              menuCommandStarted = true;
              Serial.println("Command started");
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

          case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            if (menuCommandStarted) { // appeand to the string
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

      case KEYPAD_USE_SELECT_WITHROTTLE_SERVER:
        Serial.print("key wit... "); Serial.println(key);
        switch (key){
          case '1': case '2': case '3': case '4': case '5':
            selectWitServer(key - '0');
            break;
          default:  // do nothing 
            break;
        }
        break;

      case KEYPAD_USE_SELECT_ROSTER:
        Serial.print("key Roster... "); Serial.println(key);
        switch (key){
          case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            selectRoster(key - '0');
            break;
          
          case '*':  // cancel
          case '#':  // cancel  todo: change to next page
            resetMenu();
            writeOledSpeed();
            break;

          default:  // do nothing 
            break;
        }
        break;
    }

  } // else {  // released
    // do nothing

    // if (keypadUseType == KEYPAD_USE_OPERATION) {
    //   if ( (!menuCommandStarted) && (key>='0') && (key<='D')) { // only process releases for the numeric keys + A,B,C,D and only if a menu command has not be started
    //     Serial.println("Operation - Process key release");
    //     doDirectCommand(key, false);
    //   } else {
    //     Serial.println("Non-Operation - Process key release");
    //     if (keypadUseNonOperationComplete) {   //finished processing the menu etc.
    //       resetMenu();
    //     }
    //   }
    // } // else {  // keypadUser type = KEYPAD_USE_SELECT_WITHROTTLE_SERVER
    // }
  // }
}

void doDirectCommand (char key, boolean pressed) {
  Serial.print("Direct command: "); Serial.println(key);
  int buttonAction = buttonActions[(key - '0')];
  if (buttonAction!=FUNCTION_NULL) {
    if ( (buttonAction>=FUNCTION_0) && (buttonAction<=FUNCTION_28) ) {
      doFunction(buttonAction, pressed);

    } else {
      if (pressed) { // only process these on the key press, not the release
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
            case SPEED_UP: {
              speedUp(speedStep);
              break; 
            }
            case SPEED_DOWN: {
              speedDown(speedStep);
              break; 
            }
            case SPEED_UP_FAST: {
              speedUp(speedStep*speedStepMultiplier);
              break; 
            }
            case SPEED_DOWN_FAST: {
              speedUp(speedStep*speedStepMultiplier);
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
    }
  }
}

void doMenu() {
  String loco = "";
  String function = "";
  boolean result = false;
  Serial.print("Menu: "); Serial.println(menuCommand);

  switch (menuCommand[0]) {
    case '1': { // select loco
        if (menuCommand.length()>1) {
          loco = menuCommand.substring(1, menuCommand.length());
          loco = getLocoWithLength(loco);
          Serial.print("add Loco: "); Serial.println(loco);
          wiThrottleProtocol.addLocomotive(loco);
          writeOledSpeed();
        } else {
          writeOledRoster("");
        }
        break;
      }
    case '2': { // de-select loco
        loco = menuCommand.substring(1, menuCommand.length());
        if (loco!="") { // a loco is specified
          loco = getLocoWithLength(loco);
          Serial.print("release Loco: "); Serial.println(loco);
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
    //  case '4': { // 
    //     break;
    //   }
   case '5': {  // throw turnout
        String turnout = turnoutPrefix + menuCommand.substring(1, menuCommand.length());
        if (!turnout.equals("")) { // a turnout is specified
          Serial.print("throw turnout: "); Serial.println(turnout);
          wiThrottleProtocol.setTurnout(turnout, TurnoutThrow);
        }
        break;
      }
    case '6': {  // close turnout
        String turnout = turnoutPrefix + menuCommand.substring(1, menuCommand.length());
        if (!turnout.equals("")) { // a turnout is specified
          Serial.print("close turnout: "); Serial.println(turnout);
          wiThrottleProtocol.setTurnout(turnout, TurnoutClose);
        }
        break;
      }
    case '7': {  // route
        String route = routePrefix + menuCommand.substring(1, menuCommand.length());
        if (!route.equals("")) { // a loco is specified
          Serial.print("route: "); Serial.println(route);
          wiThrottleProtocol.setRoute(route);
        }
        break;
      }
    case '8': {
        powerToggle();
        Serial.println("Power toggle");
        break;
      }
    case '9': { // disconnect/reconnect/sleep
        String subcommand = menuCommand.substring(1, menuCommand.length());
        if (subcommand.equals("")) { // no subcommand is specified   
          if (witConnectionState == WIT_CONNECTION_STATE_CONNECTED) {
            witConnectionState == WIT_CONNECTION_STATE_DISCONNECTED;
            disconnectWitServer();
          } else {
            connectWitServer();
          }
        } else { // subcommand
          if (subcommand.equals("9")) { // sleep
            deepSleepStart();
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
        break;
      }
  }
  menuCommandStarted = result; 
}

void resetMenu() {
  menuCommand = "";
  menuCommandStarted = false;
  if (keypadUseType != KEYPAD_USE_SELECT_WITHROTTLE_SERVER) keypadUseType = KEYPAD_USE_OPERATION; 
 }

String getLocoWithLength(String loco) {
  int locoNo = menuCommand.toInt() + 0;
  if (locoNo <= 127) {
    loco = "S" + loco;
  } else {
    loco = "L" + loco;
  }
  return loco;
}

void speedEstop() {
  wiThrottleProtocol.emergencyStop();
  currentSpeed = 0;
  Serial.println("Speed EStop"); 
  writeOledSpeed();
}

void speedDown(int amt) {
  if (wiThrottleProtocol.getNumberOfLocomotives()>0) {
    int newSpeed = currentSpeed - amt;
    Serial.print("Speed Down: "); Serial.println(amt);
    speedSet(newSpeed);
  }
}

void speedUp(int amt) {
  if (wiThrottleProtocol.getNumberOfLocomotives()>0) {
    int newSpeed = currentSpeed + amt;
    Serial.print("Speed Up: "); Serial.println(amt);
    speedSet(newSpeed);
  }
}

void speedSet(int amt) {
  if (wiThrottleProtocol.getNumberOfLocomotives()>0) {
    int newSpeed = amt;
    if (newSpeed >128) { newSpeed = 126; }
    if (newSpeed <0) { newSpeed = 0; }
    wiThrottleProtocol.setSpeed(newSpeed);
    currentSpeed = newSpeed;
    Serial.print("Speed Set: "); Serial.println(newSpeed);
    writeOledSpeed();
  }
}

void releaseAllLocos() {
  if (wiThrottleProtocol.getNumberOfLocomotives()>0) {
    for(int index=0;index<wiThrottleProtocol.getNumberOfLocomotives();index++) {
      wiThrottleProtocol.releaseLocomotive(wiThrottleProtocol.getLocomotiveAtPosition(index));
      writeOledSpeed();
    } 
  }
}

void toggleDirection() {
  if (wiThrottleProtocol.getNumberOfLocomotives()>0) {
    Direction direction = Reverse;
    if (currentDirection == Reverse) {
      direction = Forward;
    }
    changeDirection(direction);
    writeOledSpeed();
  }
}

void changeDirection(Direction direction) {
  if (wiThrottleProtocol.getNumberOfLocomotives()>0) {
    wiThrottleProtocol.setDirection(direction);
    currentDirection = direction;
    Serial.print("Change direction: "); Serial.println( (direction==Forward) ? "Forward" : "Reverse");
    writeOledSpeed(); 
  }
}

void doFunction(int functionNumber, boolean pressed) {
  if (wiThrottleProtocol.getNumberOfLocomotives()>0) {
    wiThrottleProtocol.setFunction(functionNumber, pressed );
    Serial.print("fn: "); Serial.print(functionNumber); Serial.println( (pressed) ? " Pressed" : " Released");
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

void selectRoster(int selection) {
  Serial.print("selectRoster() "); Serial.println(selection);

  if ((selection>=0) && (selection < rosterSize)) {
    String loco = String(rosterLength[selection]) + rosterAddress[selection];
    Serial.print("add Loco: "); Serial.println(loco);
    wiThrottleProtocol.addLocomotive(loco);
    writeOledSpeed();
    keypadUseType = KEYPAD_USE_OPERATION;
  }
}

void writeOledRoster(String soFar) {
  keypadUseType = KEYPAD_USE_SELECT_ROSTER;
  if (soFar == "") { // nothing entered yet
    clearOledArray();
    int j = 0;
    for (int i=0; i<10 && i<rosterSize; i++) {
      j = (i<5) ? j=i : j = i+1;
      oledText[j] = String(rosterIndex[i]) + "." + rosterName[i].substring(0,10);
    }
    oledText[5] = "*.Cancel";
    oledText[11] = "0-9.Select";
    writeOledArray(false);
  } else {
    int cmd = menuCommand.substring(0, 1).toInt();
  }
}

void writeOledMenu(String soFar) {
  if (soFar == "") { // nothing entered yet
    clearOledArray();
    int j = 0;
    for (int i=1; i<10; i++) {
      j = (i<6) ? j=i : j = i+1;
      oledText[j-1] = String(i) + "." + menuText[i][0];
    }
    oledText[10] = "0." + menuText[0][0];
    oledText[11] = menu_cancel;
    writeOledArray(false);
  } else {
    int cmd = menuCommand.substring(0, 1).toInt();
    String subMenuText1 = (menuText[cmd][1]=="") ? menu_finish : menuText[cmd][1];
    String subMenuText2 = (menuText[cmd][2]=="") ? "" : menuText[cmd][2];

    clearOledArray();
    oledText[0] = "Menu: " + menuText[cmd][0]; oledText[1] =  menuCommand.substring(1, menuCommand.length());
    oledText[4] = subMenuText2; oledText[5] = subMenuText1; oledText[11] = menu_cancel;
    writeOledArray(false);
  }
}

void writeOledSpeed() {
  String sLocos = "";
  String sSpeed = "";
  String sDirection = "";

  if (wiThrottleProtocol.getNumberOfLocomotives() > 0 ) {
    for (int i=0; i < wiThrottleProtocol.getNumberOfLocomotives(); i++) {
      sLocos = sLocos + " " + wiThrottleProtocol.getLocomotiveAtPosition(i);
    }
    sSpeed = String(currentSpeed);
    sDirection = (currentDirection==Forward) ? direction_forward : direction_reverse;
  }

  clearOledArray();
  // oledText[0] = msg_locos_label; oledText[1] = sLocos; oledText[2] = msg_speed_label + sSpeed; oledText[6] = sDirection; oledText[11] = menu_menu;
  oledText[0] = msg_locos_label; oledText[1] = sLocos; oledText[2] = msg_speed_label; oledText[6] = sDirection; oledText[11] = menu_menu;
  writeOledArray(false, false);

  const char *cSpeed = sSpeed.c_str();
  u8g2.setFont(u8g2_font_inb21_mn); // big
  u8g2.drawStr(35,45, cSpeed);
  u8g2.sendBuffer();
}

void writeOledArray(boolean isThreeColums) {
  writeOledArray(isThreeColums, true);
}

void writeOledArray(boolean isThreeColums, boolean sendBuffer) {

  u8g2.clearBuffer();					// clear the internal memory

  // u8g2.setFont(u8g2_font_ncenB08_tr);	// serif bold
  // u8g2.setFont(u8g2_font_helvB08_te);	// helv bold
  // u8g2.setFont(u8g2_font_helvB08_tf);	// helv
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
    u8g2.drawStr(x,y, cLine1);
    y = y + 10;
    if ((i==5) || (i==11)) {
      x=x+xInc;
      y=10;
    }
  }

  u8g2.drawHLine(0,51,128);

  if (sendBuffer) u8g2.sendBuffer();					// transfer internal memory to the display
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
  writeOledArray(true);
}

void deepSleepStart() {
  u8g2.setPowerSave(1);
  esp_deep_sleep_start();
}