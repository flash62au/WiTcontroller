/**
 * This app turns the ESP32 into a Bluetooth LE keyboard that is intended to act as a dedicated
 * gamepad for the JMRI or any wiThrottle server.

  Instructions:
  - Update WiFi SSID and password as necessary.
  - Flash the sketch 
 */

#include <WiFi.h>                 // https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi
#include <ESPmDNS.h>              // https://github.com/espressif/arduino-esp32/blob/master/libraries/ESPmDNS
#include <WiThrottleProtocol.h>   // https://github.com/flash62au/WiThrottleProtocol
#include <AiEsp32RotaryEncoder.h> // https://github.com/igorantolic/ai-esp32-rotary-encoder
#include <Keypad.h>               // https://www.arduinolibraries.info/libraries/keypad
#include <U8g2lib.h>
#include <string>

#include "actions.h"
#include "static.h"
#include "config_network.h"
#include "config.h"

int currentSpeed = 0;
Direction currentDirection;
const int speedStep = 4;
const int speedStepMultiplier = 2;  // for 'fast' speed steps
TrackPower trackPower = PowerUnknown;
String turnoutPrefix = "";
String routePrefix = "";

#define MAX_LOCOS     10  // maximum number of locos that can be added to the consist

// large EPS32
// #define ROTARY_ENCODER_A_PIN 33
// #define ROTARY_ENCODER_B_PIN 32
// #define ROTARY_ENCODER_BUTTON_PIN 27

// Small ESP32
#define ROTARY_ENCODER_A_PIN 12
#define ROTARY_ENCODER_B_PIN 14
#define ROTARY_ENCODER_BUTTON_PIN 13

#define ROTARY_ENCODER_VCC_PIN -1 /* 27 put -1 of Rotary encoder Vcc is connected directly to 3,3V; else you can use declared output pin for powering rotary encoder */
#define ROTARY_ENCODER_STEPS 4 //depending on your encoder - try 1,2 or 4 to get expected behaviour

bool circleValues = true;
int encoderValue = 0;
int lastEncoderValue = 0;

// 4x4 keaypad
// #define ROW_NUM     4 // four rows
// #define COLUMN_NUM  4 // four columns
// char keys[ROW_NUM][COLUMN_NUM] = {
//   {'1', '2', '3', 'A'},
//   {'4', '5', '6', 'B'},
//   {'7', '8', '9', 'C'},
//   {'*', '0', '#', 'D'}
// };
// byte pin_rows[ROW_NUM]      = {19, 18, 5, 17}; // connect to the row pins
// byte pin_column[COLUMN_NUM] = {16, 4, 2, 15};   // connect to the column pins

// 4x3 keypad
#define ROW_NUM     4 // four rows
#define COLUMN_NUM  3 // four columns
char keys[ROW_NUM][COLUMN_NUM] = {
 {'1', '2', '3'},
 {'4', '5', '6'},
 {'7', '8', '9'},
 {'*', '0', '#'}
};
byte pin_rows[ROW_NUM]      = {19, 18, 17, 16}; // GIOP19, GIOP18, GIOP5, GIOP17 connect to the row pins
byte pin_column[COLUMN_NUM] = { 4, 0, 2};   // GIOP16, GIOP4, GIOP0 connect to the column pins

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );
const int keypadDebounceTime = 50;   // in miliseconds

int keypadUseType = KEYPAD_USE_OPERATION;

boolean menuCommandStarted = false;
String menuCommand = "";

int ssidIndex = 99;

const String menuText[10][3] = {
  {"Function","",""},
  {"Add Loco","",""},
  {"Drop Loco", "",""},
  {"Toggle Dir", "",""},
  {"","",""},
  {"Throw Point","",""},
  {"Close Point", "", ""},
  {"Route", "",""},
  {"Trk Power" "","",},
  {"Dis/connect","#.Dis/connect","9,#.Sleep"}
};

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>                       // add to include path [Arduino install]\hardware\arduino\avr\libraries\SPI\src
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>                      // add to include path [Arduino install]\hardware\arduino\avr\libraries\Wire\src
#endif

// Please select a contructor line for below depending on your display
// U8g2 Contructor List (Frame Buffer)
// The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
// Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected
// U8G2_SSD1312_128X64_NONAME_F_SW_I2C u8g2(U8G2_MIRROR_VERTICAL, /* clock=*/ 22, /* data=*/ 23, /* reset=*/ U8X8_PIN_NONE);
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 22, /* data=*/ 23, /* reset=*/ U8X8_PIN_NONE);

String oledText[18] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};

// WiThrottleProtocol Delegate class
class MyDelegate : public WiThrottleProtocolDelegate {
  
  public:
    void receivedVersion(String version) {     
      Serial.print("Received version: "); Serial.println(version);  
    }
    void receivedServerDescription(String description) {
      Serial.print("Received description: "); Serial.println(description);
    }
    void receivedSpeed(int speed) {             // Vnnn
      currentSpeed = speed;
      Serial.print("Speed: "); Serial.println(speed); 
    }
    void receivedDirection(Direction dir) {     // R{0,1}
      currentDirection = dir;
    }
    // void receivedFunctionState(uint8_t func, bool state) { 
    //   Serial.print("Received Fn: "); Serial.print(func); Serial.print(" State: "); Serial.println( (state) ? "True" : "False" );
    // }
    void receivedTrackPower(TrackPower state) { 
      trackPower = state;
    }
};

