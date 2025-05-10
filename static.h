const String appVersion = "v1.92";
#ifndef CUSTOM_APPNAME
   const String appName = "WiTcontroller";
#else
   const String appName = CUSTOM_APPNAME;
#endif
#ifndef DEVICE_NAME
   #define DEVICE_NAME "WiTcontroller"
#endif


#ifndef USE_WIFI_COUNTRY_CODE
   #define USE_COUNTRY_CODE false
#endif
#ifndef COUNTRY_CODE
   #define COUNTRY_CODE "01"
#endif
#ifndef ENFORCED_COUNTRY_CODE
   #define ENFORCED_COUNTRY_CODE false
#endif

#ifndef MENU_TEXT_MENU
   #define MENU_TEXT_MENU                      "* Menu                # Key Defs"
#endif
#ifndef MENU_TEXT_MENU_HASH_IS_FUNCTIONS
   #define MENU_TEXT_MENU_HASH_IS_FUNCTIONS    "* Menu                          # Fn"
#endif
#ifndef MENU_TEXT_FINISH
   #define MENU_TEXT_FINISH                    "                        # Finish"
#endif
#ifndef MENU_TEXT_CANCEL
   #define MENU_TEXT_CANCEL                    "* Cancel"
#endif
#ifndef MENU_TEXT_SHOW_DIRECT
   #define MENU_TEXT_SHOW_DIRECT               "                  # Show Direct"
#endif
#ifndef MENU_TEXT_ROSTER
   #define MENU_TEXT_ROSTER                    "* Cancel      0-9      #Pg"
#endif
#ifndef MENU_TEXT_TURNOUT_LIST
   #define MENU_TEXT_TURNOUT_LIST              "* Cancel      0-9      #Pg"
#endif
#ifndef MENU_TEXT_ROUTE_LIST
   #define MENU_TEXT_ROUTE_LIST                "* Cancel      0-9      #Pg"
#endif
#ifndef MENU_TEXT_FUNCTION_LIST
   #define MENU_TEXT_FUNCTION_LIST             "* Cancel      0-9      #Pg"
#endif
#ifndef MENU_TEXT_SELECT_WIT_SERVICE
   #define MENU_TEXT_SELECT_WIT_SERVICE        "0-4      # Entry      E.btn OFF"
#endif
#ifndef MENU_TEXT_SELECT_WIT_ENTRY
   #define MENU_TEXT_SELECT_WIT_ENTRY          "0-9   * Back  # Go   E.btn OFF"
#endif
#ifndef MENU_TEXT_SELECT_SSIDS
   #define MENU_TEXT_SELECT_SSIDS              "0-9     # Search      E.btn OFF"
#endif
#ifndef MENU_TEXT_SELECT_SSIDS_FROM_FOUND
   #define MENU_TEXT_SELECT_SSIDS_FROM_FOUND   "0-4    9 List  # Pg   E.btn OFF"
#endif
#ifndef MENU_TEXT_ENTER_SSID_PASSWORD
   #define MENU_TEXT_ENTER_SSID_PASSWORD       "E Chrs  E.btn Slct  # Go  * Bck"
#endif

const String menu_text[14] = {
  MENU_TEXT_MENU,
  MENU_TEXT_MENU_HASH_IS_FUNCTIONS,
  MENU_TEXT_FINISH,
  MENU_TEXT_CANCEL,
  MENU_TEXT_SHOW_DIRECT,
  MENU_TEXT_ROSTER,
  MENU_TEXT_TURNOUT_LIST,
  MENU_TEXT_ROUTE_LIST,
  MENU_TEXT_FUNCTION_LIST,
  MENU_TEXT_SELECT_WIT_SERVICE,
  MENU_TEXT_SELECT_WIT_ENTRY,
  MENU_TEXT_SELECT_SSIDS,
  MENU_TEXT_SELECT_SSIDS_FROM_FOUND,
  MENU_TEXT_ENTER_SSID_PASSWORD
};

const int menu_menu =                     0;
const int menu_menu_hash_is_functions =   1;
const int menu_finish =                   2;
const int menu_cancel =                   3;
const int menu_show_direct =              4;
const int menu_roster =                   5;
const int menu_turnout_list =             6;
const int menu_route_list =               7;
const int menu_function_list =            8;
const int menu_select_wit_service =       9;
const int menu_select_wit_entry =        10;
const int menu_select_ssids =            11;
const int menu_select_ssids_from_found = 12;
const int menu_enter_ssid_password =     13;

const int last_oled_screen_speed =            0;
const int last_oled_screen_roster =           1;
const int last_oled_screen_turnout_list =     2;
const int last_oled_screen_route_list =       3;
const int last_oled_screen_function_list =    4;
const int last_oled_screen_menu          =    5;
const int last_oled_screen_extra_submenu =    6;
const int last_oled_screen_all_locos =        7;
const int last_oled_screen_edit_consist =     8;
const int last_oled_screen_direct_commands =  9;

typedef enum ShowBattery {
    NONE = 0,
    ICON_ONLY = 1,
    ICON_AND_PERCENT = 2
} ShowBattery;

#ifndef DIRECT_COMMAND_LIST
  #define DIRECT_COMMAND_LIST            "Direct Commands"
#endif 
#ifndef DIRECTION_FORWARD_TEXT
  #define DIRECTION_FORWARD_TEXT         "Fwd"
#endif 
#ifndef DIRECTION_REVERSE_TEXT
  #define DIRECTION_REVERSE_TEXT         "Rev"
#endif 
#ifndef DIRECTION_FORWARD_TEXT_SHORT
  #define DIRECTION_FORWARD_TEXT_SHORT   ">"
#endif 
#ifndef DIRECTION_REVERSE_TEXT_SHORT
  #define DIRECTION_REVERSE_TEXT_SHORT   "<"
#endif 
#ifndef DIRECTION_REVERSE_INDICATOR
  #define DIRECTION_REVERSE_INDICATOR    "'"
