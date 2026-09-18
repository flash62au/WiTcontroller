// **********************************************************************
// DO NOT alter this file
// **********************************************************************

#define FUNCTION_NULL -1 // do nothing

#define FUNCTION_0 0 // lights
#define FUNCTION_00 0 // lights
#define FUNCTION_1 1 // bell
#define FUNCTION_01 1 // bell
#define FUNCTION_2 2 // horn
#define FUNCTION_02 2 // horn
#define FUNCTION_3 3
#define FUNCTION_03 3
#define FUNCTION_4 4
#define FUNCTION_04 4
#define FUNCTION_5 5
#define FUNCTION_05 5
#define FUNCTION_6 6
#define FUNCTION_06 6
#define FUNCTION_7 7
#define FUNCTION_07 7
#define FUNCTION_8 8
#define FUNCTION_08 8
#define FUNCTION_9 9
#define FUNCTION_09 9
#define FUNCTION_10 10
#define FUNCTION_11 11
#define FUNCTION_12 12
#define FUNCTION_13 13
#define FUNCTION_14 14
#define FUNCTION_15 15
#define FUNCTION_16 16
#define FUNCTION_17 17
#define FUNCTION_18 18
#define FUNCTION_19 19
#define FUNCTION_20 20
#define FUNCTION_21 21
#define FUNCTION_22 22
#define FUNCTION_23 23
#define FUNCTION_24 24
#define FUNCTION_25 25
#define FUNCTION_26 26
#define FUNCTION_27 27
#define FUNCTION_28 28
#define FUNCTION_29 29
#define FUNCTION_30 30
#define FUNCTION_31 31

#define SPEED_STOP 40
#define SPEED_UP 41
#define SPEED_DOWN 42
#define SPEED_UP_FAST 43
#define SPEED_DOWN_FAST 44
#define E_STOP 45
#define SPEED_MULTIPLIER 46

#define DIRECTION_TOGGLE 50
#define DIRECTION_FORWARD 51
#define DIRECTION_REVERSE 52

#define SPEED_STOP_THEN_TOGGLE_DIRECTION 54   // stops the loco if moving.  Toggles the direction if stationary.

#define E_STOP_CURRENT_LOCO 55

#define MAX_THROTTLE_INCREASE 56
#define MAX_THROTTLE_DECREASE 57

// **********************************************************************
// actions that are not related to a loco need to be above 500
// **********************************************************************

#define POWER_TOGGLE 501
#define POWER_ON 502
#define POWER_OFF 503

#define SHOW_HIDE_BATTERY 504   // the battery test must be enabled for this to have any effect

#define SLEEP 505

#define NEXT_THROTTLE 510
#define THROTTLE_1 511
#define THROTTLE_2 512
#define THROTTLE_3 513
#define THROTTLE_4 514
#define THROTTLE_5 515
#define THROTTLE_6 516

#define CUSTOM_1 521
#define CUSTOM_2 522
#define CUSTOM_3 523
#define CUSTOM_4 524
#define CUSTOM_5 525
#define CUSTOM_6 526
#define CUSTOM_7 527
#define CUSTOM_8 528
#define CUSTOM_9 529
#define CUSTOM_10 530
#define CUSTOM_11 531

#define CUSTOM_MENU_SELECT_1 532
#define CUSTOM_MENU_SELECT_2 533
#define CUSTOM_MENU_SELECT_3 534
#define CUSTOM_MENU_SELECT_4 535
#define CUSTOM_MENU_SELECT_5 536
#define CUSTOM_MENU_SELECT_6 537
#define CUSTOM_MENU_SELECT_7 538
#define CUSTOM_MENU_SELECT_8 539
#define CUSTOM_MENU_SELECT_9 540
#define CUSTOM_MENU_SELECT_10 542
#define CUSTOM_MENU_SELECT_11 543

// **********************************************************************
// search functions
// **********************************************************************
// these can only be assigned to 'additional buttons' and not to the keypad or encoder button

#define HORN_OR_WHISTLE_SEARCH 600
#define WHISTLE_SEARCH 601
#define HORN_SEARCH 602
#define BRAKE_SEARCH 603

// **********************************************************************
// reserved for custom search commands
// 900 - 999

// **********************************************************************
