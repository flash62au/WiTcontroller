#include <WiThrottleProtocol.h>   // https://github.com/flash62au/WiThrottleProtocol
#include <string>
#include "static.h"

int keypadUseType = KEYPAD_USE_OPERATION;

boolean menuCommandStarted = false;
String menuCommand = "";

int ssidIndex = 99;

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
IPAddress selectedWitServerIP;
int selectedWitServerPort = 0;
int noOfWitServices = 0;
int witConnectionState = WIT_CONNECTION_STATE_DISCONNECTED;

// roster variables
int rosterSize = 0;
int rosterIndex[10]; 
String rosterName[10]; 
int rosterAddress[10];
char rosterLength[10];