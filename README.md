# WiTcontroller

A WiTcontroller is a simple DIY, handheld controller that talks to a wThrottle Server (JMRI, DCC++EX and many others) using the wiThrottle protocol to control DCC model trains. 

[See a video of it in use here.](https://youtu.be/RKnhfBCP_SQ)


[Discussion on Discord.](https://discord.com/channels/713189617066836079/735019837746577438)

[3d Printed Case](https://www.thingiverse.com/thing:5440351)

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
* Wire - If you plan to solder the connections,which is the recommended approach, then stranded, coloured wire is advisable.  ([Example](https://www.jaycar.com.au/rainbow-cable-16-core-sold-per-metre/p/WM4516))
* Optional: Up to 7 additional buttons can be added, each with their own independent commands.

*Standard Configuration Pinouts*
![Assembly diagram](WiTcontroller%20pinouts%20v0.1.png)

*Pinouts for Optional Additional Buttons*
![Assembly diagram - Optional Additional Buttons](WiTcontroller%20-%20Optional%20Buttons%20-%20pinouts%20v0.1.png)


Notes:

* My case was 3D Printed for me by peteGSX (See the [Thingiverse.](https://www.thingiverse.com/thing:5440351) )
* The 3x4 keypad petGSX designed the case for came from Jaycar and is slightly narrower than the one you see in the 'deconstructed' view in the video above.
* The case requires about a dozen M2x4mm screws

* For a different take on what is possible by extending the design, have a look at: https://1fatgmc.com/RailRoad/DCC/page-5-B.html

## Loading the code

1. Download the Arduino IDE.  
    * Available from  https://support.arduino.cc/hc/en-us/articles/360019833020-Download-and-install-Arduino-IDE
2. Download the esp32 boards in the Arduino IDE.
    * add the esp322 support with the following instructions:  (See here for detailed instructions:  https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/)
        * In the Arduino IDE, go to *File* > *Preferences*
        * Enter the following into the 'Additional Board Manager URLs' field:  https://dl.espressif.com/dl/package_esp32_index.json
    * Then Use the *Boards Manager* in the *Arduino IDE* to install the esp32 board support
        * *Tools* > *Board* > *Boards Manager*
        * Search for "esp32" by Expressive Systems.  Install version 1.0.6
3. Download or clone *this* repository. (Note: if you 'clone' initially, it is easier to receive updates to the code by doing a 'fetch' subsequently.  See Notes below.)
    * Clone - **First Time**
       * Install *GitHub Desktop* from https://desktop.github.com/
       * Create a free account on GitHub and authorise the app to allow it to connect top GitHub
       * Select *file* -> *Clone Repository* - or 'Clone an repository from the internet' from the welcome page then select the 'URL' tab
       * Enter *https://github.com/flash62au/WiTcontroller* as the URL
       * Select a local folder to install it.  The default folder for the Arduino usually looks like "...username\Documents\Arduino\". (This is a good but not essential place to put it.)
       * Click *Clone*
       * **Subsequently**  (Anytime after the first 'clone')
         * click *Fetch Origin* and any changes to the code will be bought down to you PC, but you config_buttons.h and config_network.h will not be touched.
    * Download 
       * Open *https://github.com/flash62au/WiTcontroller*
       * Click the green "Code" button and select download zip
       * Extract the zip file to a local folder.  The default folder for the Arduino usually looks like "...username\Documents\Arduino\". This is a good but not essential place to put it.
4. Load the needed libraries to your PC. These can loaded from the *Library Manager* in the *Arduino IDE*.
    * *U8g2lib.h* -  Search for "U8g2"   Install version 2.34.22
    * *AiEsp32RotaryEncoder.h* - search for "Ai Esp32 Rotary Encoder"  Install Version 1.6
    * *Keypad.h* - Search for "Keypad" by Mark Stanley   install version 3.1.1
    * *WiThrottleProtocol.h* - Search for "WiThrottleProtocol" (not "WiThrottle")  Install version 1.0.3 or later if available
5. These should have been automatically installed when you downloaded the esp32 boards.
    * *WiFi.h*  - https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi
    * *ESPmDNS.h* - https://github.com/espressif/arduino-esp32/blob/master/libraries/ESPmDNS
6. Copy '**config_network_example.h**' to a new file to '**config_network.h**'.
    * Then edit it to include the network ssids you want to use.  (Not essential, but entering passwords via the encoder is tedious.)
7. Copy '**config_buttons_example.h**' to a new file '**config_buttons.h**'.
    * Optionally, edit this to change the mapping of the keypad buttons to specific functions.
    * Optionally, edit this to configure the additional buttons (if you have included them) to specific functions.
    * Optionally, edit this to change if you want the function buttons to display when you press #, instead of the default of showing the Key Definitions
8. Upload the sketch.  
    * Select the board type as "WEMOS LOLIN32 Lite" in the *Arduino IDE*.
    * Connect the board via USB and select the appropriate port in the *Arduino IDE*.
    * Click *Upload* 

Notes: 
   * WiTcontroller version 1.19 or later requires WiThrottleProtocol version 1.1.3 or later.
   * WiTcontroller version 1.06 or later requires WiThrottleProtocol version 1.1.2 or later.
   * WiTcontroller version 1.01 or later requires WiThrottleProtocol version 1.1.0 or later.
   * WiTcontroller version 0.10 or later requires WiThrottleProtocol version 1.0.3 or later.
   * The *WiFi.h* and *ESPmDNS.h* libraries were automatically installed for me when I installed the esp32 boards, however you may need to install them manually.
   * Later versions of the esp board support are available and do appear to work, but if you have difficulties version 1.0.6 appears to be stable.
   * Later versions of the libraries generally should work, but if you have difficulties use the versions listed above.
   * To get the WiTcontroller sketch I recommend using either the git command line, or the far more friendly 'GitHub Desktop' app.  See instructions above.
   * If you receive and error related to Python, and you are on MacOs 12 and above please edit the platform file, change from python to python3 as follows; preferences->user/path/arduino/packages/hardware/esp32/version/platform.txt and edit the line that looks as follows:tools.gen_esp32part.cmd=python3 "{runtime.platform.path}/tools/gen_esp32part.py"

---

## Using WiTController

**Currently functioning:**
- Provides a list of discovered SSIDs with the ability to choose one. When you select one:
  - if it is one in your specified list (in the sketch), it will use that specified password 
  - if it is a DCC++EX wiFi Command Station in access Point mode, it will guess the password
  - otherwise it will ask to enter the password (Use the rotary encoder to choose each character and the encoder button to select it.  * = backspace.  # = enter the password.) 
- Optionally provides a list of SSIDs with the specified passwords (in the sketch) to choose from
- Auto-connects to the first found wiThrottle Protocol Server if only one found, otherwise 
  - Asks which to connect to
  - If none found will ask to enter the IP Address and Port
  - Guesses the wiThrottle IP address and Port for DCC++EX WiFi Access Point mode Command Stations
  - optionally can add a #define (a preference) to disable this auto connect feature
- Rudimentary on-the-fly consists
- Assign commands directly to the 1-9 buttons (in the sketch) (see list below)
  - This is done in config_button.h
  - Latching / non-latching for the function is provided by the roster entry of wiThrottle server
- Optional ability to assign commands directly to the 1-7 additional buttons (in the sketch) (see list below)
  - These are defined config_button.h
- Command menu (see below for full list) including:
  - Able to select and deselect locos 
    - by their DCC address, via the keypad
      - On NCE systems, a leading zero (0) will force a long address
    - from the first 50 locos in the roster
  - Able to select multiple locos to create a consist
    - Able to change the facing of the additional locos in the consists (via the 'extra' menu after selection)
  - Able to activate any function (0-28)
    - Showing of the roster function labels (from the wiThrottle server if provided)
    - Quick access to the functions by pressing #. Temporarily enabled via the Extras menu (or permanently enabled in config_button.h)
    - Limited ability to configure which functions are sent to the first or all locos in a consist (defined in config_button.h)
  - Able to throw/close turnouts/points
    - from the address
    - from the first 50 turnouts/points in the server list
  - Able to activate routes
    - from their address
    - from the first 50 routes in the server list
  - Set/unset a multiplier for the rotary encoder
  - Power Track On/Off
  - Disconnect / Reconnect
  - Put ESP32 in deep sleep and restart it
- Have up to 6 throttles, each with an unlimited number of locos in consist. Default is 2 throttles, which can be increased or decreased temporarily via the Extras menu (or permanently enabled in config_button.h)
- Limited dealing with unexpected disconnects.  It will throw you back to the WiThtottle Server selection screen.
- Boundary between short and long DCC addresses can be configured in config_buttons.h
- The default speed step (per encoder click) can be configured in config_buttons.h
- The controller will automatically shut down if no SSID is selected or entered in 4 minutes (to conserve the battery)

**ToDo:**
- Speed button repeat (i.e. hold the button down)
- Deal with unexpected disconnects better
  - automatic attempt to reconnect
- Keep a list of ip addresses and ports if mDNS doesn't provide any
- Remember (for the current session only) recently selected locos

### Command menu:
- 0-9 keys = pressing these directly will do whatever you has been preset in the sketch for them to do  (see \# below)
- \* = Menu:  The button press following the \* is the actual command:
  - 1 = Add loco.  
     - Followed by the loco number, followed by \# to complete.  e.g. to select loco 99 you would press '\*199\#'
     - or \# alone to show the roster   \# again will show the next page
  - 2 = release loco:
     - Followed by the loco number, followed by \# to release an individual loco.  e.g. to deselect the loco 99 you would press '\*299\#'
     - Otherwise followed directly by \#  to release all e.g. '\*2\#'
  - 3 = Toggle direction.
  - 4 = Set / Unset a 2 times multiplier for the rotary encoder dial.
  - 5 = Throw turnout/point.  
     - Followed by the turnout/point number, followed by the \# to complete.  e.g. Throw turnout XX12 '\*512\#'  (where XX is a prefix defined in the sketch) 
     - or \# alone to show the list from the server   \# again will show the next page
  - 6 = Close turnout.    
     - Followed by the turnout/point number, followed by \# to complete.  e.g. Close turnout XX12 '\*612\#'  (where XX is a prefix defined in the sketch)
     - or \# alone to show the list from the server
  - 7 = Set Route.    
      - Followed by the Route number, followed by \# to complete.  e.g. to Set route XX:XX:0012 '\*60012\#'  (where \'XX:XX:\' is a prefix defined in the sketch)
      - or \# alone to show the list from the server   \# again will show the next page
  - 0 = Function button. Followed by...
      - the function number, Followed by \# to complete.  e.g. to set function 17 you would press '\*017\#'
      - \# alone, to show the list of functions.
  - 8 = Track Power On/Off.
  - 9 = Extras. Followed by...
      - 0 then \# to toggle the action the the \# key does as a direct action, either to show the direct action key definitions, or the Function labels.  
      - 1 to change the facing of locos in a consist.
      - 3 to toggle the heartbeat check.
      - 4 to increase the number of available throttle (up to 6)
      - 5 to decrease the number of available throttle (down to 1)
      - 6 then \# to Disconnect/Reconnect.  
      - 7 (or 9) then \# to put into deep sleep
Pressing '\*' again before the '\#' will terminate the current command (but not start a new command)
 - \# = Pressing # alone will show the function the the numbered keys (0-9) perform, outside the menu.
       Optionally, you can configure it so that the the Function labels from the roster show 

Pressing the Encoder button while the ESP32 is in Deep Sleep will revive it.


### Default number key assignments (0-9)  (outside the menu)

* 0 = FUNCTION_0 (DCC Lights)
* 1 = FUNCTION_1 (DCC Bell)
* 2 = FUNCTION_3 (DCC Horn/Whistle)
* 3 = FUNCTION_3
* 4 = FUNCTION_4
* 5 = NEXT_THROTLE
* 6 = SPEED_MULTIPLIER
* 7 = DIRECTION_REVERSE
* 8 = SPEED_STOP
* 9 = DIRECTION_FORWARD

### Allowed assignments for the 0-9 keys:

Note: you need to edit config_buttons.h to alter these assignments   (copy config_buttons_example.h)
- FUNCTION_NULL   - don't do anything
- FUNCTION_0 - FUNCTION_28
- SPEED_STOP
- SPEED_UP
- SPEED_DOWN
- SPEED_UP_FAST
- SPEED_DOWN_FAST
- SPEED_MULTIPLIER
- E_STOP   - E Stop all locos on all throttles
- E_STOP_CURRENT_LOCO - E Stop locos on current throttle only
- POWER_TOGGLE
- POWER_ON
- POWER_OFF
- DIRECTION_TOGGLE
- DIRECTION_FORWARD
- DIRECTION_REVERSE
- NEXT_THROTTLE
- SPEED_STOP_THEN_TOGGLE_DIRECTION   - stops the loco if moving.  Toggles the direction if stationary.
- MAX_THROTTLE_INCREASE    - change the number of available throttles on-the-fly
- MAX_THROTTLE_DECREASE    - change the number of available throttles on-the-fly