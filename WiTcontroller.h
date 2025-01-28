//
// DO NOT alter this file
// 

#define maxFoundWitServers 5     // must be 5 for the moment
#define maxFoundSsids 60     // must be a multiple of 5
#define maxRoster 70     // must be a multiple of 10
#define maxTurnoutList 60     // must be a multiple of 10
#define maxRouteList 60     // must be a multiple of 10


extern int keypadUseType;
extern int encoderUseType;

extern bool menuCommandStarted;
extern String menuCommand;
extern bool menuIsShowing;

extern String oledText[];
extern bool oledTextInvert[];

extern int currentSpeed[];
extern Direction currentDirection[];
extern int speedStepCurrentMultiplier;

extern TrackPower trackPower;
extern String turnoutPrefix;
extern String routePrefix;

extern bool circleValues;
extern int encoderValue;
extern int lastEncoderValue;

extern String selectedSsid;
extern String selectedSsidPassword;
extern int ssidConnectionState;

extern String ssidPasswordEntered;
extern bool ssidPasswordChanged;
extern char ssidPasswordCurrentChar; 

extern IPAddress selectedWitServerIP;
extern int selectedWitServerPort;
extern String selectedWitServerName;
extern int noOfWitServices;
extern int witConnectionState;

extern IPAddress foundWitServersIPs[];
extern int foundWitServersPorts[];
extern String foundWitServersNames[];
extern int foundWitServersCount;

extern String foundSsids[];
extern long foundSsidRssis[];
extern bool foundSsidsOpen[];
extern int foundSsidsCount;
extern int ssidSelectionSource;

extern String witServerIpAndPortConstructed;
extern String witServerIpAndPortEntered;
extern bool witServerIpAndPortChanged;

extern int rosterSize;
extern int rosterIndex[]; 
extern String rosterName[]; 
extern int rosterAddress[];
extern char rosterLength[];

extern int page;

extern int turnoutListSize;
extern int turnoutListIndex[]; 
extern String turnoutListSysName[]; 
extern String turnoutListUserName[];
extern int turnoutListState[];

extern int routeListSize;
extern int routeListIndex[]; 
extern String routeListSysName[]; 
extern String routeListUserName[];
extern int routeListState[];
extern bool functionStates[][MAX_FUNCTIONS];
extern String functionLabels[][MAX_FUNCTIONS];
extern int functionFollow[][MAX_FUNCTIONS];
extern int currentSpeedStep[];
extern int heartBeatPeriod;
extern long lastServerResponseTime;
extern bool heartbeatCheckEnabled;
extern const char* deviceName;
extern const bool encoderRotationClockwiseIsIncreaseSpeed;
extern const bool toggleDirectionOnEncoderButtonPressWhenStationary;
extern int buttonActions[];
extern const String directCommandText[][3];
extern int additionalButtonActions[];

extern long lastSpeedSentTime;
extern int lastSpeedSent;
// extern int lastDirectionSent;
extern int lastSpeedThrottleIndex;

// extern AiEsp32RotaryEncoder rotaryEncoder;

extern int currentThrottleIndex;

// function prototypes

void displayUpdateFromWit(void);
void ssidsLoop(void);
void browseSsids(void);
void selectSsidFromFound(int);
void getSsidPasswordAndWitIpForFound(void);
void enterSsidPassword(void);
void showListOfSsids(void);
void selectSsid(int);
void connectSsid(void);

void witServiceLoop(void);
void browseWitService(void);
void selectWitServer(int);
void connectWitServer(void);
void enterWitServer(void);
void disconnectWitServer(void);
void witEntryAddChar(char);
void witEntryDeleteChar(char);

void ssidPasswordAddChar(char);
void ssidPasswordDeleteChar(char);
void buildWitEntry(void);

void IRAM_ATTR readEncoderISR(void);
void rotary_onButtonClick(void);
void rotary_loop(void);
void encoderSpeedChange(bool, int);
void keypadEvent(KeypadEvent);
void initialiseAdditionalButtons(void);
void additionalButtonLoop(void);

void setup(void);
void loop(void);

void doKeyPress(char, bool);
void doDirectCommand (char, bool);
void doDirectAdditionalButtonCommand (int, bool);
void doDirectAction(int);
void doMenu(void);
void resetMenu(void);

void resetFunctionStates(int);
void resetFunctionLabels(int); 
void resetAllFunctionLabels(void); 
String getLocoWithLength(String);
void speedEstop(void);
void speedDown(int, int);
void speedUp(int, int);
void speedSet(int, int);
void releaseAllLocos(int);
void toggleAdditionalMultiplier(void);
void toggleHeartbeatCheck(void);
void toggleDirection(int);
void changeDirection(int, Direction);

void doDirectFunction(int, bool);
void doDirectFunction(int, bool, bool force);
void doFunction(int, int, bool);
void doFunction(int, int, bool, bool force);

void doFunctionWhichLocosInConsist(int multiThrottleIndex, int functionNumber, bool pressed);
void doFunctionWhichLocosInConsist(int multiThrottleIndex, int functionNumber, bool pressed, bool force);

void powerOnOff(TrackPower);
void powerToggle(void);
void nextThrottle(void);
void reconnect(void);
void setLastServerResponseTime(bool);

void selectRoster(int);
void selectTurnoutList(int, TurnoutAction);
void selectRouteList(int);
void selectFunctionList(int);
void selectEditConsistList(int);

// *********************************************************************************
//  oLED functions
// *********************************************************************************

void setAppnameForOled(void);
void receivingServerInfoOled(int, int);
void setMenuTextForOled(int);
void refreshOled();
void writeOledFoundSSids(String);
void writeOledRoster(String);
void writeOledTurnoutList(String, TurnoutAction);
void writeOledRouteList(String);
void writeOledFunctionList(String);
void writeOledEnterPassword(void);
void writeOledMenu(String, bool);
void writeOledAllLocos(bool);
void writeOledEditConsist();
void writeHeartbeatCheck(void);
void writeOledSpeed(void);
void writeOledSpeedStepMultiplier();
void writeOledBattery();
void writeOledFunctions(void);
void writeOledArray(bool, bool);
void writeOledArray(bool, bool, bool);
void writeOledArray(bool, bool, bool, bool);
void clearOledArray();
void writeOledDirectCommands(void);

void deepSleepStart();
void deepSleepStart(int);

char getMultiThrottleChar(int);
int getMultiThrottleIndex(char);