// ********************************************************************************************

// false = Counter clockwise  true = clockwise
#define ENCODER_ROTATION_CLOCKWISE_IS_INCREASE_SPEED                     false

// Define what the rotary encoder button does.   (Pushing down on the knob)
// By default it will stop the loco if moving, then a second push will change the direction.
// Uncomment and change the action if you wish it to do something else.
// See static.h or README.md for details on the allowed functions.

// #define ENCODER_BUTTON_ACTION SPEED_STOP_THEN_TOGGLE_DIRECTION 

// true = if the locos(s) are stationary, clicking the encoder button will toggle the direction
// this only takes effect if the ENCODER_BUTTON_ACTION (above) is set to SPEED_STOP_THEN_TOGGLE_DIRECTION

#define TOGGLE_DIRECTION_ON_ENCODER_BUTTON_PRESSED_WHEN_STATIONAY         true

// rotary encode debounce time
// increase if you find the encoder buttons bounce (activate twice) or you get speed changes when you press the encoder button
// #define ROTARY_ENCODER_DEBOUNCE_TIME 200

//Internal GPIO pullups required if the hardware build utilises a bare EC11 rotary encoder in place of a
//KY040 encoder module. (The encoder module has physical pullups fitted)
//true = bare EC11 used for hardware build WITHOUT any physical pullups, GPIO pullups will ne enabled in main
//false = KY040 module used in hardware build OR bare EC11 used but with physical pullup resistors (default false)

#define EC11_PULLUPS_REQUIRED         false

// ********************************************************************************************

// define what each button will do as direct press (not in a menu)   * and # cannot be remapped
// see actions.h or README.md for details on the allowed functions

// #define CHOSEN_KEYPAD_0_FUNCTION     FUNCTION_0
// #define CHOSEN_KEYPAD_1_FUNCTION     FUNCTION_1
// #define CHOSEN_KEYPAD_2_FUNCTION     FUNCTION_2
// #define CHOSEN_KEYPAD_3_FUNCTION     FUNCTION_3
// #define CHOSEN_KEYPAD_4_FUNCTION     FUNCTION_4
// #define CHOSEN_KEYPAD_5_FUNCTION     NEXT_THROTTLE
// #define CHOSEN_KEYPAD_6_FUNCTION     SPEED_MULTIPLIER
// #define CHOSEN_KEYPAD_7_FUNCTION     DIRECTION_REVERSE
// #define CHOSEN_KEYPAD_8_FUNCTION     E_STOP
// #define CHOSEN_KEYPAD_9_FUNCTION     DIRECTION_FORWARD
// #define CHOSEN_KEYPAD_A_FUNCTION     CUSTOM_1
// #define CHOSEN_KEYPAD_B_FUNCTION     CUSTOM_2
// #define CHOSEN_KEYPAD_C_FUNCTION     CUSTOM_3
// #define CHOSEN_KEYPAD_D_FUNCTION     CUSTOM_4

// text that will appear when you press #
// if you rearrange the items above, modify this text to suit    * and # cannot be remapped here

// #define CHOSEN_KEYPAD_0_DISPLAY_NAME     "0 Lights"
// #define CHOSEN_KEYPAD_1_DISPLAY_NAME     "1 Bell"
// #define CHOSEN_KEYPAD_2_DISPLAY_NAME     "2 Horn"
// #define CHOSEN_KEYPAD_3_DISPLAY_NAME     "3 F3"
// #define CHOSEN_KEYPAD_4_DISPLAY_NAME     "4 F4"
// #define CHOSEN_KEYPAD_5_DISPLAY_NAME     "5 Nxt Ttl"
// #define CHOSEN_KEYPAD_6_DISPLAY_NAME     "6 X Spd"
// #define CHOSEN_KEYPAD_7_DISPLAY_NAME     "7 Rev"
// #define CHOSEN_KEYPAD_8_DISPLAY_NAME     "8 Estop"
// #define CHOSEN_KEYPAD_9_DISPLAY_NAME     "9 Fwd"

// by default, # will show the list above. 
// if you change the following line to true, it will take you to the Loco Function Labels screen directly

#define HASH_SHOWS_FUNCTIONS_INSTEAD_OF_KEY_DEFS         true

// ********************************************************************************************

// define the number of throttles that you want.
// to use multiple throttles, one of the keys or buttons will need to be defined as NEXT_THROTTLE.  (keypad 5 is by default)
// Maximum supported by the WiTcontroller is 6

// #define MAX_THROTTLES                 2          // uncomment and increase the number if you always need more that two throttles

// ********************************************************************************************

// speed increase for each click of the encoder 

#define SPEED_STEP                       4
#define SPEED_STEP_MULTIPLIER            3          // for 'fast' speed steps

