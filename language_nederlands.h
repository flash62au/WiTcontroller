// Version 1.0 for WiTcontroller 1.105 by Hans Metselaar
#ifndef MENU_TEXT_MENU
  #define MENU_TEXT_MENU                                "* Menu            # Toetsindeling"             // "* Menu # Key Defs"
#endif
#ifndef MENU_TEXT_MENU_HASH_IS_FUNCTIONS
  #define MENU_TEXT_MENU_HASH_IS_FUNCTIONS              "* Menu                     # Fn"               // "* Menu # Fn"
#endif
#ifndef MENU_TEXT_FINISH
  #define MENU_TEXT_FINISH                              "                        # Gereed"              // "# Finish"
#endif
#ifndef MENU_TEXT_CANCEL
  #define MENU_TEXT_CANCEL                              "* Annuleren"                                   // "* Cancel"
#endif
#ifndef MENU_TEXT_SHOW_DIRECT
  #define MENU_TEXT_SHOW_DIRECT                         "            # Direkt bekijken"                 // "# Show Direct"
#endif
#ifndef MENU_TEXT_ROSTER
  #define MENU_TEXT_ROSTER                              "* Annuleren    0-9    #Pag."                   // "* Cancel 0-9 #Pg"
#endif
#ifndef MENU_TEXT_TURNOUT_LIST
  #define MENU_TEXT_TURNOUT_LIST                        "* Annuleren    0-9    #Pag."                   // "* Cancel 0-9 #Pg"
#endif
#ifndef MENU_TEXT_ROUTE_LIST
  #define MENU_TEXT_ROUTE_LIST                          "* Annuleren    0-9    #Pag."                   // "* Cancel 0-9 #Pg"
#endif
#ifndef MENU_TEXT_FUNCTION_LIST
  #define MENU_TEXT_FUNCTION_LIST                       "* Annuleren    0-9    #Pag."                   // "* Cancel 0-9 #Pg"
#endif
#ifndef MENU_TEXT_SELECT_WIT_SERVICE
  #define MENU_TEXT_SELECT_WIT_SERVICE                  "0-4      # ander IP   E.Knop UIT"              // "0-4 # Entry E.btn OFF"
#endif
#ifndef MENU_TEXT_SELECT_WIT_ENTRY
  #define MENU_TEXT_SELECT_WIT_ENTRY                    "0-9 * Annu. # Verb. E.Knop UIT"                // "0-9 * Back # Go E.btn OFF"
#endif
#ifndef MENU_TEXT_SELECT_SSIDS
  #define MENU_TEXT_SELECT_SSIDS                        "0-9       # Zoeken  E.Knop UIT"                // "0-9 # Search E.btn OFF"
#endif
#ifndef MENU_TEXT_SELECT_SSIDS_FROM_FOUND
  #define MENU_TEXT_SELECT_SSIDS_FROM_FOUND             "0-4 9 Lijst  # Pag.  E.Knop UIT"               // "0-4 9 List # Pg E.btn OFF"
#endif
#ifndef MENU_TEXT_ENTER_SSID_PASSWORD
  #define MENU_TEXT_ENTER_SSID_PASSWORD                 "E.Knop Kies  # Verb.  * Terug"                 // "E Chrs E.btn Slct # Go * Bck"
#endif


#ifndef DIRECT_COMMAND_LIST
  #define DIRECT_COMMAND_LIST                           "Direkt Commando"                               // "Direct Commands"
#endif
#ifndef DIRECTION_FORWARD_TEXT
  #define DIRECTION_FORWARD_TEXT                        "  >>>"                                         // "Fwd"
#endif
#ifndef DIRECTION_REVERSE_TEXT
  #define DIRECTION_REVERSE_TEXT                        "  <<<"                                         // "Rev"
#endif


#ifndef MSG_START
  #define MSG_START                                     "Start"                                         // "Start"
