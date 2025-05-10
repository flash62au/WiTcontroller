# Change Log

### V1.92
- Additional button option SLEEP.  Will put the ESP32 to sleep. (i.e. turn off)

### V1.91
- Optional use of the WiFi Country Code. See ``config_network_example.h`` for more information

### V1.90
- Option to release locos in consist/mu by the index rather than the Address. New optional ``#define DROP_LOCO_BY_INDEX true``

### V1.89
- Bug fix for SPEED_STOP SPEED_UP SPEED_DOWN SPEED_UP_FAST SPEED_DOWN_FAST E_STOP SPEED_MULTIPLIER

### V1.88
- Additional actions to change to specific throttle  THROTTLE_1 THROTTLE_2 THROTTLE_3 THROTTLE_4 THROTTLE_5 THROTTLE_6

### V1.87
- support UTF8 fonts better
- Italian Translations by Roberto B
- able to override the default font (not recommended)
- add define for ``CUSTOM_APPNAME``

### V1.86
- added missing debug messages for the battery

### V1.85
- bug fix for when using the Arduino IDE

### V1.84
- New set of defines to allow for an indefinite number of additional buttons. This requires the use of new set of defines that override the old style ones. <br/> At the moment you can only have up to 11 buttons.
- Bug fix for momentary functions selected from the roster

### V1.83
- log any unknown commands from the server.  Requires WiThrottleProtocol version 1.1.26 or later

### V1.82
- Made ``RESTORE_ACQUIRED_LOCOS`` default to true.
  - Added an Extra Menu option to save the current locos
  - Removed the save when deliberately disconnected or slept the device (extra menu ``#96`` or ``#97``)

### V1.81
- Show the battery status on the Start, SSID browse, Wit browse and Shutdown screens.

### V1.80
- Added ``RESTORE_ACQUIRED_LOCOS`` option. If this option is enabled, and you have deliberately disconnected or slept the device (extra menu ``#96`` or ``#97``), then the next time you connect the device it will try to load the previously acquired locos.

### V1.78
- Added ``DROP_BEFORE_ACQUIRE`` option and also toggle for this in the Extras menu

### V1.77
- Bug fix for when a DCC-EX EX-CommandStation has the password changed from the default.

### V1.77
- The additional CR+LF are now sent ahead of all commands to all command stations by default. This was originally only sent for WiFiTrax, but other commercial CSs seem to have the same issue that this resolved. This can be disabled if needed with ``#define SEND_LEADING_CR_LF_FOR_COMMANDS false`` in ``confing_network.h``.
- Added a message when the server has just connected and is caught up receiving the roster etc. and is unable to process commands yet.

### V1.76
- Option to automatically acquire the only roster entry on connection to the WiThrottle server, but only if there is ***only one*** roster entry.  ``#define ACQUIRE_ROSTER_ENTRY_IF_ONLY_ONE true``

### V1.75
- Option for up to four start-up commands ``#define STARTUP_COMMAND_1 ""`` .. ``#define STARTUP_COMMAND_4 ""``

### V1.74
- German translations by Bastian Zechendorf.
- Made it simpler to change the language.
- Improvements for non-English language display.
- Bug fix. Removed duplicated first entry in the Roster (at the end).

### V1.73
- Add debug level option ``#define DEBUG_LEVEL``
- A bug in the heartbeat send in the protocol library was fixed at the same time as this release.
- if heartbeat is enabled, deliberately send the command to server to start it. (I had previously assumed that this was not required.)

### V1.72
- add the ability to sort the roster with the new define ``ROSTER_SORT_SEQUENCE``
   - 0 = no sorting.  As it comes from the server
   - 1 = sort by name (first 10 chars only)  - Default
   - 2 = sort by DCC Address

### V1.71
- add the ability to define the consist follow function options for all 32 functions ``CONSIST_FUNCTION_FOLLOW_F0`` to ``CONSIST_FUNCTION_FOLLOW_F31``

### V1.70
- add define to enable or disable the heartbeat by default    ``#define HEARTBEAT_ENABLED true``

### V1.69
- New define added for if no response is received from the server in the specified period (milliseconds), shut down. MAX_HEARTBEAT_PERIOD   default is 4 minutes = 240000 

### V1.68
- improve bouncing on the pot values
- bug fix for functions list & menus closing when the pot value bounces

### V1.67
- bug fix for adding locos greater than 300 and less than 499

### V1.66
- Add support for setting the additional button as latching or non-latching, overriding the WiThrottle defaults. Requires WiThrottleProtocol version 1.1.24.

### V1.65
- Fix for individual loco direction (facing) changes in a consist. Requires WiThrottleProtocol version 1.1.23 or later

### V1.63 - v1.64
- bug fix for the user defined menus

### V1.62
- rewrite of the menu code to support user defined menus
- bug fix for the Edit Consist not working

### V1.61
- Bug fix - Speed step multiplier not being displayed
- Added screenshots to the Readme

### V1.60
- ignore broadcast alert "Steal from other WiThrottle or JMRI throttle Required"

### V1.59
- Experimental attempt to automatically steal locos on Dijitrax systems

### V1.58
- bug fix when not using the battery test

### V1.56 / 1.57
- Moved the battery position to the top line
- added an Action to show or hide the battery
- Added some of the UI improvements from HandCab
- Fixed the E_STOP VS E_STOP_CURRENT_LOCO. Requires WiThrottleProtocol version 1.1.20 to actually work differently.
- Updated documentation. (Particularly on the use of the battery)

### V1.55
- automated fix the latest versions of the ESP32 Board Library (3.0.0 and later) having renamed an attribute. The code now automatically adjusts for this.  
- German translation file included

### V1.54
- minor format change 

### V1.53
- Add support for hardware build that utilises a bare EC11 rotary encoder (with no physical resistor pullups) in place of the KY040 encoder module (which incorporates physical pullups)
- This version will switch on GPIO internal pullups if EC11_PULLUPS_REQUIRED is set true in config_buttons.h
- added auto deep sleep on low battery 

### V1.52
- Made two changes in main code 1. Comment out random values when displaying battery percentage
- and 2.Invert logic for the test to display the battery percentage 

### V1.51
- changed option to have the battery to show icon as well as a percent

### V1.50
- option to have the battery to show as a percent

### V1.49
- change the battery check to once ever 10 seconds

### V1.48
- change the battery charge level to an icon/glyph

### V1.47
- support for optionally using a voltage divider to show the battery charge level

### V1.46
- support for optionally using a potentiometer (pot) instead of the encoder for the throttle

### V1.45
- support for 4x4 keypads

### V1.44
- support for optional custom commands

### V1.43
- bug fix

### V1.43
 - text, button and display PIN configurations moved to defines that can be overridden in personal config_buttons_etc.h files
 - no functional changes

### V1.42
 - minor bug fix
 
### V1.41
  - support for broadcast messages. Requires WiThrottleProtocol version 1.1.12 or later.

### V1.40
 - fix for the debounce of the rotary encoder button. Will now ignore rotations when the button is pressed for (default) 200ms.  The #define for the debounce has been moved from config_keypad_etc.h to config_buttons as: #define ROTARY_ENCODER_DEBOUNCE_TIME 200

### V1.38 & V1.39 
- fix problem with wifiTrax

### V1.36 & V1.37
- failed attempt to fix problem with wifiTrax

### V1.35
- add option to send commands twice

### V1.34
- minimum time separation/delay between commands sent

### V1.33
- changes to allow the use of VSC

### V1.32
- add support for 32 functions
- bypass WiT server search if EX-CS is in AP mode

### v1.31
- option to auto-connect to first SSID

### prior 
- various