// Additional multiplier.  If the multiplier is enabled from the menu, each rotation of the encoder becomes the speedStep * the AdditionalMultiplier

#define SPEED_STEP_ADDITIONAL_MULTIPLIER 2

// by default, the speed will be displayed as the the DCC speed (0-126)
// IMPORTANT: only one should be enabled.  If DISPLAY_SPEED_AS_PERCENT is enabled it will take presidence over DISPLAY_SPEED_AS_0_TO_28
// Note: there will be rounding errors!

// uncomment this line to display the speeds a percentage.
// #define DISPLAY_SPEED_AS_PERCENT         true
// uncomment this line to display the speeds as 0-28.
// #define DISPLAY_SPEED_AS_0_TO_28         true

// ********************************************************************************************
// DCC functions in consists
#define CONSIST_FUNCTION_FOLLOW_F0                  CONSIST_ALL_LOCOS            // lights
#define CONSIST_FUNCTION_FOLLOW_F1                  CONSIST_LEAD_LOCO            // bell
#define CONSIST_FUNCTION_FOLLOW_F2                  CONSIST_LEAD_LOCO            // horn
#define CONSIST_FUNCTION_FOLLOW_OTHER_FUNCTIONS     CONSIST_LEAD_LOCO

// ********************************************************************************************

// define what each of the optional additional buttons will do
// see static.h or README.md for details on the allowed functions
// all must be included, just set the ones you don't need to FUNCTION_NULL
// the button numbers relate to the GPIO pins 5,15,25,26,27,32,33

#define CHOSEN_ADDITIONAL_BUTTON_0_FUNCTION     FUNCTION_NULL   // GPIO 5
#define CHOSEN_ADDITIONAL_BUTTON_1_FUNCTION     FUNCTION_NULL   // GPIO 15
#define CHOSEN_ADDITIONAL_BUTTON_2_FUNCTION     FUNCTION_NULL   // GPIO 25
#define CHOSEN_ADDITIONAL_BUTTON_3_FUNCTION     FUNCTION_NULL   // GPIO 26
#define CHOSEN_ADDITIONAL_BUTTON_4_FUNCTION     FUNCTION_NULL   // GPIO 27
#define CHOSEN_ADDITIONAL_BUTTON_5_FUNCTION     FUNCTION_NULL   // GPIO 32
#define CHOSEN_ADDITIONAL_BUTTON_6_FUNCTION     FUNCTION_NULL   // GPIO 33

// *******************************************************************************************************************
// additional / optional commands
// these can be any legitimate WiThrottle protocol command encosed in quotes or double quotes
// refer to https://www.jmri.org/help/en/package/jmri/jmrit/withrottle/Protocol.shtml

// alert message
// #define CUSTOM_COMMAND_1 "HMHello World"
// set route R500  (DCCEX system route perfix is 'R')
// #define CUSTOM_COMMAND_2 "PRA2R500"
// #define CUSTOM_COMMAND_3 ""
// #define CUSTOM_COMMAND_4 ""
// #define CUSTOM_COMMAND_5 ""
// #define CUSTOM_COMMAND_6 ""
// #define CUSTOM_COMMAND_7 ""

// ********************************************************************************************

// uncomment and/or correct one (only) of the #define lines below if you need to override the default .9 inch oLED display settings 
//
// Please select a constructor line for below depending on your display
// U8g2 Constructor List (Frame Buffer)
// The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
// Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected

// this is one of the common .9 inch OLED displays and is included by default
// #define OLED_TYPE U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 22, /* data=*/ 23);

// this is one of the common 1.3 inch OLED displays
// #define OLED_TYPE U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 22, /* data=*/ 23);

// *******************************************************************************************************************
// Debugging

// WiTcontroller console debug messages are ENABLED by default
// to disable to console messages, uncomment and set the following to  1    (i.e. 0 = on  1 = off)
// #define WITCONTROLLER_DEBUG    0

// WiThrottle Protocol console debug messages are DISABLED by default
// to enable to console messages, uncomment and set the following to  0    (i.e. 0 = on  1 = off)
// #define WITHROTTLE_PROTOCOL_DEBUG    1

// *******************************************************************************************************************
// Default latching/momentary functions - Direct mapping only 

// uncomment any of these lines if you wish to change.  latching = true or momentary = false
// F1 & F1 default to momentary
//#define F0_LATCHING false
//#define F1_LATCHING true
//#define F2_LATCHING true

// *******************************************************************************************************************
// Default function labels

// uncomment any of these lines if you wish to change the labels
//#define F0_LABEL "Light"
//#define F1_LABEL "Bell"
//#define F2_LABEL "Horn"

// *******************************************************************************************************************
// encoder hardware
// 
// uncomment and change as needed
// defaults are for the Small ESP32