#endif
#ifndef MSG_BROWSING_FOR_SERVICE
  #define MSG_BROWSING_FOR_SERVICE                      "Zoeken naar WiT-Services"                      // "Browsing for WiT services"
#endif
#ifndef MSG_BROWSING_FOR_SSIDS
  #define MSG_BROWSING_FOR_SSIDS                        "Zoeken naar SSID's"                            // "Browsing for SSIDs"
#endif
#ifndef MSG_NO_SSIDS_FOUND
  #define MSG_NO_SSIDS_FOUND                            "Geen SSID's gevonden!"                         // "No SSIDs found"
#endif
#ifndef MSG_SSIDS_LISTED
  #define MSG_SSIDS_LISTED                              " bekende SSID's"                               // "SSIDs listed"
#endif
#ifndef MSG_SSIDS_FOUND
  #define MSG_SSIDS_FOUND                               "  SSIDs gevonden"                              // "SSIDs found"
#endif
#ifndef MSG_BOUNJOUR_SETUP_FAILED
  #define MSG_BOUNJOUR_SETUP_FAILED                     "Geen Listener ingericht!"                      // "Unable to setup Listener"
#endif
#ifndef MSG_NO_SERVICES_FOUND
  #define MSG_NO_SERVICES_FOUND                         "Geen Services gevonden!"                       // "No services found"
#endif
#ifndef MSG_NO_SERVICES_FOUND_ENTRY_REQUIRED
  #define MSG_NO_SERVICES_FOUND_ENTRY_REQUIRED          "WitServer IP:Poort invoeren"                   // "Enter witServer IP:Port"
#endif
#ifndef MSG_SERVICES_FOUND
  #define MSG_SERVICES_FOUND                            "Service(s) gevonden"                           // "Service(s) found"
#endif
#ifndef MSG_TRYING_TO_CONNECT
  #define MSG_TRYING_TO_CONNECT                         "Proberen te verbinden"                         // "Trying to Connect"
#endif
#ifndef MSG_CONNECTED
  #define MSG_CONNECTED                                 "             Verbonden"                        // "Connected"
#endif
#ifndef MSG_CONNECTING
  #define MSG_CONNECTING                                "             Verbinden..."                     // "Connecting..."
#endif


#ifndef MSG_ADDRESS_LABEL
  #define MSG_ADDRESS_LABEL                             "IP Adres:"                                     // "IP address:"
#endif
#ifndef MSG_CONNECTION_FAILED
  #define MSG_CONNECTION_FAILED                         "Verbinding mislukt!"                           // "Connection failed"
#endif
#ifndef MSG_DISCONNECTED
  #define MSG_DISCONNECTED                              "Verbroken"                                     // "Disconnected"
#endif
#ifndef MSG_AUTO_SLEEP
  #define MSG_AUTO_SLEEP                                "Te lang op keuze gewacht"                      // "Waited too long for Select"
#endif
#ifndef MSG_BATTERY_SLEEP
  #define MSG_BATTERY_SLEEP                             "Batterij zeer laag"                            // "Battery critically low"
#endif 
#ifndef MSG_START_SLEEP
  #define MSG_START_SLEEP                               "Afsluiten    E.Knop AAN"                       // "Shutting Down. E.btn ON"
#endif
#ifndef MSG_THROTTLE_NUMBER
  #define MSG_THROTTLE_NUMBER                           "             Throttle #"                       // "Throttle #"
#endif
#ifndef MSG_NO_LOCO_SELECTED
  #define MSG_NO_LOCO_SELECTED                          "   Geen Locomotief gekozen   "                 // "No Loco selected"
#endif
#ifndef MSG_ENTER_PASSWORD
  #define MSG_ENTER_PASSWORD                            "Wachtwoord invoeren"                           // "Enter Password"
#endif
#ifndef MSG_GUESSED_EX_CS_WIT_SERVER
  #define MSG_GUESSED_EX_CS_WIT_SERVER                  "EX-CS WiT-Server gevonden"                     // "Guessed' EX-CS WiT server"
