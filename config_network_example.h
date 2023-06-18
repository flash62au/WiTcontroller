// enter the SSIDs and passwords of as many networks you wish to try to connect to.
const int maxSsids = 4;

// There must be the same number of entities (specified above) in each of the following arrays

const String ssids[maxSsids] = {"Network1", "Network2", "Network3", "Network4"};
const String passwords[maxSsids] = {"password1", "password2", "password3", "password4"};

const String turnoutPrefixes[maxSsids] = {"NT", "NT", "NT", "NT"};  // required if you wish to use turnouts  
// this the prefix of all turnout system names for YOUR system, for the wiThrottle servers on the the networks above

const String routePrefixes[maxSsids] = {"IO:AUTO:", "IO:AUTO:", "IO:AUTO:", "IO:AUTO:"};  // required if you wish to use routes  
// this is the prefix of all route system names for YOUR system, for the wiThrottle servers on the  networks above

// If defined this must be of the form 999.999.999.999:99999  e.g. 192.168.004.001:2560
// Each block of the ip address MUST be 3 digit. Use leading zeros if needed to insure 3 digits.
// The port can be any number of digits
// #define DEFAULT_IP_AND_PORT "192.168.004.001:2560"