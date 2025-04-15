// enter the SSIDs and passwords of as many networks you wish to try to connect to.
const int maxSsids = 4;

// There must be the same number of entities (specified above) in each of the following arrays

const String ssids[maxSsids] = {"Network1", "Network2", "Network3", "Network4"};
const String passwords[maxSsids] = {"password1", "password2", "password3", "password4"};

const String turnoutPrefixes[maxSsids] = {"NT", "NT", "NT", "NT"};  // required if you wish to use turnouts  
// this the prefix of all turnout system names for YOUR system, for the wiThrottle servers on the the networks above
// note: for DCC-EX EX-CommandStations, the corresponing value should be blank (i.e. "")

const String routePrefixes[maxSsids] = {"IO:AUTO:", "IO:AUTO:", "IO:AUTO:", "IO:AUTO:"};  // required if you wish to use routes  
// this is the prefix of all route system names for YOUR system, for the wiThrottle servers on the  networks above

// If defined this must be of the form 999999999999:99999  e.g. "19216800400102560" equals 192.168.004.001:2560
// Each block of the ip address MUST be 3 digit. Use leading zeros if needed to insure 3 digits.
// The port MUST be 5 digit. Use leading zeros if needed to insure 5 digits.
// #define DEFAULT_IP_AND_PORT      "19216800400102560"

// ********************************************************************************************
// WiFi Country Code for the country you are in.  This is used to set the WiFi channel and power level
// for the ESP32.  The default is 01 (World) and should work in most countries.  If you are having 
// problems try change to another code.
// Supported country codes are "01"(world safe mode) "AT","AU","BE","BG","BR",
//               "CA","CH","CN","CY","CZ","DE","DK","EE","ES","FI","FR","GB","GR","HK","HR","HU",
//               "IE","IN","IS","IT","JP","KR","LI","LT","LU","LV","MT","MX","NL","NO","NZ","PL","PT",
//               "RO","SE","SI","SK","TW","US"

// #define USE_WIFI_COUNTRY_CODE true
// #define COUNTRY_CODE "01"

// By default the countrty code of the device you are connecting to can override the country code selected here
// Uncommenting the following line will enforce the country code selected above.
// This is useful if you are using a device that does not have a country code set or is set to a different country code.
// This will also set the power level to the maximum allowed for that country code.
// This is not recommended unless you are sure of the country code of the device you are connecting to.

// #define ENFORCED_COUNTRY_CODE true

// ********************************************************************************************

// The period that the controller waits for a connection in milliseconds. Default is 10 seconds (10000ms)
// If you are having problems connection to you network, try uncommenting the line increasing this
// #define SSID_CONNECTION_TIMEOUT 10000

// ********************************************************************************************

// Autoconnect to first SSID in the list above (default, if not specified is false)
// #define AUTO_CONNECT_TO_FIRST_DEFINED_SERVER true

// Autoconnect to first found server (default, if not specified is true)
// #define AUTO_CONNECT_TO_FIRST_WITHROTTLE_SERVER true

// ********************************************************************************************

// Minimum time spacing in milliseconds for commands sent.  
// Default is 50 
// uncomment and increase this value  if the command station is skipping some commands
// Probably Not advisable to set to more than 500

// #define OUTBOUND_COMMANDS_MINIMUM_DELAY 200

// ********************************************************************************************

// For some reason WifiTrax WFD-30 system don't respond unless the commands are preceeded with CR+LF
// Originally these would be sent if the SSID name contains "wftrx_" or you could override the name
// From version v1.77 the extra CR+LF are sent by default.  This is a new define that allows you to
// override this behaviour if needed.
// 
// Uncomment the following line and set to false to disable this feature

// #define SEND_LEADING_CR_LF_FOR_COMMANDS false