#endif
#ifndef MSG_BYPASS_WIT_SERVER_SEARCH
  #define MSG_BYPASS_WIT_SERVER_SEARCH                  "Zoeken WiT-Server overslaan"                   // "Bypass WiT server search"
#endif
#ifndef MSG_NO_FUNCTIONS
  #define MSG_NO_FUNCTIONS                              "Funktielijst - Lijst leeg"                     // "Function List - No Functions"
#endif
#ifndef MSG_HEARTBEAT_CHECK_ENABLED
  #define MSG_HEARTBEAT_CHECK_ENABLED                   "Heartbeat-Check  ingeschakeld"                 // "Heartbeat Check Enabled"
#endif
#ifndef MSG_HEARTBEAT_CHECK_DISABLED
  #define MSG_HEARTBEAT_CHECK_DISABLED                  "Heartbeat-Check uitgeschakeld"                 // "Heartbeat Check Disabled"
#endif
#ifndef MSG_RECEIVING_SERVER_DETAILS
   #define MSG_RECEIVING_SERVER_DETAILS                 "Ontvang Data van Server"                       // "Getting data from server"
#endif

#ifndef MENU_ITEM_TEXT_TITLE_FUNCTION
  #define MENU_ITEM_TEXT_TITLE_FUNCTION                 "Funktie"                                       // "Function"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_ADD_LOCO
  #define MENU_ITEM_TEXT_TITLE_ADD_LOCO                 "Loco toev."                                    // "Add Loco"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_DROP_LOCO
  #define MENU_ITEM_TEXT_TITLE_DROP_LOCO                "Loco verw."                                    // "Drop Loco"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_TOGGLE_DIRECTION
  #define MENU_ITEM_TEXT_TITLE_TOGGLE_DIRECTION         "Richting Wijz."                                // "Toggle Dir"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_SPEED_STEP_MULTIPLIER
  #define MENU_ITEM_TEXT_TITLE_SPEED_STEP_MULTIPLIER    "Snelh. Stap"                                   // "X Speed Step"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_THROW_POINT
  #define MENU_ITEM_TEXT_TITLE_THROW_POINT              "Wissel afbuig"                                 // "Throw Point"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_CLOSE_POINT
  #define MENU_ITEM_TEXT_TITLE_CLOSE_POINT              "Wissel recht"                                  // "Close Point"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_ROUTE
  #define MENU_ITEM_TEXT_TITLE_ROUTE                    "Route"                                         // "Route"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_TRACK_POWER
  #define MENU_ITEM_TEXT_TITLE_TRACK_POWER              "Rail Power"                                    // "Trk Power"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_EXTRAS
  #define MENU_ITEM_TEXT_TITLE_EXTRAS                   "Extra's"                                       // "Extras"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_HEARTBEAT
  #define MENU_ITEM_TEXT_TITLE_HEARTBEAT                "Heartbeat"                                     // "Heartbeat"
#endif
#ifndef MENU_ITEM_TEXT_TITLE_EDIT_CONSIST
  #define MENU_ITEM_TEXT_TITLE_EDIT_CONSIST             "Consist Bewerken"                              // "Edit Consist Facing"
#endif


#ifndef MENU_ITEM_TEXT_MENU_FUNCTION
  #define MENU_ITEM_TEXT_MENU_FUNCTION                  "Nr+# Kiezen   * Annu.   # Lijst"               // "no+# Select * Cancel # List"
#endif
#ifndef MENU_ITEM_TEXT_MENU_ADD_LOCO
  #define MENU_ITEM_TEXT_MENU_ADD_LOCO                  "Adr+# Toev.   * Annu.   # Lijst"               // "addr+# Add * Cancel # Roster"
#endif
#ifndef MENU_ITEM_TEXT_MENU_DROP_LOCO
  #define MENU_ITEM_TEXT_MENU_DROP_LOCO                 "Adr+# Verw.   * Annu.   # Alles"               // "addr+# One * Cancel # All"
