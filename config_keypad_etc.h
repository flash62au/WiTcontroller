//
// DO NOT alter this file
// 
// If you need to alter one of the defines in here, do so in config_buttons.h  NOT HERE
//

///Keypad + encoder + oLed config

// *******************************************************************************************************************
// encoder

// default Small ESP32
#ifndef ROTARY_ENCODER_A_PIN
    #define ROTARY_ENCODER_A_PIN 12  // to DT
#endif
#ifndef ROTARY_ENCODER_B_PIN
    #define ROTARY_ENCODER_B_PIN 14  // to CLK
#endif
#ifndef ROTARY_ENCODER_BUTTON_PIN
    #define ROTARY_ENCODER_BUTTON_PIN 13  // to SW
#endif

#ifndef ROTARY_ENCODER_VCC_PIN
    #define ROTARY_ENCODER_VCC_PIN -1 /* 27 put -1 of Rotary encoder Vcc is connected directly to 3,3V; else you can use declared output pin for powering rotary encoder */
#endif
#ifndef ROTARY_ENCODER_STEPS
    #define ROTARY_ENCODER_STEPS 2 //depending on your encoder - try 1,2 or 4 to get expected behaviour
#endif

// *******************************************************************************************************************
// keypad

// 4x3 keypad
#ifndef ROW_NUM
    #define ROW_NUM     4
#endif
#ifndef COLUMN_NUM
    #define COLUMN_NUM  3
#endif
#ifndef KEYPAD_KEYS
    #define KEYPAD_KEYS  {'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}, {'*', '0', '#'}
#endif
#ifndef KEYPAD_ROW_PINS
    #define KEYPAD_ROW_PINS    {19, 18, 17, 16}
#endif
#ifndef KEYPAD_COLUMN_PINS
    #define KEYPAD_COLUMN_PINS { 4, 0, 2}
#endif


char keys[ROW_NUM][COLUMN_NUM] = {KEYPAD_KEYS};
byte pin_rows[ROW_NUM]      = KEYPAD_ROW_PINS; // GIOP19, GIOP18, GIOP5, GIOP17 connect to the row pins
byte pin_column[COLUMN_NUM] = KEYPAD_COLUMN_PINS;   // GIOP16, GIOP4, GIOP0 connect to the column pins

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

#ifndef KEYPAD_DEBOUNCE_TIME
    #define KEYPAD_DEBOUNCE_TIME 10
#endif
#ifndef KEYPAD_HOLD_TIME
    #define KEYPAD_HOLD_TIME 200
#endif

