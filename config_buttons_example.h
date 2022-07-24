#define DEVICE_NAME "WiT Controller"

// false = Counterclockwise  true = clockwise
#define ENCODER_ROTATION_CLOCKWISE_IS_INCREASE_SPEED false

// true = if the locos(s) are stationary, clicking the encoder button will toggle the direction
#define TOGGLE_DIRECTION_ON_ENCODER_BUUTTON_PRESSED_WHEN_STATIONAY true

// define what each button will do as direct press (not in a menu)   * and # cannot be remapped
// see static.h or README.md for details on the allowed functions
#define CHOSEN_KEYPAD_0_FUNCTION FUNCTION_0
#define CHOSEN_KEYPAD_1_FUNCTION FUNCTION_1
#define CHOSEN_KEYPAD_2_FUNCTION FUNCTION_2
#define CHOSEN_KEYPAD_3_FUNCTION FUNCTION_3
#define CHOSEN_KEYPAD_4_FUNCTION FUNCTION_4
#define CHOSEN_KEYPAD_5_FUNCTION FUNCTION_5
#define CHOSEN_KEYPAD_6_FUNCTION SPEED_MULTIPLIER
#define CHOSEN_KEYPAD_7_FUNCTION DIRECTION_REVERSE
#define CHOSEN_KEYPAD_8_FUNCTION E_STOP
#define CHOSEN_KEYPAD_9_FUNCTION DIRECTION_FORWARD

// text that will appear when you press #
// if you rearrange the items above, modify this text to suit    * and # cannot be remapped
#define CHOSEN_KEYPAD_0_DISPLAY_NAME "0 Lights"
#define CHOSEN_KEYPAD_1_DISPLAY_NAME "1 Bell"
#define CHOSEN_KEYPAD_2_DISPLAY_NAME "2 Horn"
#define CHOSEN_KEYPAD_3_DISPLAY_NAME "3 F3"
#define CHOSEN_KEYPAD_4_DISPLAY_NAME "4 F4"
#define CHOSEN_KEYPAD_5_DISPLAY_NAME "5 F5"
#define CHOSEN_KEYPAD_6_DISPLAY_NAME "6 X Spd"
#define CHOSEN_KEYPAD_7_DISPLAY_NAME "7 Rev"
#define CHOSEN_KEYPAD_8_DISPLAY_NAME "8 Estop"
#define CHOSEN_KEYPAD_9_DISPLAY_NAME "9 Fwd"

// DCC Addresses below or equal to this number will be treated as short addresses.  
// DCC++EX, NCE, Digitrax use 127, Lenz use 99
#define SHORT_DCC_ADDESS_LIMIT 127  