// large EPS32
// #define ROTARY_ENCODER_A_PIN 33
// #define ROTARY_ENCODER_B_PIN 32
// #define ROTARY_ENCODER_BUTTON_PIN 27

// Small ESP32
// #define ROTARY_ENCODER_A_PIN 12
// #define ROTARY_ENCODER_B_PIN 14
// #define ROTARY_ENCODER_BUTTON_PIN 13

// #define ROTARY_ENCODER_VCC_PIN -1 /* 27 put -1 of Rotary encoder Vcc is connected directly to 3,3V; else you can use declared output pin for powering rotary encoder */
// #define ROTARY_ENCODER_STEPS 2 //depending on your encoder - try 1,2 or 4 to get expected behaviour

// *******************************************************************************************************************
// keypad hardware
//
// uncomment and change as needed
// defaults are for the Small 4x3 keypad

// 4x3 keypad - default
// #define ROW_NUM     4
// #define COLUMN_NUM  3
// #define KEYPAD_KEYS  {'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}, {'*', '0', '#'}
// #define KEYPAD_ROW_PINS    {19, 18, 17, 16}
// #define KEYPAD_COLUMN_PINS { 4, 0, 2}

// 4x4 keypad
// #define ROW_NUM     4
// #define COLUMN_NUM  4
// #define KEYPAD_KEYS {'1', '2', '3', 'A'},  {'4', '5', '6', 'B'},  {'7', '8', '9', 'C'},  {'*', '0', '#', 'D'}
// #define KEYPAD_ROW_PINS    {19, 18, 17, 16}
// #define KEYPAD_COLUMN_PINS {4, 0, 2, 33}


// in miliseconds

// #define KEYPAD_DEBOUNCE_TIME 10
// #define KEYPAD_HOLD_TIME 200

// *******************************************************************************************************************
// Additional / optional buttons
// For the 4x3 Keypad 7 buttons can be used
// For the 4x4 keypad, only 6 buttons can be used with the 'normal' pins and the last pin MUST be set to -1, 
//                     or one of pins 34,35,36,39 can be used with additional hardware (see below)

// for the 4x3 keypad - default
// To use the additional buttons, adjust the functions assigned to them in config_buttons.h
// #define ADDITIONAL_BUTTONS_PINS      {5,15,25,26,27,32,33}
// #define ADDITIONAL_BUTTONS_TYPE      {INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP}

// for the 4x4 keypad
// To use the additional buttons, adjust the functions assigned to them in config_buttons.h
// #define ADDITIONAL_BUTTONS_PINS      {5,15,25,26,27,32,-1}  // last pin must be set to -1
// #define ADDITIONAL_BUTTONS_TYPE      {INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP}

// Note: pins 34,35,36,39 can be used but don't have an internal pullup, 
//       so need additional hardware (resister) if you wish to use one of them


// default = 50
// increase if you find the buttons bounce. i.e. activate twice on a single press
// #define ADDITIONAL_BUTTON_DEBOUNCE_DELAY        50    


// *******************************************************************************************************************
// Throttle Pot
// To use a pot instead of the rotary encoder, uncomment the follow lines
//
// Set the number of notches and the values of the pot at each notch.  must be 8 values

// #define USE_ROTARY_ENCODER_FOR_THROTTLE false
// #define THROTTLE_POT_PIN 39
// #define THROTTLE_POT_USE_NOTCHES false  // if false, only THROTTLE_POT_NOTCH_VALUES 0 and 7 below (first and last) are use. 
// #define THROTTLE_POT_NOTCH_VALUES {1,585,1170,1755,2340,2925,3510,4094}
// #define THROTTLE_POT_NOTCH_SPEEDS {0,18,36,54,72,90,108,127}  // 0-127 These numbers will be the speed step for each of the 8 throttle notches.

// note: The example values above for THROTTLE_POT_NOTCH_VALUES 
// are useble for a 10k ohm pot but any value pot can be used. 
// Just adjust the numbers.
// Numbers can generated by Generate_Throttle__Brake__Reverser__Numbers program by Sumner Patterson

// *******************************************************************************************************************
// Battery Test
// To use a battery test
//  USE_BATTERY_SLEEP_AT_PERCENT - will put the device to sleep if the battery falls below this level
//                               - set to 0 (zero) to disable
//
// #define USE_BATTERY_TEST true
// #define BATTERY_TEST_PIN 34
// #define BATTERY_CONVERSION_FACTOR 1.7
// #define USE_BATTERY_PERCENT_AS_WELL_AS_ICON false
// #define USE_BATTERY_SLEEP_AT_PERCENT 3   // will put the device to sleep if the battery falls below this level

// *******************************************************************************************************************
//
