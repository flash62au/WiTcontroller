#define DEVICE_NAME "WiT Controller"

// ********************************************************************************************

// false = Counterclockwise  true = clockwise
#define ENCODER_ROTATION_CLOCKWISE_IS_INCREASE_SPEED false

// Define what the rotary encoder button does.   (Pushing down on the knob)
// By default it will stop the loco if moving, then a second push will change the direction.
// Uncomment and change the action if you wish it to do something else.
// See static.h or README.md for details on the allowed functions.
// #define ENCODER_BUTTON_ACTION SPEED_STOP_THEN_TOGGLE_DIRECTION 

// true = if the locos(s) are stationary, clicking the encoder button will toggle the direction
// this only takes effect if the ENCODER_BUTTON_ACTION (above) is set to SPEED_STOP_THEN_TOGGLE_DIRECTION
#define TOGGLE_DIRECTION_ON_ENCODER_BUTTON_PRESSED_WHEN_STATIONAY true

// ********************************************************************************************

// define what each button will do as direct press (not in a menu)   * and # cannot be remapped
// see static.h or README.md for details on the allowed functions
#define CHOSEN_KEYPAD_0_FUNCTION FUNCTION_0
#define CHOSEN_KEYPAD_1_FUNCTION FUNCTION_1
#define CHOSEN_KEYPAD_2_FUNCTION FUNCTION_2
#define CHOSEN_KEYPAD_3_FUNCTION FUNCTION_3
#define CHOSEN_KEYPAD_4_FUNCTION FUNCTION_4
#define CHOSEN_KEYPAD_5_FUNCTION NEXT_THROTTLE
#define CHOSEN_KEYPAD_6_FUNCTION SPEED_MULTIPLIER
#define CHOSEN_KEYPAD_7_FUNCTION DIRECTION_REVERSE
#define CHOSEN_KEYPAD_8_FUNCTION E_STOP
#define CHOSEN_KEYPAD_9_FUNCTION DIRECTION_FORWARD

// text that will appear when you press #
// if you rearrange the items above, modify this text to suit    * and # cannot be remapped here
#define CHOSEN_KEYPAD_0_DISPLAY_NAME "0 Lights"
#define CHOSEN_KEYPAD_1_DISPLAY_NAME "1 Bell"
#define CHOSEN_KEYPAD_2_DISPLAY_NAME "2 Horn"
#define CHOSEN_KEYPAD_3_DISPLAY_NAME "3 F3"
#define CHOSEN_KEYPAD_4_DISPLAY_NAME "4 F4"
#define CHOSEN_KEYPAD_5_DISPLAY_NAME "5 Nxt Ttl"
#define CHOSEN_KEYPAD_6_DISPLAY_NAME "6 X Spd"
#define CHOSEN_KEYPAD_7_DISPLAY_NAME "7 Rev"
#define CHOSEN_KEYPAD_8_DISPLAY_NAME "8 Estop"
#define CHOSEN_KEYPAD_9_DISPLAY_NAME "9 Fwd"

// by default, # will show the list above. if you change the following line to true, it will take you to the Loco Function Labels screen directly
#define HASH_SHOWS_FUNCTIONS_INSTEAD_OF_KEY_DEFS false

// ********************************************************************************************

// Uncomment and increase the number if you need more than 2 throttles
// Maximum supported by the WiThrottleProtocol is 6
//
// #define MAX_THROTTLES 2 

// ********************************************************************************************

// DCC Addresses below or equal to this number will be treated as short addresses.  
// DCC++EX, NCE, Digitrax use 127, Lenz use 99
#define SHORT_DCC_ADDESS_LIMIT 127  

// speed increase for each click of the encoder 
#define SPEED_STEP 4
#define SPEED_STEP_MULTIPLIER 3  // for 'fast' speed steps
// Additional multiplier.  If the multiplier is enabled from the menu, each rotation of the encoder becomes the speedStep * the AdditionalMultiplier
#define SPEED_STEP_ADDITIONAL_MULTIPLIER 2

// define the number of throttles that you want.
// to use multiple throttles, one of the keys or buttons will need to be defined as NEXT_THROTTLE.  (keypad 5 is by default)
// #define MAX_THROTTLES 2  // uncomment and increase the number if you need more that two throttles

// ********************************************************************************************

// define what each of the optional additional buttons will do
// see static.h or README.md for details on the allowed functions
// all must be included, just set the ones you don't need to FUNCTION_NULL
// the button numbers relate to the GPIO pins 5,15,25,26,27,32,33
#define CHOSEN_ADDITIONAL_BUTTON_0_FUNCTION FUNCTION_NULL   // GPIO 5
#define CHOSEN_ADDITIONAL_BUTTON_1_FUNCTION FUNCTION_NULL   // GPIO 15
#define CHOSEN_ADDITIONAL_BUTTON_2_FUNCTION FUNCTION_NULL   // GPIO 25
#define CHOSEN_ADDITIONAL_BUTTON_3_FUNCTION FUNCTION_NULL   // GPIO 26
#define CHOSEN_ADDITIONAL_BUTTON_4_FUNCTION FUNCTION_NULL   // GPIO 27
#define CHOSEN_ADDITIONAL_BUTTON_5_FUNCTION FUNCTION_NULL   // GPIO 32
#define CHOSEN_ADDITIONAL_BUTTON_6_FUNCTION FUNCTION_NULL   // GPIO 33

#define ADDITIONAL_BUTTON_DEBOUNCE_DELAY 50   // increase if you find the buttons bounce. i.e. activate twice on a single press

// ********************************************************************************************

// uncomment and/or correct one (only) of the #define lines below if you need to override the default .9 inch oLED dispaly settings 
//
// Please select a constructor line for below depending on your display
// U8g2 Constructor List (Frame Buffer)
// The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
// Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected
// this is one of the common .9 inch OLED displays
// #define OLED_TYPE U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 22, /* data=*/ 23);
// this is one of the common 1.3 inch OLED displays
// #define OLED_TYPE U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 22, /* data=*/ 23);
 