#endif 

// const String function_states = "fn ";

#ifndef MSG_START
  #define MSG_START                     "Start"
#endif
#ifndef MSG_BROWSING_FOR_SERVICE
  #define MSG_BROWSING_FOR_SERVICE      "Browsing for WiT services"
#endif
#ifndef MSG_BROWSING_FOR_SSIDS
  #define MSG_BROWSING_FOR_SSIDS        "Browsing for SSIDs"
#endif
#ifndef MSG_NO_SSIDS_FOUND
  #define MSG_NO_SSIDS_FOUND            "No SSIDs found"
#endif
#ifndef MSG_SSIDS_LISTED
  #define MSG_SSIDS_LISTED              "  SSIDs listed"
#endif
#ifndef MSG_SSIDS_FOUND
  #define MSG_SSIDS_FOUND               "    SSIDs found"
#endif
#ifndef MSG_BOUNJOUR_SETUP_FAILED
  #define MSG_BOUNJOUR_SETUP_FAILED     "Unable to setup Listener"
#endif
#ifndef MSG_NO_SERVICES_FOUND
  #define MSG_NO_SERVICES_FOUND         "No services found"
#endif
#ifndef MSG_NO_SERVICES_FOUND_ENTRY_REQUIRED
  #define MSG_NO_SERVICES_FOUND_ENTRY_REQUIRED "Enter witServer IP:Port"
#endif
#ifndef MSG_SERVICES_FOUND
  #define MSG_SERVICES_FOUND            " Service(s) found"
#endif
#ifndef MSG_TRYING_TO_CONNECT
  #define MSG_TRYING_TO_CONNECT         "Trying to Connect"
#endif
#ifndef MSG_CONNECTED
  #define MSG_CONNECTED                 "             Connected"
#endif
#ifndef MSG_CONNECTING
  #define MSG_CONNECTING                "             Connecting..."
#endif

#ifndef MSG_ADDRESS_LABEL
   #define MSG_ADDRESS_LABEL            "IP address: "
#endif
#ifndef MSG_CONNECTION_FAILED
   #define MSG_CONNECTION_FAILED        "Connection failed"
#endif
#ifndef MSG_DISCONNECTED
   #define MSG_DISCONNECTED             "Disconnected"
#endif
#ifndef MSG_AUTO_SLEEP
   #define MSG_AUTO_SLEEP               "Waited too long for Select"
#endif
#ifndef MSG_BATTERY_SLEEP
   #define MSG_BATTERY_SLEEP            "Battery critically low"
#endif
#ifndef MSG_START_SLEEP
   #define MSG_START_SLEEP              "Shutting Down.        E.btn ON"
#endif
#ifndef MSG_THROTTLE_NUMBER
   #define MSG_THROTTLE_NUMBER          "          Throttle #"
#endif
#ifndef MSG_NO_LOCO_SELECTED
   #define MSG_NO_LOCO_SELECTED         "        No Loco selected"
#endif
#ifndef MSG_ENTER_PASSWORD
   #define MSG_ENTER_PASSWORD           "Enter Password"
#endif
#ifndef MSG_GUESSED_EX_CS_WIT_SERVER
   #define MSG_GUESSED_EX_CS_WIT_SERVER "'Guessed' EX-CS WiT server"
#endif
#ifndef MSG_BYPASS_WIT_SERVER_SEARCH
   #define MSG_BYPASS_WIT_SERVER_SEARCH     "Bypass WiT server search"
#endif
#ifndef MSG_NO_FUNCTIONS
   #define MSG_NO_FUNCTIONS             "Function List - No Functions"
#endif
#ifndef MSG_HEARTBEAT_CHECK_ENABLED
   #define MSG_HEARTBEAT_CHECK_ENABLED  "Heartbeat Check Enabled"
#endif
#ifndef MSG_HEARTBEAT_CHECK_DISABLED
   #define MSG_HEARTBEAT_CHECK_DISABLED "Heartbeat Check Disabled"
#endif
#ifndef MSG_RECEIVING_SERVER_DETAILS
   #define MSG_RECEIVING_SERVER_DETAILS  "Getting data from server"
#endif

const String label_track_power = "TRK";

const int glyph_heartbeat_off = 0x00b7;
const int glyph_track_power = 0x00eb;
const int glyph_speed_step = 0x00d6;
// const int glyph_direction_forward = 0x0070;
// const int glyph_direction_reverse = 0x006d;

#define SLEEP_REASON_COMMAND 0
#define SLEEP_REASON_INACTIVITY 1
#define SLEEP_REASON_BATTERY 2

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

#define MAX_FUNCTIONS 32


#ifndef MENU_ITEM_TEXT_TITLE_FUNCTION
   #define MENU_ITEM_TEXT_TITLE_FUNCTION               "Function"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_ADD_LOCO
   #define MENU_ITEM_TEXT_TITLE_ADD_LOCO               "Add Loco"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_DROP_LOCO
   #define MENU_ITEM_TEXT_TITLE_DROP_LOCO              "Drop Loco"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_TOGGLE_DIRECTION
   #define MENU_ITEM_TEXT_TITLE_TOGGLE_DIRECTION       "Toggle Dir"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_SPEED_STEP_MULTIPLIER
   #define MENU_ITEM_TEXT_TITLE_SPEED_STEP_MULTIPLIER  "X SpeedStep"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_THROW_POINT
   #define MENU_ITEM_TEXT_TITLE_THROW_POINT            "Throw Point"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_CLOSE_POINT
   #define MENU_ITEM_TEXT_TITLE_CLOSE_POINT            "Close Point"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_ROUTE
   #define MENU_ITEM_TEXT_TITLE_ROUTE                  "Route"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_TRACK_POWER
   #define MENU_ITEM_TEXT_TITLE_TRACK_POWER            "Trk Power"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_EXTRAS
   #define MENU_ITEM_TEXT_TITLE_EXTRAS                 "Extras"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_HEARTBEAT
   #define MENU_ITEM_TEXT_TITLE_HEARTBEAT              "Heartbeat"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_EDIT_CONSIST
   #define MENU_ITEM_TEXT_TITLE_EDIT_CONSIST           "Edit Consist Facing"
