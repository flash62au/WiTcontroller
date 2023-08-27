const String appName = "WiTcontroller";
const String appVersion = "     Version 1.29";
const String msg_start = "Start";

const String menu_menu =                   "* Menu                # Key Defs";
const String menu_menu_hash_is_functions = "* Menu                          # Fn";
const String menu_finish =                 "                        # Finish";
const String menu_cancel =                 "* Cancel";
const String menu_show_direct =            "                  # Show Direct";
// const String menu_direct =                 "* Cancel        0-9 Select";
const String menu_roster =                 "* Cancel      0-9      #Pg";
const String menu_turnout_list =           "* Cancel      0-9      #Pg";
const String menu_route_list =             "* Cancel      0-9      #Pg";
const String menu_function_list =          "* Cancel      0-9      #Pg";

const String menu_select_wit_service =      "0-4      # Entry      E.btn OFF";
const String menu_select_wit_entry =        "0-9   * Back  # Go   E.btn OFF";
const String menu_select_ssids =            "0-9     # Search      E.btn OFF";
const String menu_select_ssids_from_found = "0-4    9 List  # Pg   E.btn OFF";

const String menu_enter_ssid_password =     "E Chrs  E.btn Slct  # Go  * Bck";

const String direct_command_list = "Direct Commands";

const String direction_forward = "Fwd";
const String direction_reverse = "Rev";
const String direction_forward_short = ">";
const String direction_reverse_short = "<";
const String direction_reverse_indicator = "'";

// const String function_states = "fn ";

const String msg_browsing_for_service ="Browsing for WiT services";
const String msg_browsing_for_ssids ="Browsing for SSIDs";

const String msg_no_ssids_found =        "No SSIDs found";
const String msg_ssids_listed =          "  SSIDs listed";
const String msg_ssids_found =           "    SSIDs found";
const String msg_bounjour_setup_failed = "Unable to setup Listener";
const String msg_no_services_found =     "No services found";
const String msg_no_services_found_entry_required = "Enter witServer IP:Port";
const String msg_services_found =        " Service(s) found";
const String msg_trying_to_connect =     "Trying to Connect";
const String msg_connected =             "             Connected";
const String msg_connecting =            "             Connecting...";
const String msg_address_label =         "IP address: ";
const String msg_connection_failed =     "Connection failed";
const String msg_disconnected =          "Disconnected";
const String msg_auto_sleep =           "Waited too long for Select";
const String msg_start_sleep =           "Shutting Down.        E.btn ON";
const String msg_throttle_number  =      "          Throttle #";
const String msg_no_loco_selected =      "        No Loco selected";
const String msg_enter_password =        "Enter Password";
const String msg_guessed_ex_cs_wit_server = "'Guessed' EX-CS WiT server";

const String msg_heartbeatCheckEnabled = "Heartbeat Check Enabled";
const String msg_heartbeatCheckDisabled = "Heartbeat Check Disabled";

// const String label_locos = "Locos: ";
// const String label_speed = "Speed: ";
const String label_track_power = "TRK";
// const String label_unknown = "?";
// const String label_on = "On" ;
// const String label_off = "Off";

const int glyph_heartbeat_off = 0x00b7;
const int glyph_track_power = 0x00eb;
const int glyph_speed_step = 0x00d6;
// const int glyph_direction_forward = 0x0070;
// const int glyph_direction_reverse = 0x006d;

#define KEYPAD_USE_OPERATION 0
#define KEYPAD_USE_SELECT_SSID 1
#define KEYPAD_USE_SELECT_SSID_FROM_FOUND 2
#define KEYPAD_USE_SELECT_WITHROTTLE_SERVER 3
#define KEYPAD_USE_ENTER_WITHROTTLE_SERVER 4
#define KEYPAD_USE_SELECT_ROSTER 5
#define KEYPAD_USE_SELECT_TURNOUTS_THROW 6
#define KEYPAD_USE_SELECT_TURNOUTS_CLOSE 7
#define KEYPAD_USE_SELECT_ROUTES 8
#define KEYPAD_USE_SELECT_FUNCTION 9
#define KEYPAD_USE_ENTER_SSID_PASSWORD 10
#define KEYPAD_USE_EDIT_CONSIST 11

#define ENCODER_USE_OPERATION 0
#define ENCODER_USE_SSID_PASSWORD 1

// used for both wit and ssid
#define CONNECTION_STATE_DISCONNECTED 0
#define CONNECTION_STATE_CONNECTED 1
#define CONNECTION_STATE_SELECTION_REQUIRED 2
#define CONNECTION_STATE_ENTRY_REQUIRED 3
#define CONNECTION_STATE_SELECTED 4
#define CONNECTION_STATE_PASSWORD_ENTRY 5
#define CONNECTION_STATE_ENTERED 6

#define SSID_CONNECTION_SOURCE_LIST 0
#define SSID_CONNECTION_SOURCE_BROWSE 1

#define CMD_FUNCTION 0

#define MAX_LOCOS     10  // maximum number of locos that can be added to the consist

#define MENU_ITEM_FUNCTION '0'
#define MENU_ITEM_ADD_LOCO '1'
#define MENU_ITEM_DROP_LOCO '2'
#define MENU_ITEM_TOGGLE_DIRECTION '3'
#define MENU_ITEM_SPEED_STEP_MULTIPLIER '4'
#define MENU_ITEM_THROW_POINT '5'
#define MENU_ITEM_CLOSE_POINT '6'
#define MENU_ITEM_ROUTE '7'
#define MENU_ITEM_TRACK_POWER '8'
#define MENU_ITEM_EXTRAS '9'

