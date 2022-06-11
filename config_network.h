// enter the SSIDs and passwords of as many networks you wish to try to connect to.
// It will try for 10 seconds to connect before moving to the next.  
// It will cycle back to the begining if all fail
const int maxSsids = 3;
const String ssids[maxSsids] = {"Network1", "Network2", "Network3"};
const String passwords[maxSsids] = {"password1", "password2", "password3"};