#endif

#ifndef MENU_ITEM_TEXT_MENU_FUNCTION
   #define MENU_ITEM_TEXT_MENU_FUNCTION               "no+# Select   * Cancel  # List"
#endif
#ifndef MENU_ITEM_TEXT_MENU_ADD_LOCO
   #define MENU_ITEM_TEXT_MENU_ADD_LOCO               "addr+# Add  * Cancel  # Roster"
#endif
#ifndef MENU_ITEM_TEXT_MENU_DROP_LOCO
   #define MENU_ITEM_TEXT_MENU_DROP_LOCO              "addr+# One   * Cancel    # All"
#endif
#ifndef MENU_ITEM_TEXT_MENU_DROP_LOCO_BY_INDEX
   #define MENU_ITEM_TEXT_MENU_DROP_LOCO_BY_INDEX     "no+# One     * Cancel    # All"
#endif
#if DROP_LOCO_BY_INDEX
   #define MENU_ITEM_TEXT_MENU_DROP_LOCO_REAL MENU_ITEM_TEXT_MENU_DROP_LOCO_BY_INDEX
#else
   #define MENU_ITEM_TEXT_MENU_DROP_LOCO_REAL MENU_ITEM_TEXT_MENU_DROP_LOCO
#endif
#ifndef MENU_ITEM_TEXT_MENU_TOGGLE_DIRECTION
   #define MENU_ITEM_TEXT_MENU_TOGGLE_DIRECTION       "# Toggle                      "
#endif
#ifndef MENU_ITEM_TEXT_MENU_SPEED_STEP_MULTIPLIER
   #define MENU_ITEM_TEXT_MENU_SPEED_STEP_MULTIPLIER  "* Cancel                # Toggle"
#endif
#ifndef MENU_ITEM_TEXT_MENU_THROW_POINT
   #define MENU_ITEM_TEXT_MENU_THROW_POINT            "no+# Throw   * Cancel   # List"
#endif
#ifndef MENU_ITEM_TEXT_MENU_CLOSE_POINT
   #define MENU_ITEM_TEXT_MENU_CLOSE_POINT            "no+# Close   * Cancel   # List"
#endif
#ifndef MENU_ITEM_TEXT_MENU_ROUTE
   #define MENU_ITEM_TEXT_MENU_ROUTE                  "no+# Select  * Cancel   # List"
#endif
// #ifndef MENU_ITEM_TEXT_MENU_TRACK_POWER
//    #define MENU_ITEM_TEXT_MENU_TRACK_POWER            "* Cancel                # Toggle"
// #endif
#ifndef MENU_ITEM_TEXT_MENU_EXTRAS
   #define MENU_ITEM_TEXT_MENU_EXTRAS                 "no Select  * Cancel         "
#endif
#ifndef MENU_ITEM_TEXT_MENU_HEARTBEAT
   #define MENU_ITEM_TEXT_MENU_HEARTBEAT              "* Close                       "
#endif
#ifndef MENU_ITEM_TEXT_MENU_EDIT_CONSIST
   #define MENU_ITEM_TEXT_MENU_EDIT_CONSIST           "no Chng Facing   * Close"
#endif
#define MENU_ITEM_TEXT_MENU_NA                     ""   // for items that don't require showing a menu


#ifndef EXTRA_MENU_TEXT_CHAR_FUNCTION_KEY_TOGGLE
   #define EXTRA_MENU_TEXT_CHAR_FUNCTION_KEY_TOGGLE    "Fnc/Key Tgl"
#endif
#ifndef EXTRA_MENU_TEXT_CHAR_EDIT_CONSIST
   #define EXTRA_MENU_TEXT_CHAR_EDIT_CONSIST           "Edt Consist"
#endif
#ifndef EXTRA_MENU_TEXT_CHAR_TBA
   #define EXTRA_MENU_TEXT_CHAR_TBA                    "" 
#endif
#ifndef EXTRA_MENU_TEXT_CHAR_HEARTBEAT_TOGGLE
   #define EXTRA_MENU_TEXT_CHAR_HEARTBEAT_TOGGLE       "Heartbt Tgl"
#endif
#ifndef EXTRA_MENU_TEXT_CHAR_DROP_BEFORE_ACQUIRE_TOGGLE
   #define EXTRA_MENU_TEXT_CHAR_DROP_BEFORE_ACQUIRE_TOGGLE "1 Loco Tgl"
#endif
#ifndef EXTRA_MENU_TEXT_SAVE_CURRENT_LOCOS
   #define EXTRA_MENU_TEXT_SAVE_CURRENT_LOCOS          "Save Locos"
#endif
#ifndef EXTRA_MENU_TEXT_CHAR_INCREASE_MAX_THROTTLES
   #define EXTRA_MENU_TEXT_CHAR_INCREASE_MAX_THROTTLES "#Throttles +"
#endif
#ifndef EXTRA_MENU_TEXT_CHAR_DECREASE_MAX_THROTTLES
   #define EXTRA_MENU_TEXT_CHAR_DECREASE_MAX_THROTTLES "#Throttles -"
#endif
#ifndef EXTRA_MENU_TEXT_CHAR_DISCONNECT
   #define EXTRA_MENU_TEXT_CHAR_DISCONNECT             "Disconnect"
#endif
#ifndef EXTRA_MENU_TEXT_CHAR_OFF_SLEEP
   #define EXTRA_MENU_TEXT_CHAR_OFF_SLEEP              "OFF / Sleep"
#endif

#define MENU_ITEM_TYPE_DIRECT_COMMAND 0
#define MENU_ITEM_TYPE_SELECT_FROM_LIST 1
#define MENU_ITEM_TYPE_SUB_MENU 2
#define MENU_ITEM_TYPE_ONE_OR_MORE_CHARS 3