IPAddress selectedWitServerIP;
int selectedWitServerPort = 0;
int noOfWitServices = 0;
int witConnectionState = WIT_CONNECTION_STATE_DISCONNECTED;

WiFiClient client;
WiThrottleProtocol wiThrottleProtocol;
MyDelegate myDelegate;

int deviceId = random(1000,9999);

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
    wiThrottleProtocol.setSpeed(0);
    currentSpeed = 0;
  }
  Serial.println("encoder button pressed");
}

void rotary_loop() {
  if (rotaryEncoder.encoderChanged()) {   //don't print anything unless value changed
    encoderValue = rotaryEncoder.readEncoder();
     Serial.print("Encoder From: "); Serial.print(lastEncoderValue);
     Serial.print(" to: "); Serial.println(encoderValue);
    if (abs(encoderValue-lastEncoderValue) > 800) { // must have passed through zero
      if (encoderValue > 800) {
        lastEncoderValue = 1001; 
      } else {
        lastEncoderValue = 0; 
      }
      Serial.print("Corrected Encoder From: "); Serial.print(lastEncoderValue);
      Serial.print(" to: "); Serial.println(encoderValue);
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
    keypadUseType = KEYPAD_USE_OPERATION;
    wiThrottleProtocol.check();    // parse incoming messages
  }
  char key = keypad.getKey();
  rotary_loop();

  delay(100);
}


void doKeyPress(char key, boolean pressed) {
  if (pressed)  { //pressed
    if (keypadUseType == KEYPAD_USE_OPERATION) {
      switch (key){
        case '*':  // menu command
          menuCommand = "";
          if (menuCommandStarted) { // then cancel the menu
            menuCommandStarted = false; 
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
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
          if (menuCommandStarted) { // appeand to the string
            menuCommand += key;
            writeOledMenu(menuCommand);
          } else{
            doDirectCommand(key, true);
          }
          break;
        default: { //A, B, C, D
          doDirectCommand(key, true);
          break;
        }
      }
    } else {  // keypadUser type = KEYPAD_USE_SELECT_WITHROTTLE_SERVER
        Serial.print("key... "); Serial.println(key);
        switch (key){
          case '0':
          case '1':
          case '2':
          case '3':
          case '4':
            selectWitServer(key - '0');
            break;
          // case '5':
          // case '6':
          // case '7':
          // case '8':
          // case '9':
          default:  // do nothing 
            break;
        }
    }
  } else {  // released
    if (keypadUseType == KEYPAD_USE_OPERATION) {
      if ( (!menuCommandStarted) && (key>='0') && (key<='D')) { // only process releases for the numeric keys + A,B,C,D and only if a menu command has not be started
        Serial.println("Process key release");
        doDirectCommand(key, false);
      }
    } // else {  // keypadUser type = KEYPAD_USE_SELECT_WITHROTTLE_SERVER
      // do nothing
    // }
  }
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
  Serial.print("Menu: "); Serial.println(menuCommand);

  switch (menuCommand[0]) {
    case '1': { // select loco
        if (menuCommand.length()>1) {
          loco = menuCommand.substring(1, menuCommand.length());
          loco = getLocoWithLength(loco);
          Serial.print("add Loco: "); Serial.println(loco);
          wiThrottleProtocol.addLocomotive(loco);
          writeOledSpeed();
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
  menuCommandStarted = false; 
}

void resetMenu() {
  menuCommandStarted = false;
  menuCommand = "";
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

    Serial.println(""); 
    Serial.print("Trying SSID number: "); Serial.println(ssidIndex);

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
  Serial.println("witService()");

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
      if (i<5) {
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
  }
}

void connectWitServer() {
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
  oledText[1] = msg_connected;
  oledText[11] = menu_menu;
  writeOledArray(false);

  // Uncomment for logging on Serial
//    wiThrottleProtocol.setLogStream(&Serial);
  
  // Pass the delegate instance to wiThrottleProtocol
  wiThrottleProtocol.setDelegate(&myDelegate);

  // Pass the communication to WiThrottle
  wiThrottleProtocol.connect(&client);
  Serial.println("WiThrottle connected");
  wiThrottleProtocol.setDeviceName(deviceName);  
  wiThrottleProtocol.setDeviceID(""+deviceId);  

  witConnectionState = WIT_CONNECTION_STATE_CONNECTED;
}

void disconnectWitServer() {
  releaseAllLocos();
  wiThrottleProtocol.disconnect();
  Serial.println("Disconnected from wiThrottle server\n");
  clearOledArray(); oledText[0] = msg_disconnected;
  writeOledArray(false);
  witConnectionState = WIT_CONNECTION_STATE_DISCONNECTED;
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

// char* stringToCharArray(String str, char* ret) {
//   int len = str.length();
//   char ret[len];
//   for(int i = 0; i < len; ++i) {
//     Serial.print(str.substring(i,1));
//       ret[i] = str.substring(i,1).toInt();
//   }
//   Serial.println("");
//   return ret;
// }

void writeOledMenu(String soFar) {
  if (soFar == "") { // nothing entered yet
    clearOledArray();
    for (int i=1; i<10; i++) {
      oledText[i-1] = String(i) + "." + menuText[i][0];
    }
    oledText[9] = "0." + menuText[0][0];
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
  oledText[0] = msg_locos_label; oledText[1] = sLocos; oledText[2] = msg_speed_label + sSpeed; oledText[6] = sDirection; oledText[11] = menu_menu;
  writeOledArray(false);
}

void writeOledArray(boolean isThreeColums) {

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
  u8g2.sendBuffer();					// transfer internal memory to the display
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