#endif
#ifndef MENU_ITEM_TEXT_MENU_DROP_LOCO_BYINDEX
  #define MENU_ITEM_TEXT_MENU_DROP_LOCO_BY_INDEX        "Nr+# Verw.    * Annu.   # Alles"               // "no+# One * Cancel # All"
#endif
#ifndef MENU_ITEM_TEXT_MENU_TOGGLE_DIRECTION
  #define MENU_ITEM_TEXT_MENU_TOGGLE_DIRECTION          "# Omschakelen"                                 // "# Toggle"
#endif
#ifndef MENU_ITEM_TEXT_MENU_SPEED_STEP_MULTIPLIER
  #define MENU_ITEM_TEXT_MENU_SPEED_STEP_MULTIPLIER     "* Annuleren      # Omschakelen"                // "* Cancel # Toggle"
#endif
#ifndef MENU_ITEM_TEXT_MENU_THROW_POINT
  #define MENU_ITEM_TEXT_MENU_THROW_POINT               "Nr+# afbuigen  * Annu.  # Lijst"               // "no+# Throw * Cancel # List"
#endif
#ifndef MENU_ITEM_TEXT_MENU_CLOSE_POINT
  #define MENU_ITEM_TEXT_MENU_CLOSE_POINT               "Nr+# recht     * Annu.   # Lijst"              // "no+# Close * Cancel # List"
#endif
#ifndef MENU_ITEM_TEXT_MENU_ROUTE
  #define MENU_ITEM_TEXT_MENU_ROUTE                     "Nr+# kiezen   * Annu.   # Lijst"               // "no+# Select * Cancel # List"
#endif
#ifndef MENU_ITEM_TEXT_MENU_TRACK_POWER
  #define MENU_ITEM_TEXT_MENU_TRACK_POWER               "* Annuleren      # Omschakelen"                // "* Cancel # Toggle"
  #endif
#ifndef MENU_ITEM_TEXT_MENU_EXTRAS
  #define MENU_ITEM_TEXT_MENU_EXTRAS                    "Nr kiezen          * Annuleren"                // "no Select * Cancel"
#endif
#ifndef MENU_ITEM_TEXT_MENU_HEARTBEAT
  #define MENU_ITEM_TEXT_MENU_HEARTBEAT                 "* Sluiten"                                     // "* Close"
#endif
#ifndef MENU_ITEM_TEXT_MENU_EDIT_CONSIST
  #define MENU_ITEM_TEXT_MENU_EDIT_CONSIST              "Nr Richting omkeren  * Sluiten"                // "no Chng Facing * Close"
#endif


#ifndef EXTRA_MENU_TEXT_CHAR_FUNCTION_KEY_TOGGLE
  #define EXTRA_MENU_TEXT_CHAR_FUNCTION_KEY_TOGGLE      "Fn/Toets <>"                                   // "Fnc/Key Tgl"
#endif
#ifndef EXTRA_MENU_TEXT_CHAR_EDIT_CONSIST
  #define EXTRA_MENU_TEXT_CHAR_EDIT_CONSIST             "Consist aanp."                                 // "Edt Consist"
#endif

#ifndef EXTRA_MENU_TEXT_CHAR_HEARTBEAT_TOGGLE
  #define EXTRA_MENU_TEXT_CHAR_HEARTBEAT_TOGGLE         "Heartbt A/U"                                   // "Heartbt Tgl"
#endif
#ifndef EXTRA_MENU_TEXT_CHAR_DROP_BEFORE_ACQUIRE_TOGGLE
  #define EXTRA_MENU_TEXT_CHAR_DROP_BEFORE_ACQUIRE_TOGGLE "1 Loco A/U"                                  // "1 loco Tgl"
#endif
#ifndef EXTRA_MENU_TEXT_SAVE_CURRENT_LOCOS
  #define EXTRA_MENU_TEXT_SAVE_CURRENT_LOCOS            "Locs Opslaan"                                  // "1 loco Tgl"