// menu items

#ifndef USER_DEFINED_MENUS
   #define MENU_ITEM_FUNCTION                   '0'
   #define MENU_ITEM_ADD_LOCO                   '1'
   #define MENU_ITEM_DROP_LOCO                  '2'
   #define MENU_ITEM_TOGGLE_DIRECTION           '3'
   #define MENU_ITEM_SPEED_STEP_MULTIPLIER      '4'
   #define MENU_ITEM_THROW_POINT                '5'
   #define MENU_ITEM_CLOSE_POINT                '6'
   #define MENU_ITEM_ROUTE                      '7'
   #define MENU_ITEM_TRACK_POWER                '8'
   #define MENU_ITEM_EXTRAS                     '9'

   #define MENU_ITEM_FUNCTION_KEY_TOGGLE        'A'
   #define MENU_ITEM_EDIT_CONSIST               'B'
   #define MENU_ITEM_TBA                        'C'
   #define MENU_ITEM_HEARTBEAT_TOGGLE           'D'
   #define MENU_ITEM_INCREASE_MAX_THROTTLES     'E'
   #define MENU_ITEM_DECREASE_MAX_THROTTLES     'F'
   #define MENU_ITEM_DISCONNECT                 'G'
   #define MENU_ITEM_OFF_SLEEP                  'H'
   #define MENU_ITEM_DROP_BEFORE_ACQUIRE_TOGGLE 'I'
   #define MENU_ITEM_SAVE_CURRENT_LOCOS         'J'
#endif

#ifndef USER_DEFINED_MENUS
   // menu item labels, menu to appear at the bottom of the screen
   const String menuText[20][2] = {
      {MENU_ITEM_TEXT_TITLE_FUNCTION,              MENU_ITEM_TEXT_MENU_FUNCTION},      //0
      {MENU_ITEM_TEXT_TITLE_ADD_LOCO,              MENU_ITEM_TEXT_MENU_ADD_LOCO},      //1
      {MENU_ITEM_TEXT_TITLE_DROP_LOCO,             MENU_ITEM_TEXT_MENU_DROP_LOCO_REAL},//2
      {MENU_ITEM_TEXT_TITLE_TOGGLE_DIRECTION,      MENU_ITEM_TEXT_MENU_NA},            //3
      {MENU_ITEM_TEXT_TITLE_SPEED_STEP_MULTIPLIER, MENU_ITEM_TEXT_MENU_NA},            //4
      {MENU_ITEM_TEXT_TITLE_THROW_POINT,           MENU_ITEM_TEXT_MENU_THROW_POINT},   //5
      {MENU_ITEM_TEXT_TITLE_CLOSE_POINT,           MENU_ITEM_TEXT_MENU_CLOSE_POINT},   //6
      {MENU_ITEM_TEXT_TITLE_ROUTE,                 MENU_ITEM_TEXT_MENU_ROUTE},         //7
      {MENU_ITEM_TEXT_TITLE_TRACK_POWER,           MENU_ITEM_TEXT_MENU_NA},            //8 
      {MENU_ITEM_TEXT_TITLE_EXTRAS,                MENU_ITEM_TEXT_MENU_EXTRAS},        //9
      {EXTRA_MENU_TEXT_CHAR_FUNCTION_KEY_TOGGLE,   MENU_ITEM_TEXT_MENU_NA},            // 10 A
      {EXTRA_MENU_TEXT_CHAR_EDIT_CONSIST,          MENU_ITEM_TEXT_MENU_EDIT_CONSIST},  // 11 B
      {EXTRA_MENU_TEXT_CHAR_TBA,                   MENU_ITEM_TEXT_MENU_NA},            // 12 C
      {EXTRA_MENU_TEXT_CHAR_HEARTBEAT_TOGGLE,      MENU_ITEM_TEXT_MENU_NA},            // 13 D
      {EXTRA_MENU_TEXT_CHAR_INCREASE_MAX_THROTTLES,MENU_ITEM_TEXT_MENU_NA},            // 14 E
      {EXTRA_MENU_TEXT_CHAR_DECREASE_MAX_THROTTLES,MENU_ITEM_TEXT_MENU_NA},            // 15 F
      {EXTRA_MENU_TEXT_CHAR_DISCONNECT,            MENU_ITEM_TEXT_MENU_NA},            // 16 G
      {EXTRA_MENU_TEXT_CHAR_OFF_SLEEP,             MENU_ITEM_TEXT_MENU_NA},            // 17 H
      {EXTRA_MENU_TEXT_CHAR_DROP_BEFORE_ACQUIRE_TOGGLE, MENU_ITEM_TEXT_MENU_NA},       // 18 I
      {EXTRA_MENU_TEXT_SAVE_CURRENT_LOCOS,         MENU_ITEM_TEXT_MENU_NA}             // 19 J
   };
#else 
   const String menuText[20][2] = MENU_STRUCTURE;
#endif

#ifndef USER_DEFINED_MENUS
   const int menuCharsRequired[20] = {  
      MENU_ITEM_TYPE_ONE_OR_MORE_CHARS, //0
      MENU_ITEM_TYPE_ONE_OR_MORE_CHARS, //1
      MENU_ITEM_TYPE_ONE_OR_MORE_CHARS, //2
      MENU_ITEM_TYPE_DIRECT_COMMAND,    //3
      MENU_ITEM_TYPE_DIRECT_COMMAND,    //4
      MENU_ITEM_TYPE_ONE_OR_MORE_CHARS, //5
      MENU_ITEM_TYPE_ONE_OR_MORE_CHARS, //6
      MENU_ITEM_TYPE_ONE_OR_MORE_CHARS, //7
      MENU_ITEM_TYPE_DIRECT_COMMAND,    //8
      MENU_ITEM_TYPE_SUB_MENU,          //9
      MENU_ITEM_TYPE_DIRECT_COMMAND,    //10 A
      MENU_ITEM_TYPE_DIRECT_COMMAND,  //11 B
      MENU_ITEM_TYPE_DIRECT_COMMAND,    //12 C
      MENU_ITEM_TYPE_DIRECT_COMMAND,    //13 D
      MENU_ITEM_TYPE_DIRECT_COMMAND,    //14 E
      MENU_ITEM_TYPE_DIRECT_COMMAND,    //15 F
      MENU_ITEM_TYPE_DIRECT_COMMAND,    //16 G
      MENU_ITEM_TYPE_DIRECT_COMMAND,    //17 H
      MENU_ITEM_TYPE_DIRECT_COMMAND,    //18 I
      MENU_ITEM_TYPE_DIRECT_COMMAND     //19 J
   };
