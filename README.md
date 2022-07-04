# WiTcontroller

A WiTcontroller is a simple DIY, handheld controller that talks to a wThrottle Server (JMRI, DCC++EX and many others) using the wiThrottle protocol to control DCC model trains. 

[See a video of it in use here.](https://youtu.be/RKnhfBCP_SQ)


[Discussion on Discord.](https://discord.com/channels/713189617066836079/735019837746577438)

---

## Prerequisites

1. Some basic soldering skills.  

    The components will work if just plugged together using jumpers, but they take a lot of space that way, so soldering them together is advised to make it hand held.

2. Loading the code (sketch) requires downloading of one of the IDEs, this sketch, the libraries, etc. so some experience with Arduinos is helpful, but not critical.

3. A wiThrottle Server to connect to. WiTcontroller will work with any wiThrottle Server. e.g.

    * JMRI
    * DCC++EX
    * MRC WiFi
    * Dijitrax LnWi

---

## Building

Required Components

* WeMos Lite LOLIN32  (ESP32 Arduino with LiPo charger) ([Example](https://www.ebay.com.au/itm/284800618644?hash=item424f709094:g:-soAAOSwHslfC9ce&frcectupt=true))
* 3x4 Keypad  ([Example](https://www.jaycar.com.au/12-key-numeric-keypad/p/SP0770?pos=2&queryId=20aedf107668ad42c6fe1f8b7f7a9ca7))
* Polymer Lithium Ion Battery LiPo 400mAh 3.7V 502535 JST Connector (or larger capacity) ([500mAh Example](https://www.ebay.com.au/itm/133708965793?hash=item1f21ace7a1:g:tlwAAOSwfORgYqYK))
* KY-040 Rotary Encoder Module ([Example](https://www.ebay.com.au/itm/394147297055?hash=item5bc502771f:g:gr0AAOSwcixiwo2~&frcectupt=true))
* OLED Display 0.96" 128x64 Blue I2C IIC SSD1306 ([Example](https://www.ebay.com.au/itm/273746192621?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2060353.m2749.l2649))
* Case - my one was 3d printed (see below)
* Knob ([Example](https://www.jaycar.com.au/35mm-knob-matching-equipment-style/p/HK7766?pos=7&queryId=cbd19e2486968bca41273cc2dbce54a4&sort=relevance))

![Assembly diagram](WiTcontroller%20pinouts%20v0.1.png)

Note:

* The case was 3D Printed for me by PeteGSX (See the [Discussion on Discord.](https://discord.com/channels/713189617066836079/735019837746577438) )
* The 3x4 keypad he designed the case for came from Jaycar and is slightly narrower than the one you see in the 'deconstructed' view in the video above.
* The case requires about a dozen M2x4mm screws

## Loading the code

1. Download the Arduino IDE. (I prefer to use VSC, but you still need to have the Arduino IDE installed.)
2. Download or clone this repository. (Note: if you 'clone', it is easier to receive updates to the code.)
3. Load the needed libraries to your PC (these can loaded from the library manager)
    * WiFi.h https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi
    * ESPmDNS.h https://github.com/espressif/arduino-esp32/blob/master/libraries/ESPmDNS
    * AiEsp32RotaryEncoder.h https://github.com/igorantolic/ai-esp32-rotary-encoder
    * Keypad.h https://www.arduinolibraries.info/libraries/keypad
    * U8g2lib.h
4. Download or clone the WiThrottleProtocol library to you PC (this **must** be placed in the 'libraries' folder)
    * WiThrottleProtocol.h https://github.com/flash62au/WiThrottleProtocol
5. Copy **config_network_example.h** to **config_network.h**  
Then edit it to include the network ssids you want to use
6. Upload the sketch

---

## Using WiTController

**Currently functioning:**
- provides a list of SSIDs with the specified password to choose from
  - choose from these or select from the found SSIDs, but...
    - they must have no password, or
    - if it is a DCC++EX wiFi server, it will guess the password
- Auto-connects to the first found wiThrottle Protocol Server if only one found, otherwise 
  - asks which to connect to
  - if none found will ask to enter the IP Address and Port
- Rudimentary on-the-fly consists
- Assign commands directly to the 1-9 (see list below)
- Command menu (see below for full list) including:
  - Able to select and deselect locos 
    - by their DCC address, via the keypad
    - from the first 50 locos in the roster
  - Able to throw/close turnouts/points
    - from the address
    - from the first 50 turnouts/points in the server list
  - Able to activate routes
    - from their address
    - from the first 50 routes in the server list
  - set/unset a multiplier for the rotary encoder
  - Power Track On/Off
  - Disconnect / Reconnect
  - Put ESP32 in deep sleep and restart it

**ToDo:**
- speed button repeat (i.e. hold the button down)
- automatic reconnection
- functions are currently only sent to the earliest selected loco
- change facing of a loco in consist
- nice to have: 
  - specify functions as latching/non-latching
- keep a list of ip addresses and ports if bonjour doesn't provide any

### Command menu:
- 0-9 keys = pressing these directly will do whatever you has been preset in the sketch for them to do  (see \# below)
- \* = Menu:  The button press following the \* is the actual command:
 - 1 = Add loco.  
      - Followed by the loco number, followed by \# to complete.  e.g. to select loco 99 you would press '\*199\#'
      - or \# alone to show the roster
 - 2 = release loco:
  - Followed by the loco number, followed by \# to release an individual loco.  e.g. to deselect the loco 99 you would press '\*299\#'
  - Otherwise followed directly by \#  to release all e.g. '\*2\#'
 - 3 = Change direction.  Followed by \# to complete.  e.g. forward '\*21\#'  reverse '\*20\#'
 - 4 = Set / Unset a 2 times multiplier for the rotary encoder dial.
 - 5 = Throw turnout/point.  
      - Followed by the turnout/point number, followed by the \# to complete.  e.g. Throw turnout XX12 '\*512\#'  (where XX is a prefix defined in the sketch) 
      - or \# alone to show the list from the server
 - 6 = Close turnout.    
      - Followed by the turnout/point number, followed by \# to complete.  e.g. Close turnout XX12 '\*612\#'  (where XX is a prefix defined in the sketch)
      - or \# alone to show the list from the server
 - 7 = Set Route.    
      - Followed by the Route number, followed by \# to complete.  e.g. to Set route XX:XX:0012 '\*60012\#'  (where \'XX:XX:\' is a prefix defined in the sketch)
      - or \# alone to show the list from the server
 - 0 = Function button. Followed by the loco number, Followed by \# to complete.  e.g. to set function 17 you would press '\*017\#'
 - 8 = Track Power On/Off. Followed by \# to complete.
 - 9 = Disconnect/reconnect. 
       - Followed by \# to complete.  
       - or followed by 9 then \# to put into deep sleep
Pressing '\*' again before the '\#' will terminate the current command (but not start a new command)
 - \# = Pressing # alone will show the function the the numbered keys (0-9)perform, outside the menu.

Pressing the Encoder button while the ESP32 is in Deep Sleep will revive it.


### Default number key assignments (0-9)  (outside the menu)

* 0 = SPEED_STOP
* 1 = FUNCTION_0 (DCC Lights)
* 2 = FUNCTION_1 (DCC Bell)
* 3 = FUNCTION_3 (DCC Horn/Whistle)
* 4 = SPEED_MULTIPLIER
* 5 = SPEED_UP
* 6 = FUNCTION_NULL
* 7 = DIRECTION_REVERSE
* 8 = SPEED_DOWN
* 9 = DIRECTION_FORWARD`

### Allowed assignments for the 0-9 keys:

Note: you need to edit config.h to alter these assignments 
- FUNCTION_NULL   - don't do anything
- FUNCTION_0 - FUNCTION_28
- SPEED_STOP
- SPEED_UP
- SPEED_DOWN
- SPEED_UP_FAST
- SPEED_DOWN_FAST
- SPEED_MULTIPLIER
- E_STOP
- POWER_TOGGLE
- POWER_ON
- POWER_OFF
- DIRECTION_TOGGLE
- DIRECTION_FORWARD
- DIRECTION_REVERSE