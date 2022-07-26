
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

// encode variables
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
#define maxFoundWitServers 5     // must be 5 for the moment
IPAddress foundWitServersIPs[maxFoundWitServers];
int foundWitServersPorts[maxFoundWitServers];
String foundWitServersNames[maxFoundWitServers];
int foundWitServersCount = 0;

//found ssids
#define maxFoundSsids 20     // must be a multiple of 5
String foundSsids[maxFoundSsids];
boolean foundSsidsOpen[maxFoundSsids];
int foundSsidsCount = 0;
int ssidSelectionSource;

// wit Server ip entry
String witServerIpAndPortConstructed = "###.###.###.###:#####";
String witServerIpAndPortEntered = "";
boolean witServerIpAndPortChanged = true;

// roster variables
#define maxRoster 60     // must be a multiple of 10
int rosterSize = 0;
int rosterIndex[maxRoster]; 
String rosterName[maxRoster]; 
int rosterAddress[maxRoster];
char rosterLength[maxRoster];

int page = 0;

// turnout variables
#define maxTurnoutList 50     // must be a multiple of 10
int turnoutListSize = 0;
int turnoutListIndex[maxTurnoutList]; 
String turnoutListSysName[maxTurnoutList]; 
String turnoutListUserName[maxTurnoutList];
int turnoutListState[maxTurnoutList];

// route variables
#define maxRouteList 50     // must be a multiple of 10
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

// don't alter the assignments here
// alter them in config_buttons.h

const char* deviceName = DEVICE_NAME;

const boolean encoderRotationClockwiseIsIncreaseSpeed = ENCODER_ROTATION_CLOCKWISE_IS_INCREASE_SPEED;
// false = Counterclockwise  true = clockwise

const boolean toggleDirectionOnEncoderButtonPressWhenStationary = TOGGLE_DIRECTION_ON_ENCODER_BUUTTON_PRESSED_WHEN_STATIONAY;
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
