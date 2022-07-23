const char* deviceName = "WiT Controller";

const boolean encoderRotationClockwiseIsIncreaseSpeed = false;
// false = Counterclockwise  true = clockwise

const boolean toggleDirectionOnEncoderButtonPressWhenStationary = true;
// true = if the locos(s) are stationary, clicking the encoder button will toggle the direction

//4x3 keypad
int buttonActions[10] = { E_STOP,           // 0
                         FUNCTION_0,        // 1 - lights
                         FUNCTION_1,        // 2 - bell
                         FUNCTION_2,        // 3 - horn
                         SPEED_MULTIPLIER,  // 4
                         SPEED_UP,          // 5
                         FUNCTION_NULL,     // 6
                         DIRECTION_REVERSE, // 7
                         SPEED_DOWN,        // 8
                         DIRECTION_FORWARD  // 9
};

// text that will appear when you press #
// if you rearrange the items above, modify this text to suit    * and # cannot be remapped
const String directCommandText[4][3] = {
    {"1: F0", "2: F1", "3: F2"},
    {"4: X Spd", "5: V Up", "6: "}, 
    {"7: Rev", "8: V Dwn", "9: Fwd"},
    {"*: Menu", "E Stop", "#: This"}
};

// DCC Addresses below or equal to this number will be treated as short addresses.  
// DCC++EX, NCE, Digitrax use 127, Lenz use 99
#define SHORT_DCC_ADDESS_LIMIT 127  
