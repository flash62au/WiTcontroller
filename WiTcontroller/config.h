#include "actions.h"

const char* deviceName = "WiT Controller";

// enter the SSIDs and passwords of up to three networks you wish to try to connect to.
// It will try for 10 seconds to connect before moving to the next.
// leave the values blank/empty if you dont all three.  ssid0 must not be blank
const char* ssid0     = "MyWifi1";
const char* password0 = "mywifi1 pasword";
const char* ssid1     = "MyWifi2";
const char* password1 = "MyWifi2 password";
const char* ssid2     = "MyWifi3";
const char* password2 = "MyWifi3 password";

const String turnoutPrefixes[3] = {"NT", "NT", "NT"};  // required if you wish to use turnouts  
// this the prefix of all turnout system names for YOUR, for three wiThrottle servers on the three networks above

const String routePrefixes[3] = {"IO:AUTO:", "IO:AUTO:", "IO:AUTO:"};  // required if you wish to use routes  
// this is the prefix of all route system names for YOUR system, for three wiThrottle servers on the three networks above

// configure the keypad buttons to perform the actions you wish
// 4x4 keypad
// int buttonActions[14] = { SPEED_STOP,   // 0
//                          FUNCTION_0,    // 1 - lights
//                          FUNCTION_1,    // 2 - bell
//                          FUNCTION_2,    // 3 - horn
//                          FUNCTION_NULL, // 4
//                          SPEED_UP,      // 5
//                          FUNCTION_NULL, // 6
//                          DIRECTION_REVERSE, // 7
//                          SPEED_DOWN,    // 8
//                          DIRECTION_FORWARD  // 9
//                          FUNCTION_NULL, // A
//                          FUNCTION_NULL, // B
//                          FUNCTION_NULL, // C
//                          FUNCTION_NULL // D
// };
4x3 keypad
int buttonActions[10] = { SPEED_STOP,   // 0
                         FUNCTION_0,    // 1 - lights
                         FUNCTION_1,    // 2 - bell
                         FUNCTION_2,    // 3 - horn
                         FUNCTION_NULL, // 4
                         SPEED_UP,      // 5
                         FUNCTION_NULL, // 6
                         DIRECTION_REVERSE, // 7
                         SPEED_DOWN,    // 8
                         DIRECTION_FORWARD  // 9
};