#else 
   const int menuCharsRequired[20] = MENU_CHARS_REQUIRED;
#endif

// if defined in config_buttons.h these values will be overwritten
//
#ifndef ENCODER_ROTATION_CLOCKWISE_IS_INCREASE_SPEED
   #define ENCODER_ROTATION_CLOCKWISE_IS_INCREASE_SPEED false // counter clockwise
#endif 
#ifndef TOGGLE_DIRECTION_ON_ENCODER_BUTTON_PRESSED_WHEN_STATIONAY
   #define TOGGLE_DIRECTION_ON_ENCODER_BUTTON_PRESSED_WHEN_STATIONAY true
#endif
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
#ifndef SPEED_STEP_ADDITIONAL_MULTIPLIER
   #define SPEED_STEP_ADDITIONAL_MULTIPLIER 2
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


#ifndef SHORT_DCC_ADDRESS_LIMIT
  #define SHORT_DCC_ADDRESS_LIMIT 127
#endif

// *******************************************************************************************************************

#ifndef FONT_DEFAULT
   #define FONT_DEFAULT u8g2_font_NokiaSmallPlain_te
#endif
#define FONT_FUNCTION_INDICATORS u8g2_font_tiny_simon_tr
#define FONT_THROTTLE_NUMBER u8g2_font_neuecraft_tr
#define FONT_PASSWORD u8g2_font_9x15_tf
#define FONT_SPEED u8g2_font_profont29_mr
#define FONT_DIRECTION u8g2_font_neuecraft_tr
// #define FONT_TRACK_POWER u8g2_font_profont10_tf
#define FONT_NEXT_THROTTLE u8g2_font_6x12_m_symbols
#define FONT_GLYPHS u8g2_font_open_iconic_all_1x_t

// *******************************************************************************************************************

# define DCC_EX_TURNOUT_PREFIX ""
# define DCC_EX_ROUTE_PREFIX "R"

#define CONSIST_LEAD_LOCO 0
#define CONSIST_ALL_LOCOS 1

// *******************************************************************************************************************

#ifndef WITCONTROLLER_DEBUG 
  #define WITCONTROLLER_DEBUG    0
#endif

#ifndef WITHROTTLE_PROTOCOL_DEBUG
  #define WITHROTTLE_PROTOCOL_DEBUG   1
#endif

#ifndef DEBUG_LEVEL
  #define DEBUG_LEVEL   1
#endif

// *******************************************************************************************************************

#ifndef AUTO_CONNECT_TO_FIRST_DEFINED_SERVER
  #define AUTO_CONNECT_TO_FIRST_DEFINED_SERVER false
#endif

#ifndef AUTO_CONNECT_TO_FIRST_WITHROTTLE_SERVER
  #define AUTO_CONNECT_TO_FIRST_WITHROTTLE_SERVER true
#endif

#ifndef OUTBOUND_COMMANDS_MINIMUM_DELAY
  #define OUTBOUND_COMMANDS_MINIMUM_DELAY 50
#endif

#ifndef SEND_LEADING_CR_LF_FOR_COMMANDS
  #define SEND_LEADING_CR_LF_FOR_COMMANDS true
#endif

#ifndef ROTARY_ENCODER_DEBOUNCE_TIME
  #define ROTARY_ENCODER_DEBOUNCE_TIME 200
#endif

// *******************************************************************************************************************

#ifndef F0_LABEL
  #define F0_LABEL "Light"
#endif

#ifndef F1_LABEL
  #define F1_LABEL "Bell"
#endif

#ifndef F2_LABEL
  #define F2_LABEL "Horn"
#endif

// *******************************************************************************************************************

#ifndef ROTARY_ENCODER_DEBOUNCE_TIME
  #define ROTARY_ENCODER_DEBOUNCE_TIME 200
#endif

#ifndef SEARCH_ROSTER_ON_ENTRY_OF_DCC_ADDRESS
  #define SEARCH_ROSTER_ON_ENTRY_OF_DCC_ADDRESS false
#endif

// *******************************************************************************************************************
// if bare EC11 rotary encoder is used rather than KY040 then ESP32 GPIO internal pullups must be enabled. Set default to be false
// as the prototype build used KY040 encoder module that incorporates physical resistors

#ifndef EC11_PULLUPS_REQUIRED
  #define EC11_PULLUPS_REQUIRED false
#endif

