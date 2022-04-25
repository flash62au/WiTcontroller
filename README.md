# WiTcontroller

Currently functioning:
- Connects to specified SSID with specified password
- Autoconncts to the first found wiThrottle Protocol Server
- Assign commands directly to the 1-9 and A,B,C,D keys (see list below)
- Rudimentary on-the-fly consists  (max 10 locos)
- Command menu (see below)
 - including the ability to select and deselect locos via the keypad

ToDo:
- automatically cycle through a list of possible SSIDs, till it finds a working one
- speed button repeat (i.e. hold the button down)
- disconnect does not appear to work fully
- automatic reconnection
- functions are currenly only sent the earliest selected loco
- change facing of a loco in consist
- nice to have: 
  - specify functions as latching/non-latching

Command menu:
- 0-9 keys = pressing these directly will do whatever you want to app them to do  (see below)
- \* = Menu:  The button press following the \* is the actual command:
 - 1 = Add loco.  Followed by the loco number, followed by \# to complete.  e.g. to select loco 99 you would press '\*199\#'
 - 2 = release loco:
  - Followed by the loco number, followed by \# to release an individual loco.  e.g. to deselect the loco 99 you would press '\*299\#'
  - Othewise followed directly by \#  to release all e.g. '\*2\#'
 - 3 = C1hange direction.  Followed by \# to complete.  e.g. forward '\*21\#'  reverse '\*20\#'
 - 0 = Function button. Followed by \# to complete.  e.g. to select function 12 you would press '\*012\#'
 - 8 = Power On/Off. Followed by \# to complete.
 - 9 = Disconnect/reconnect. Followed by \# to complete.  
Pressing '\*' again before the '\#' will terminate the current command (but not start a new command)
other things could be added to the menu \*4 - \*7 

Allowed assigments for the 0-9 and A,B,C,D keys:
- FUNCTION_NULL   - don't do anything
- FUNCTION_0 - FUNCTION_28
- SPEED_STOP
- SPEED_UP
- SPEED_DOWN
- SPEED_UP_FAST
- SPEED_DOWN_FAST
- E_STOP
- POWER_TOGGLE
- POWER_ON
- POWER_OFF
- DIRECTION_TOGGLE
- DIRECTION_FORWARD
- DIRECTION_REVERSE