# witController

Currently functioning:
- Connects to the first available SSID (of three you can specify) with the specified password
- Auto-connects to the first found wiThrottle Protocol Server if only one found, otherwise askes which to connect to
- Rudimentary on-the-fly consists
- Assign commands directly to the 1-9 (see list below)
- Command menu (see below for full list) including:
  - Able to select and deselect locos by their DCC address, via the keypad
  - Able to select from the first 10 locos in the roster
  - Power Track On/Off
  - Disconnect / Reconnect
  - Put ESP32 in deep sleep and restart it

ToDo:
- speed button repeat (i.e. hold the button down)
- automatic reconnection
- functions are currently only sent the earliest selected loco
- change facing of a loco in consist
- nice to have: 
  - specify functions as latching/non-latching
- able to select from more than 10 locos in the roster
- select from the server turnout/point list
- select from the server route list
- choose which SSID to connect to
- type in an ip address and port
- keep a list of ip addresses and ports if bonjour doesn't provide any

Command menu:
- 0-9 keys = pressing these directly will do whatever you want to app them to do  (see below)
- \* = Menu:  The button press following the \* is the actual command:
 - 1 = Add loco.  
      - Followed by the loco number, followed by \# to complete.  e.g. to select loco 99 you would press '\*199\#'
      - or \# alone to show the roster
 - 2 = release loco:
  - Followed by the loco number, followed by \# to release an individual loco.  e.g. to deselect the loco 99 you would press '\*299\#'
  - Otherwise followed directly by \#  to release all e.g. '\*2\#'
 - 3 = Change direction.  Followed by \# to complete.  e.g. forward '\*21\#'  reverse '\*20\#'
 - 5 = Throw turnout.  Followed by \# to complete.  e.g. Throw turnout XX12 '\*512\#'  (where XX is a prefix defined in the sketch) 
 - 6 = Close turnout.  Followed by \# to complete.  e.g. Close turnout XX12 '\*612\#'  (where XX is a prefix defined in the sketch)
 - 7 = Set Route.  Followed by \# to complete.  e.g. to Set route XX:XX:0012 '\*60012\#'  (where \'XX:XX:\' is a prefix defined in the sketch)
 - 0 = Function button. Followed by the loco number, Followed by \# to complete.  e.g. to make loco 99 the lead loco you would press '\*499\#'
 - 8 = Power On/Off. Followed by \# to complete.
 - 9 = Disconnect/reconnect. 
       - Followed by \# to complete.  
       - or followed by 9 then \# to put into deep sleep
Pressing '\*' again before the '\#' will terminate the current command (but not start a new command)

Pressing the Encoder button while the ESP32 is in Deep Sleep will revive.

Allowed assignments for the 0-9 keys:
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