#endif
#ifndef EXTRA_MENU_TEXT_CHAR_INCREASE_MAX_THROTTLES
  #define EXTRA_MENU_TEXT_CHAR_INCREASE_MAX_THROTTLES   "#Throttles +"                                  // "#Throttles +"
#endif
#ifndef EXTRA_MENU_TEXT_CHAR_DECREASE_MAX_THROTTLES
  #define EXTRA_MENU_TEXT_CHAR_DECREASE_MAX_THROTTLES   "#Throttles -"                                  // "#Throttles -"
#endif
#ifndef EXTRA_MENU_TEXT_CHAR_DISCONNECT
  #define EXTRA_MENU_TEXT_CHAR_DISCONNECT               "Verbreken"                                     // "Disconnect"
#endif
#ifndef EXTRA_MENU_TEXT_CHAR_OFF_SLEEP
  #define EXTRA_MENU_TEXT_CHAR_OFF_SLEEP                "UIT / Slaap"                                   // "OFF / Sleep"
#endif


#ifndef F0_LABEL
  #define F0_LABEL                                      "Licht"                                         // "Light"
#endif
#ifndef F1_LABEL
  #define F1_LABEL                                      "Bel"                                           // "Bell"
#endif
#ifndef F2_LABEL
  #define F2_LABEL                                      "Hoorn"                                         // "Horn"
#endif

#ifndef CHOSEN_KEYPAD_DISPLAY_PREFIX
  #define CHOSEN_KEYPAD_DISPLAY_PREFIX                  "* Menu"                                        // "* Menu"
#endif
#ifndef CHOSEN_KEYPAD_DISPLAY_SUFIX
  #define CHOSEN_KEYPAD_DISPLAY_SUFIX                   "# Deze"                                        // "# This"
#endif

#ifndef CHOSEN_KEYPAD_0_DISPLAY_NAME
  #define CHOSEN_KEYPAD_0_DISPLAY_NAME                  "0 Licht"                                       // "0 Lights"
#endif
#ifndef CHOSEN_KEYPAD_1_DISPLAY_NAME
  #define CHOSEN_KEYPAD_1_DISPLAY_NAME                  "1 Bel"                                         // "1 Bell"
#endif
#ifndef CHOSEN_KEYPAD_2_DISPLAY_NAME
  #define CHOSEN_KEYPAD_2_DISPLAY_NAME                  "2 Hoorn"                                       // "2 Horn"
#endif
#ifndef CHOSEN_KEYPAD_3_DISPLAY_NAME
  #define CHOSEN_KEYPAD_3_DISPLAY_NAME                  "3 F3"                                          // "3 F3"
#endif
#ifndef CHOSEN_KEYPAD_4_DISPLAY_NAME
  #define CHOSEN_KEYPAD_4_DISPLAY_NAME                  "4 F4"                                          // "4 F4"
#endif
#ifndef CHOSEN_KEYPAD_5_DISPLAY_NAME
  #define CHOSEN_KEYPAD_5_DISPLAY_NAME                  "5 Thrtl+1"                                     // "5 Nxt Ttl"
#endif
#ifndef CHOSEN_KEYPAD_6_DISPLAY_NAME
  #define CHOSEN_KEYPAD_6_DISPLAY_NAME                  "6 X Snlh."                                     // "6 X Spd"
#endif
#ifndef CHOSEN_KEYPAD_7_DISPLAY_NAME
  #define CHOSEN_KEYPAD_7_DISPLAY_NAME                  "7 <<<"                                         // "7 Rev"
#endif
#ifndef CHOSEN_KEYPAD_8_DISPLAY_NAME
  #define CHOSEN_KEYPAD_8_DISPLAY_NAME                  "8 STOP!"                                       // "8 Estop"
#endif
#ifndef CHOSEN_KEYPAD_9_DISPLAY_NAME
  #define CHOSEN_KEYPAD_9_DISPLAY_NAME                  "9 >>>"                                         // "9 Fwd"
#endif