const String menuText[12][3] = {
  {"Function",    "no+# Select   * Cancel  # List", ""},   //0
  {"Add Loco",    "addr+# Add  * Cancel  # Roster", ""},   //1
  {"Drop Loco",   "addr+# One   * Cancel    # All", ""},   //2
  {"Toggle Dir",  "# Toggle                      ", ""},   //3
  {"X Speed Step","* Cancel                # Toggle", ""}, //4
  {"Throw Point", "no+# Throw   * Cancel   # List", ""},   //5
  {"Close Point", "no+# Close   * Cancel   # List", ""},   //6
  {"Route",       "no+# Select  * Cancel   # List", ""},   //7
  {"Trk Power",   "* Cancel                # Toggle", ""}, //8 
  {"Extras",      "no Select  * Cancel         ", ""},   //9

  {"Heartbeat",   "* Close                       ", ""},   //10
  {"Edit Consist Facing","no Chng Facing   * Close", ""}    //11
};

const int menuCharsRequired[12] = {  // 0=none effectively a direct command / 1=one used for sub menus / 2=one or more
  2,   //0
  2,   //1
  2,   //2
  0,   //3
  0,   //4
  2,   //5
  2,   //6
  2,   //7
  0,   //8
  1,   //9
  2,   //10
  2   //11
};

const String extraSubMenuText[8] = { 
   "Fnc/Key Tgl",    //0
   "Edt Consist",    //1
   "",               //2
   "Heartbt Tgl",      //3  
   "#Throttles +",   //4
   "#Throttles -",   //5
   "Disconnect",     //6
   "OFF / Sleep" };  //7

#define EXTRA_MENU_CHAR_FUNCTION_KEY_TOGGLE    '0'
#define EXTRA_MENU_CHAR_EDIT_CONSIST           '1'
#define EXTRA_MENU_CHAR_TBA                    '2'
#define EXTRA_MENU_CHAR_HEARTBEAT_TOGGLE       '3'
#define EXTRA_MENU_CHAR_INCREASE_MAX_THROTTLES '4'
#define EXTRA_MENU_CHAR_DECREASE_MAX_THROTTLES '5'
#define EXTRA_MENU_CHAR_DISCONNECT             '6'
#define EXTRA_MENU_CHAR_OFF_SLEEP              '7'
#define EXTRA_MENU_CHAR_OFF_SLEEP_HIDDEN       '9'  // doesn't appera in the menu


// if defined in config_buttons.h these values will be overwritten
//
// speed increase for each click of the encoder 
#ifdef SPEED_STEP
  const int speedStep = SPEED_STEP;
#else
  const int speedStep = 4;
#endif
#ifdef SPEED_STEP_MULTIPLIER
  const int speedStepMultiplier = SPEED_STEP_MULTIPLIER;  // for 'fast' speed steps
#else
  const int speedStepMultiplier = 3;  // for 'fast' speed steps
#endif
// Additional multiplier.  If the multiplier is enabled from the menu, each rotation of the encoder becomes the speedStep * the AdditionalMultiplier
#ifdef SPEED_STEP_MULTIPLIER
  const int speedStepAdditionalMultiplier = SPEED_STEP_MULTIPLIER;
#else
  const int speedStepAdditionalMultiplier = 2;
#endif

#ifdef  DISPLAY_SPEED_AS_PERCENT
  const bool speedDisplayAsPercent = DISPLAY_SPEED_AS_PERCENT;
#else
  const bool speedDisplayAsPercent = false;
#endif

#ifdef  DISPLAY_SPEED_AS_0_TO_28
  const bool speedDisplayAs0to28 = DISPLAY_SPEED_AS_0_TO_28;
#else
  const bool speedDisplayAs0to28 = false;
#endif

String witServerIpAndPortEntryMask = "###.###.###.###:#####";

#ifndef DEFAULT_IP_AND_PORT 
  #define DEFAULT_IP_AND_PORT ""
#endif

#ifndef SSID_CONNECTION_TIMEOUT 
  #define SSID_CONNECTION_TIMEOUT 10000
#endif

const char ssidPasswordBlankChar = 164;

#define FONT_DEFAULT u8g2_font_NokiaSmallPlain_tf
#define FONT_FUNCTION_INDICATORS u8g2_font_tiny_simon_tr
#define FONT_THROTTLE_NUMBER u8g2_font_neuecraft_tr
#define FONT_PASSWORD u8g2_font_9x15_tf
#define FONT_SPEED u8g2_font_profont29_mr
// #define FONT_DIRECTION u8g2_font_profont17_tr
#define FONT_DIRECTION u8g2_font_neuecraft_tr
// #define FONT_TRACK_POWER u8g2_font_profont10_tf
#define FONT_TRACK_POWER u8g2_font_open_iconic_all_1x_t
#define FONT_HEARTBEAT u8g2_font_open_iconic_all_1x_t
#define FONT_SPEED_STEP u8g2_font_open_iconic_all_1x_t
#define FONT_NEXT_THROTTLE u8g2_font_6x12_m_symbols

# define DCC_EX_TURNOUT_PREFIX ""
# define DCC_EX_ROUTE_PREFIX "R"

#define CONSIST_LEAD_LOCO 0
#define CONSIST_ALL_LOCOS 1

#ifndef WITCONTROLLER_DEBUG 
  #define WITCONTROLLER_DEBUG    0
#endif

#ifndef WITHROTTLE_PROTOCOL_DEBUG
  #define WITHROTTLE_PROTOCOL_DEBUG   1
#endif

#ifndef AUTO_CONNECT_TO_FIRST_WITHROTTLE_SERVER
  #define AUTO_CONNECT_TO_FIRST_WITHROTTLE_SERVER true
#endif