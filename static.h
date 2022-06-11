const String appName = "witController";
const String appVersion = "Version 0.1";
const String msg_start = "Start";

const String msg_browsing_for_service ="Browsing for service";

const String menu_menu = "*.Menu";
const String menu_finish = "#.Finish";
const String menu_cancel = "*.Cancel";
const String menu_show_direct = "#.Show Direct";

const String direct_command_list = "Direct Commands";

const String direction_forward = "Forward";
const String direction_reverse = "Reverse";

const String msg_no_services_found = "No services found";
const String msg_services_found = "Service(s) found";
const String msg_trying_to_connect = "Trying to Connect";
const String msg_connected = "Connected";
const String msg_address_label = "IP address: ";
const String msg_connection_failed = "Connection failed";
const String msg_disconnected = "Disconnected";
const String msg_locos_label = "Locos: ";
const String msg_speed_label = "Speed: ";

const String msg_select_wit_service = "[ Select ]";

#define KEYPAD_USE_OPERATION 0
#define KEYPAD_USE_SELECT_WITHROTTLE_SERVER 1

#define WIT_CONNECTION_STATE_DISCONNECTED 0
#define WIT_CONNECTION_STATE_CONNECTED 1
#define WIT_CONNECTION_STATE_SELECTION_REQUIRED 2
#define WIT_CONNECTION_STATE_SELECTED 3
