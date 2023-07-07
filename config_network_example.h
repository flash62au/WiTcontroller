// enter the SSIDs and passwords of as many networks you wish to try to connect to.
const int maxSsids = 4;

// There must be the same number of entities (specified above) in each of the following arrays

const String ssids[maxSsids] = {"Network1", "Network2", "Network3", "Network4"};
const String passwords[maxSsids] = {"password1", "password2", "password3", "password4"};

const String turnoutPrefixes[maxSsids] = {"NT", "NT", "NT", "NT"};  // required if you wish to use turnouts  
// this the prefix of all turnout system names for YOUR system, for the wiThrottle servers on the the networks above

const String routePrefixes[maxSsids] = {"IO:AUTO:", "IO:AUTO:", "IO:AUTO:", "IO:AUTO:"};  // required if you wish to use routes  
// this is the prefix of all route system names for YOUR system, for the wiThrottle servers on the  networks above

// If defined this must be of the form 999999999999:99999  e.g. "19216800400102560" equals 192.168.004.001:2560
// Each block of the ip address MUST be 3 digit. Use leading zeros if needed to insure 3 digits.
// The port MUST be 5 digit. Use leading zeros if needed to insure 5 digits.
// #define DEFAULT_IP_AND_PORT      "19216800400102560"

// ********************************************************************************************

// The period that the controller waits for a connection in milliseconds. Default is 10 seconds (10000ms)
// If you are having problems connection to you network, try uncommenting the line increasing this
// #define SSID_CONNECTION_TIMEOUT 10000

// ********************************************************************************************

// Autoconnect to first found server (defaul, if not specified is true)
#define AUTO_CONNECT_TO_FIRST_WITHROTTLE_SERVER true