// *******************************************************************************************************************
// consists follow functions
#ifndef CONSIST_FUNCTION_FOLLOW_F0
    #define CONSIST_FUNCTION_FOLLOW_F0                  CONSIST_ALL_LOCOS            // lights
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F1
    #define CONSIST_FUNCTION_FOLLOW_F1                  CONSIST_LEAD_LOCO            // bell
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F2
    #define CONSIST_FUNCTION_FOLLOW_F2                  CONSIST_LEAD_LOCO            // horn
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F3
    #define CONSIST_FUNCTION_FOLLOW_F3                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F4
    #define CONSIST_FUNCTION_FOLLOW_F4                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F5
    #define CONSIST_FUNCTION_FOLLOW_F5                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F6
    #define CONSIST_FUNCTION_FOLLOW_F6                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F7
    #define CONSIST_FUNCTION_FOLLOW_F7                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F8
    #define CONSIST_FUNCTION_FOLLOW_F8                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F9
    #define CONSIST_FUNCTION_FOLLOW_F9                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F10
    #define CONSIST_FUNCTION_FOLLOW_F10                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F11
    #define CONSIST_FUNCTION_FOLLOW_F11                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F12
    #define CONSIST_FUNCTION_FOLLOW_F12                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F13
    #define CONSIST_FUNCTION_FOLLOW_F13                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F14
    #define CONSIST_FUNCTION_FOLLOW_F14                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F15
    #define CONSIST_FUNCTION_FOLLOW_F15                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F16
    #define CONSIST_FUNCTION_FOLLOW_F16                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F17
    #define CONSIST_FUNCTION_FOLLOW_F17                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F18
    #define CONSIST_FUNCTION_FOLLOW_F18                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F19
    #define CONSIST_FUNCTION_FOLLOW_F19                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F20
    #define CONSIST_FUNCTION_FOLLOW_F20                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F21
    #define CONSIST_FUNCTION_FOLLOW_F21                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F22
    #define CONSIST_FUNCTION_FOLLOW_F22                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F23
    #define CONSIST_FUNCTION_FOLLOW_F23                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F24
    #define CONSIST_FUNCTION_FOLLOW_F24                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F25
    #define CONSIST_FUNCTION_FOLLOW_F25                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F26
    #define CONSIST_FUNCTION_FOLLOW_F26                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F27
    #define CONSIST_FUNCTION_FOLLOW_F27                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F28
    #define CONSIST_FUNCTION_FOLLOW_F28                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F29
    #define CONSIST_FUNCTION_FOLLOW_F29                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F30
    #define CONSIST_FUNCTION_FOLLOW_F30                  CONSIST_LEAD_LOCO
#endif
#ifndef CONSIST_FUNCTION_FOLLOW_F31
    #define CONSIST_FUNCTION_FOLLOW_F31                  CONSIST_LEAD_LOCO
#endif



// *******************************************************************************************************************
// other options
#ifndef HASH_SHOWS_FUNCTIONS_INSTEAD_OF_KEY_DEFS
    #define HASH_SHOWS_FUNCTIONS_INSTEAD_OF_KEY_DEFS false  // default if not defined in config_buttons.h
#endif

#ifndef MAX_THROTTLES
    #define MAX_THROTTLES 2  // default if not defined in config_buttons.h
#endif

#ifndef ENCODER_BUTTON_ACTION
    #define ENCODER_BUTTON_ACTION SPEED_STOP_THEN_TOGGLE_DIRECTION  // default if not defined in config_buttons.h
#endif

// *******************************************************************************************************************
// OLED

#ifdef U8X8_HAVE_HW_SPI
    #include <SPI.h>                       // add to include path [Arduino install]\hardware\arduino\avr\libraries\SPI\src
#endif
#ifdef U8X8_HAVE_HW_I2C
    #include <Wire.h>                      // add to include path [Arduino install]\hardware\arduino\avr\libraries\Wire\src
#endif

// U8g2 Constructor List (Frame Buffer)
// you can overide this in config_buttons.h     DO NOT CHANGE IT HERE
#ifndef OLED_TYPE
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 22, /* data=*/ 23);
#else
    OLED_TYPE
#endif

// *******************************************************************************************************************
// additional / optional commands
//  these can be any legigitmate WiThrottle protocol command
// refer to https://www.jmri.org/help/en/package/jmri/jmrit/withrottle/Protocol.shtml

#ifndef CUSTOM_COMMAND_1
   #define CUSTOM_COMMAND_1 ""
#endif 
#ifndef CUSTOM_COMMAND_2
   #define CUSTOM_COMMAND_2 ""
#endif 
#ifndef CUSTOM_COMMAND_3
   #define CUSTOM_COMMAND_3 ""
#endif 
#ifndef CUSTOM_COMMAND_4
   #define CUSTOM_COMMAND_4 ""
#endif 
#ifndef CUSTOM_COMMAND_5
   #define CUSTOM_COMMAND_5 ""
#endif 
#ifndef CUSTOM_COMMAND_6
   #define CUSTOM_COMMAND_6 ""
#endif 
#ifndef CUSTOM_COMMAND_7
   #define CUSTOM_COMMAND_7 ""
#endif 

// *******************************************************************************************************************
// default direct functions

// Labels that will appear in the UI
// These should match the actual  command below

#ifndef CHOSEN_KEYPAD_DISPLAY_PREFIX
   #define CHOSEN_KEYPAD_DISPLAY_PREFIX "* Menu"
#endif
#ifndef CHOSEN_KEYPAD_DISPLAY_SUFIX
   #define CHOSEN_KEYPAD_DISPLAY_SUFIX "# This"
#endif

#ifndef CHOSEN_KEYPAD_0_DISPLAY_NAME
   #define CHOSEN_KEYPAD_0_DISPLAY_NAME "0 Lights"
#endif
#ifndef CHOSEN_KEYPAD_1_DISPLAY_NAME
   #define CHOSEN_KEYPAD_1_DISPLAY_NAME "1 Bell"
#endif
#ifndef CHOSEN_KEYPAD_2_DISPLAY_NAME
   #define CHOSEN_KEYPAD_2_DISPLAY_NAME "2 Horn"
#endif
#ifndef CHOSEN_KEYPAD_3_DISPLAY_NAME
   #define CHOSEN_KEYPAD_3_DISPLAY_NAME "3 F3"
#endif
#ifndef CHOSEN_KEYPAD_4_DISPLAY_NAME
   #define CHOSEN_KEYPAD_4_DISPLAY_NAME "4 F4"
#endif
#ifndef CHOSEN_KEYPAD_5_DISPLAY_NAME
   #define CHOSEN_KEYPAD_5_DISPLAY_NAME "5 Nxt Ttl"
