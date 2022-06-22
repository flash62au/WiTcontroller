
int keypadUseType = KEYPAD_USE_OPERATION;

boolean menuCommandStarted = false;
String menuCommand = "";
boolean menuIsShowing = false;

String oledText[18] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};

int currentSpeed = 0;
Direction currentDirection = Forward;

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

IPAddress selectedWitServerIP;
int selectedWitServerPort = 0;
int noOfWitServices = 0;
int witConnectionState = CONNECTION_STATE_DISCONNECTED;

//found ssids
String foundSsids[10];
boolean foundSsidsOpen[10];
int foundSsidsCount = 0;
int ssidSelectionSource;

// wit Server ip entry
String witServerIpAndPortConstructed = "###.###.###.###:#####";
String witServerIpAndPortEntered = "";
boolean witServerIpAndPortChanged = true;

// roster variables
#define maxRoster 50     // must be a multiple of 10
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