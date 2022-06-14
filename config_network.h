// enter the SSIDs and passwords of as many networks you wish to try to connect to.
// It will try for 10 seconds to connect before moving to the next.  
// It will cycle back to the begining if all fail
const int maxSsids = 4;

// There must be the same number of enties (specified above) in each of the following arrays


const String ssids[maxSsids] = {"Network1", "Network2", "Network3", "Network4"};
const String passwords[maxSsids] = {"password1", "password2", "password3", "password4"};

const String turnoutPrefixes[maxSsids] = {"NT", "NT", "NT", "NT"};  // required if you wish to use turnouts  
// this the prefix of all turnout system names for YOUR, for three wiThrottle servers on the three networks above

const String routePrefixes[maxSsids] = {"IO:AUTO:", "IO:AUTO:", "IO:AUTO:", "IO:AUTO:"};  // required if you wish to use routes  
// this is the prefix of all route system names for YOUR system, for three wiThrottle servers on the three networks above