#endif
#ifndef CHOSEN_KEYPAD_6_DISPLAY_NAME
   #define CHOSEN_KEYPAD_6_DISPLAY_NAME "6 X Spd"
#endif
#ifndef CHOSEN_KEYPAD_7_DISPLAY_NAME
   #define CHOSEN_KEYPAD_7_DISPLAY_NAME "7 Rev"
#endif
#ifndef CHOSEN_KEYPAD_8_DISPLAY_NAME
   #define CHOSEN_KEYPAD_8_DISPLAY_NAME "8 Estop"
#endif
#ifndef CHOSEN_KEYPAD_9_DISPLAY_NAME
   #define CHOSEN_KEYPAD_9_DISPLAY_NAME "9 Fwd"
#endif

// actual commands

#ifndef CHOSEN_KEYPAD_0_FUNCTION
   #define CHOSEN_KEYPAD_0_FUNCTION     FUNCTION_0
#endif
#ifndef CHOSEN_KEYPAD_1_FUNCTION
   #define CHOSEN_KEYPAD_1_FUNCTION     FUNCTION_1
#endif
#ifndef CHOSEN_KEYPAD_2_FUNCTION
   #define CHOSEN_KEYPAD_2_FUNCTION     FUNCTION_2
#endif
#ifndef CHOSEN_KEYPAD_3_FUNCTION
   #define CHOSEN_KEYPAD_3_FUNCTION     FUNCTION_3
#endif
#ifndef CHOSEN_KEYPAD_4_FUNCTION
   #define CHOSEN_KEYPAD_4_FUNCTION     FUNCTION_4
#endif
#ifndef CHOSEN_KEYPAD_5_FUNCTION
   #define CHOSEN_KEYPAD_5_FUNCTION     NEXT_THROTTLE
#endif
#ifndef CHOSEN_KEYPAD_6_FUNCTION
   #define CHOSEN_KEYPAD_6_FUNCTION     SPEED_MULTIPLIER
#endif
#ifndef CHOSEN_KEYPAD_7_FUNCTION
   #define CHOSEN_KEYPAD_7_FUNCTION     DIRECTION_REVERSE
#endif
#ifndef CHOSEN_KEYPAD_8_FUNCTION
   #define CHOSEN_KEYPAD_8_FUNCTION     E_STOP
#endif
#ifndef CHOSEN_KEYPAD_9_FUNCTION
   #define CHOSEN_KEYPAD_9_FUNCTION     DIRECTION_FORWARD
#endif
#ifndef CHOSEN_KEYPAD_A_FUNCTION
   #define CHOSEN_KEYPAD_A_FUNCTION     CUSTOM_1
#endif
#ifndef CHOSEN_KEYPAD_B_FUNCTION
   #define CHOSEN_KEYPAD_B_FUNCTION     CUSTOM_2
#endif
#ifndef CHOSEN_KEYPAD_C_FUNCTION
   #define CHOSEN_KEYPAD_C_FUNCTION     CUSTOM_3
#endif
#ifndef CHOSEN_KEYPAD_D_FUNCTION
   #define CHOSEN_KEYPAD_D_FUNCTION     CUSTOM_4
#endif


// *******************************************************************************************************************
// additional / optional buttons hardware

// This format for the additional buttons is now depriated
// Use the *New Additional / optional buttons* section instead

// To use the additional buttons, adjust the functions assigned to them in config_buttons.h
#ifndef MAX_ADDITIONAL_BUTTONS
    #define MAX_ADDITIONAL_BUTTONS 7  
#endif
#ifndef ADDITIONAL_BUTTONS_PINS
    #define ADDITIONAL_BUTTONS_PINS      {5,15,25,26,27,32,33}
#endif
#ifndef ADDITIONAL_BUTTONS_TYPE
    #define ADDITIONAL_BUTTONS_TYPE      {INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP}
   // 34,35,36,39 don't have an internal pullup
#endif

#ifndef ADDITIONAL_BUTTON_DEBOUNCE_DELAY
    #define ADDITIONAL_BUTTON_DEBOUNCE_DELAY 50   // default if not defined in config_buttons.h
#endif

// *******************************************************************************************************************

// This format for the additional buttons is now depriated
// Use the *New Additional / optional buttons* section instead

#ifndef CHOSEN_ADDITIONAL_BUTTON_0_FUNCTION
  #define CHOSEN_ADDITIONAL_BUTTON_0_FUNCTION FUNCTION_NULL
#endif
#ifndef CHOSEN_ADDITIONAL_BUTTON_1_FUNCTION
  #define CHOSEN_ADDITIONAL_BUTTON_1_FUNCTION FUNCTION_NULL
#endif
#ifndef CHOSEN_ADDITIONAL_BUTTON_2_FUNCTION
  #define CHOSEN_ADDITIONAL_BUTTON_2_FUNCTION FUNCTION_NULL
#endif
#ifndef CHOSEN_ADDITIONAL_BUTTON_3_FUNCTION
  #define CHOSEN_ADDITIONAL_BUTTON_3_FUNCTION FUNCTION_NULL
#endif
#ifndef CHOSEN_ADDITIONAL_BUTTON_4_FUNCTION
  #define CHOSEN_ADDITIONAL_BUTTON_4_FUNCTION FUNCTION_NULL
#endif
#ifndef CHOSEN_ADDITIONAL_BUTTON_5_FUNCTION
  #define CHOSEN_ADDITIONAL_BUTTON_5_FUNCTION FUNCTION_NULL
#endif
#ifndef CHOSEN_ADDITIONAL_BUTTON_6_FUNCTION
  #define CHOSEN_ADDITIONAL_BUTTON_6_FUNCTION FUNCTION_NULL
#endif

#ifndef ADDITIONAL_BUTTON_OVERRIDE_DEFAULT_LATCHING
   #define ADDITIONAL_BUTTON_OVERRIDE_DEFAULT_LATCHING false
#endif

