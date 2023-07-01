#define maxFoundWitServers 5     // must be 5 for the moment
#define maxFoundSsids 60     // must be a multiple of 5
#define maxRoster 60     // must be a multiple of 10
#define maxTurnoutList 60     // must be a multiple of 10
#define maxRouteList 60     // must be a multiple of 10


extern int keypadUseType;
extern int encoderUseType;

extern boolean menuCommandStarted;
extern String menuCommand;
extern boolean menuIsShowing;

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
extern boolean ssidPasswordChanged;
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
extern boolean foundSsidsOpen[];
extern int foundSsidsCount;
extern int ssidSelectionSource;

extern String witServerIpAndPortConstructed;
extern String witServerIpAndPortEntered;
extern boolean witServerIpAndPortChanged;

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
extern boolean functionStates[][28];
extern String functionLabels[][28];
extern int functionFollow[][28];
extern int currentSpeedStep[];
extern int heartBeatPeriod;
extern long lastServerResponseTime;
extern boolean heartbeatCheckEnabled;
extern const char* deviceName;
extern const boolean encoderRotationClockwiseIsIncreaseSpeed;
extern const boolean toggleDirectionOnEncoderButtonPressWhenStationary;
extern int buttonActions[];
extern const String directCommandText[][3];
extern int additionalButtonActions[];

extern long lastSpeedSentTime;
extern int lastSpeedSent;
// extern int lastDirectionSent;
extern int lastSpeedThrottleIndex;

extern AiEsp32RotaryEncoder rotaryEncoder;

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
void encoderSpeedChange(boolean, int);
void keypadEvent(KeypadEvent);
void initialiseAdditionalButtons(void);
void additionalButtonLoop(void);

void setup(void);
void loop(void);

void doKeyPress(char, boolean);
void doDirectCommand (char, boolean);
void doDirectAdditionalButtonCommand (int, boolean);
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

void doDirectFunction(int, boolean);
void doFunction(int, int, boolean);

void powerOnOff(TrackPower);
void powerToggle(void);
void nextThrottle(void);
void reconnect(void);
void setLastServerResponseTime(boolean);

void selectRoster(int);
void selectTurnoutList(int, TurnoutAction);
void selectRouteList(int);
void selectFunctionList(int);
void selectEditConsistList(int);

void setAppnameForOled(void);
void writeOledFoundSSids(String);
void writeOledRoster(String);
void writeOledTurnoutList(String, TurnoutAction);
void writeOledRouteList(String);
void writeOledEnterPassword(void);
void writeOledMenu(String);
void writeOledExtraSubMenu();
void writeHeartbeatCheck(void);
void writeOledSpeed(void);
void writeOledFunctions(void);
void writeOledFunctionList(String);
void writeOledAllLocos(void);
void writeOledEditConsist();
void writeOledArray(boolean, boolean);
void writeOledArray(boolean, boolean, boolean);
void writeOledArray(boolean, boolean, boolean, boolean);
void clearOledArray(void);
void writeOledDirectCommands(void);

void deepSleepStart(void);
void deepSleepStart(bool);

char getMultiThrottleChar(int);
int getMultiThrottleIndex(char);