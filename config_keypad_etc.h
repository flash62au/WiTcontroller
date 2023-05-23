///Keypad + encoder + oLed config

// *******************************************************************************************************************
// encoder

// large EPS32
// #define ROTARY_ENCODER_A_PIN 33
// #define ROTARY_ENCODER_B_PIN 32
// #define ROTARY_ENCODER_BUTTON_PIN 27

// Small ESP32
#define ROTARY_ENCODER_A_PIN 12
#define ROTARY_ENCODER_B_PIN 14
#define ROTARY_ENCODER_BUTTON_PIN 13

#define ROTARY_ENCODER_VCC_PIN -1 /* 27 put -1 of Rotary encoder Vcc is connected directly to 3,3V; else you can use declared output pin for powering rotary encoder */
#define ROTARY_ENCODER_STEPS 2 //depending on your encoder - try 1,2 or 4 to get expected behaviour

const int encoderDebounceTime = 5;   // in miliseconds

// *******************************************************************************************************************
// keypad

// 4x4 keypad
// #define ROW_NUM     4 // four rows
// #define COLUMN_NUM  4 // four columns
// char keys[ROW_NUM][COLUMN_NUM] = {
//   {'1', '2', '3', 'A'},
//   {'4', '5', '6', 'B'},
//   {'7', '8', '9', 'C'},
//   {'*', '0', '#', 'D'}
// };
// byte pin_rows[ROW_NUM]      = {19, 18, 5, 17}; // connect to the row pins
// byte pin_column[COLUMN_NUM] = {16, 4, 2, 15};   // connect to the column pins

// 4x3 keypad
#define ROW_NUM     4 // four rows
#define COLUMN_NUM  3 // four columns
char keys[ROW_NUM][COLUMN_NUM] = {
 {'1', '2', '3'},
 {'4', '5', '6'},
 {'7', '8', '9'},
 {'*', '0', '#'}
};
byte pin_rows[ROW_NUM]      = {19, 18, 17, 16}; // GIOP19, GIOP18, GIOP5, GIOP17 connect to the row pins
byte pin_column[COLUMN_NUM] = { 4, 0, 2};   // GIOP16, GIOP4, GIOP0 connect to the column pins

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );
const int keypadDebounceTime = 10;   // in miliseconds
const int keypadHoldTime = 200;   // in miliseconds


// *******************************************************************************************************************
// additional / optional buttons

// To use the additional buttons, adjust the functions assigned to them in config_buttons.h
#define MAX_ADDITIONAL_BUTTONS 7  // If you alter this number, you must also alter the additionalButtonActions array in WiTcontroller.ino
int additionalButtonPin[MAX_ADDITIONAL_BUTTONS] = {5,15,25,26,27,32,33};
int additionalButtonType[MAX_ADDITIONAL_BUTTONS] = {INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP,INPUT_PULLUP};   // 34,35,36,39 don't have an internal pullup
boolean additionalButtonRead[MAX_ADDITIONAL_BUTTONS] =     {0,0,0,0,0,0,0};
boolean additionalButtonLastRead[MAX_ADDITIONAL_BUTTONS] = {0,0,0,0,0,0,0};

// *******************************************************************************************************************
// OLED

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>                       // add to include path [Arduino install]\hardware\arduino\avr\libraries\SPI\src
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>                      // add to include path [Arduino install]\hardware\arduino\avr\libraries\Wire\src
#endif

// Please select a constructor line for below depending on your display
// U8g2 Constructor List (Frame Buffer)
// The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
// Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 22, /* data=*/ 23, /* reset=*/ U8X8_PIN_NONE);
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 22, /* data=*/ 23, /* reset=*/ U8X8_PIN_NONE);
// this is one of the common .9 inch OLED displays
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 22, /* data=*/ 23);
// this is one of the common 1.3 inch OLED displays
// U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 22, /* data=*/ 23);