#ifndef ADDITIONAL_BUTTON_0_LATCHING
   #define ADDITIONAL_BUTTON_0_LATCHING true
#endif
#ifndef ADDITIONAL_BUTTON_1_LATCHING
   #define ADDITIONAL_BUTTON_1_LATCHING true
#endif
#ifndef ADDITIONAL_BUTTON_2_LATCHING
   #define ADDITIONAL_BUTTON_2_LATCHING false
#endif
#ifndef ADDITIONAL_BUTTON_3_LATCHING
   #define ADDITIONAL_BUTTON_3_LATCHING true
#endif
#ifndef ADDITIONAL_BUTTON_4_LATCHING
   #define ADDITIONAL_BUTTON_4_LATCHING true
#endif
#ifndef ADDITIONAL_BUTTON_5_LATCHING
   #define ADDITIONAL_BUTTON_5_LATCHING true
#endif
#ifndef ADDITIONAL_BUTTON_6_LATCHING
   #define ADDITIONAL_BUTTON_6_LATCHING true
#endif

// *******************************************************************************************************************
// New Additional / optional buttons

#ifndef USE_NEW_ADDITIONAL_BUTTONS_FORMAT
   #define USE_NEW_ADDITIONAL_BUTTONS_FORMAT false
#endif

#if USE_NEW_ADDITIONAL_BUTTONS_FORMAT
   #ifndef NEW_MAX_ADDITIONAL_BUTTONS

      #define USE_NEW_ADDITIONAL_BUTTONS_FORMAT true

      #define NEW_MAX_ADDITIONAL_BUTTONS 0

      #define NEW_ADDITIONAL_BUTTON_ACTIONS {\
                              FUNCTION_NULL\
                              }

      #define NEW_ADDITIONAL_BUTTON_LATCHING {\
                              true\
                              }

      #define NEW_ADDITIONAL_BUTTON_PIN {\
                              -1\
                              }

      #define NEW_ADDITIONAL_BUTTON_TYPE {\
                              INPUT_PULLUP\
                              }

   #endif
#endif

// *******************************************************************************************************************

#ifndef USE_ROTARY_ENCODER_FOR_THROTTLE
   #define USE_ROTARY_ENCODER_FOR_THROTTLE true
#endif
#ifndef THROTTLE_POT_PIN
   #define THROTTLE_POT_PIN 39
#endif
#ifndef THROTTLE_POT_USE_NOTCHES
   #define THROTTLE_POT_USE_NOTCHES true
#endif
#ifndef THROTTLE_POT_NOTCH_VALUES   
   #define THROTTLE_POT_NOTCH_VALUES {1111,1347,1591,1833,2105,2379,2622,2837};
#endif
#ifndef THROTTLE_POT_NOTCH_SPEEDS
   #define THROTTLE_POT_NOTCH_SPEEDS {0,18,36,54,72,90,108,127}
#endif

// *******************************************************************************************************************

#ifndef USE_BATTERY_TEST
   #define USE_BATTERY_TEST false
#endif
#ifndef BATTERY_TEST_PIN
   #define BATTERY_TEST_PIN 34
#endif
#ifndef BATTERY_CONVERSION_FACTOR
   #define BATTERY_CONVERSION_FACTOR 1.7
#endif
#ifndef USE_BATTERY_PERCENT_AS_WELL_AS_ICON
   #define USE_BATTERY_PERCENT_AS_WELL_AS_ICON false
#endif
#ifndef USE_BATTERY_SLEEP_AT_PERCENT
   #define USE_BATTERY_SLEEP_AT_PERCENT 3
#endif

// ***************************************************
//  ESPmDNS problem

// #ifndef USING_OLDER_ESPMDNS
//    #define USING_OLDER_ESPMDNS false
// #endif

// #if USING_OLDER_ESPMDNS == true
//   #define ESPMDNS_IP_ATTRIBUTE_NAME MDNS.IP(i)
// #else
//   #define ESPMDNS_IP_ATTRIBUTE_NAME MDNS.address(i)
// #endif

#if ESP_IDF_VERSION_MAJOR < 5
  #define ESPMDNS_IP_ATTRIBUTE_NAME MDNS.IP(i)
#else
  #define ESPMDNS_IP_ATTRIBUTE_NAME MDNS.address(i)
#endif

// ***************************************************
// Heartbeat 

// max period 
#ifndef MAX_HEARTBEAT_PERIOD
   #define MAX_HEARTBEAT_PERIOD 240000
#endif

#ifndef HEARTBEAT_ENABLED
   #define HEARTBEAT_ENABLED true
#endif

// ***************************************************
// roster sorting

#ifndef ROSTER_SORT_SEQUENCE
   #define ROSTER_SORT_SEQUENCE 1
#endif

// ***************************************************
// startup commands

#ifndef STARTUP_COMMAND_1
   #define STARTUP_COMMAND_1 ""
#endif
#ifndef STARTUP_COMMAND_2
   #define STARTUP_COMMAND_2 ""
#endif
#ifndef STARTUP_COMMAND_3
   #define STARTUP_COMMAND_3 ""
#endif
#ifndef STARTUP_COMMAND_4
   #define STARTUP_COMMAND_4 ""
#endif

#ifndef ACQUIRE_ROSTER_ENTRY_IF_ONLY_ONE
   #define ACQUIRE_ROSTER_ENTRY_IF_ONLY_ONE false
#endif

// ***************************************************
// other text

#ifndef CONSIST_SPACE_BETWEEN_LOCOS
   #define CONSIST_SPACE_BETWEEN_LOCOS " "
#endif

// ***************************************************
// loco Acquire

#ifndef DROP_BEFORE_ACQUIRE
   #define DROP_BEFORE_ACQUIRE false
#endif

#ifndef RESTORE_ACQUIRED_LOCOS
   #define RESTORE_ACQUIRED_LOCOS true
#endif

#ifndef CONSIST_RELEASE_BY_INDEX
   #define CONSIST_RELEASE_BY_INDEX true
#endif