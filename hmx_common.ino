
// *********************************************************************************
//  oLED functions
// *********************************************************************************

#ifdef ST7735 // HMX 2024-05-12 

void setAppnameForOled() {
  oledText[0] = appName; oledText[6] = appVersion; 
}

void setMenuTextForOled(int menuTextIndex) {
  oledText[10] = menu_text[menuTextIndex];
  if (broadcastMessageText!="") {
    if (millis()-broadcastMessageTime < 10000) {
      oledText[10] = broadcastMessageText;
    } else {
      broadcastMessageText = "";
    }
  }
}

void refreshOled() {
     debug_print("refreshOled(): ");
     debug_println(lastOledScreen);
  switch (lastOledScreen) {
    case last_oled_screen_speed:
      writeOledSpeed();
      break;
    case last_oled_screen_turnout_list:
      writeOledTurnoutList(lastOledStringParameter, lastOledTurnoutParameter);
      break;
    case last_oled_screen_route_list:
      writeOledRouteList(lastOledStringParameter);
      break;
    case last_oled_screen_function_list:
      writeOledFunctionList(lastOledStringParameter);
      break;
    case last_oled_screen_menu:
      writeOledMenu(lastOledStringParameter);
      break;
    case last_oled_screen_extra_submenu:
      writeOledExtraSubMenu();
      break;
    case last_oled_screen_all_locos:
      writeOledAllLocos(lastOledBooleanParameter);
      break;
    case last_oled_screen_edit_consist:
      writeOledEditConsist();
      break;
    case last_oled_screen_direct_commands:
      writeOledDirectCommands();
      break;
  }
}


void writeOledFoundSSids(String soFar) {
  menuIsShowing = true;
  keypadUseType = KEYPAD_USE_SELECT_SSID_FROM_FOUND;
  if (soFar == "") { // nothing entered yet
    clearOledArray();
    for (int i=0; i<10 && i<foundSsidsCount; i++) {
      if (foundSsids[(page*10)+i].length()>0) {
        oledText[i] = String(i) + ": " + foundSsids[(page*10)+i] + "   (" + foundSsidRssis[(page*10)+i] + ")" ;
      }
    }
    oledText[10] = "(" + String(page+1) +  ") " + menu_text[menu_select_ssids_from_found];
    writeOledArray(false, false);
  // } else {
  //   int cmd = menuCommand.substring(0, 1).toInt();
  }
}

void writeOledRoster(String soFar) {
  lastOledScreen = last_oled_screen_roster;
  lastOledStringParameter = soFar;

  menuIsShowing = true;
  keypadUseType = KEYPAD_USE_SELECT_ROSTER;
  if (soFar == "") { // nothing entered yet
    clearOledArray();
    for (int i=0; i<5 && i<rosterSize; i++) {
      if (rosterAddress[(page*5)+i] != 0) {
        oledText[i] = String(rosterIndex[i]) + ": " + rosterName[(page*5)+i] + " (" + rosterAddress[(page*5)+i] + ")" ;
      }
    }
    oledText[10] = "(" + String(page+1) +  ") " + menu_text[menu_roster];
    writeOledArray(false, false);
  // } else {
  //   int cmd = menuCommand.substring(0, 1).toInt();
  }
}

void writeOledTurnoutList(String soFar, TurnoutAction action) {
  lastOledScreen = last_oled_screen_turnout_list;
  lastOledStringParameter = soFar;
  lastOledTurnoutParameter = action;

  menuIsShowing = true;
  if (action == TurnoutThrow) {
    keypadUseType = KEYPAD_USE_SELECT_TURNOUTS_THROW;
  } else {
    keypadUseType = KEYPAD_USE_SELECT_TURNOUTS_CLOSE;
  }
  if (soFar == "") { // nothing entered yet
    clearOledArray();
    int j = 0;
    for (int i=0; i<10 && i<turnoutListSize; i++) {
      // j = (i<5) ? i : i+1;
      j = i;
      if (turnoutListUserName[(page*10)+i].length()>0) {
        oledText[j] = String(turnoutListIndex[i]) + ": " + turnoutListUserName[(page*10)+i].substring(0,10);
      }
    }
    oledText[10] = "(" + String(page+1) +  ") " + menu_text[menu_turnout_list];
    writeOledArray(false, false);
  // } else {
  //   int cmd = menuCommand.substring(0, 1).toInt();
  }
}

void writeOledRouteList(String soFar) {
  lastOledScreen = last_oled_screen_route_list;
  lastOledStringParameter = soFar;

  menuIsShowing = true;
  keypadUseType = KEYPAD_USE_SELECT_ROUTES;
  if (soFar == "") { // nothing entered yet
    clearOledArray();
    int j = 0;
    for (int i=0; i<10 && i<routeListSize; i++) {
      // j = (i<5) ? i : i+1;
      j = i;
      if (routeListUserName[(page*10)+i].length()>0) {
        oledText[j] = String(routeListIndex[i]) + ": " + routeListUserName[(page*10)+i].substring(0,10);
      }
    }
    oledText[10] =  "(" + String(page+1) +  ") " + menu_text[menu_route_list];
    writeOledArray(false, false);
  // } else {
  //   int cmd = menuCommand.substring(0, 1).toInt();
  }
}

void writeOledFunctionList(String soFar) {
  lastOledScreen = last_oled_screen_function_list;
  lastOledStringParameter = soFar;

  menuIsShowing = true;
  keypadUseType = KEYPAD_USE_SELECT_FUNCTION;
  
  if (soFar == "") { // nothing entered yet
    clearOledArray();
    if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar) > 0 ) {
      int j = 0; int k = 0;
      for (int i=0; i<10; i++) {
        k = (functionPage*10) + i;
        if (k < MAX_FUNCTIONS) {
          //j = (i<5) ? i : i+1;
          j = i;
          // if (functionLabels[currentThrottleIndex][k].length()>0) {
            oledText[j] = String(i) + ": " 
            + ((k<10) ? functionLabels[currentThrottleIndex][k].substring(0,10) : String(k) 
            + "-" + functionLabels[currentThrottleIndex][k].substring(0,7)) ;
            
            if (functionStates[currentThrottleIndex][k]) {
              oledTextInvert[j] = true;
            }
          // }
        }
      }
      oledText[10] = "(" + String(functionPage) +  ") " + menu_text[menu_function_list];
      // setMenuTextForOled("(" + String(functionPage) +  ") " + menu_function_list);
    } else {
      oledText[0] = MSG_NO_FUNCTIONS;
      oledText[2] = MSG_THROTTLE_NUMBER + String(currentThrottleIndex+1);
      oledText[3] = MSG_NO_LOCO_SELECTED;
      // oledText[5] = menu_cancel;
      setMenuTextForOled(menu_cancel);
    }
    writeOledArray(false, false);
  // } else {
  //   int cmd = menuCommand.substring(0, 1).toInt();
  }
}

void writeOledEnterPassword() {
  keypadUseType = KEYPAD_USE_ENTER_SSID_PASSWORD;
  encoderUseType = KEYPAD_USE_ENTER_SSID_PASSWORD;
  clearOledArray(); 
  String tempSsidPasswordEntered;
  tempSsidPasswordEntered = ssidPasswordEntered+ssidPasswordCurrentChar;
  if (tempSsidPasswordEntered.length()>12) {
    tempSsidPasswordEntered = "\253"+tempSsidPasswordEntered.substring(tempSsidPasswordEntered.length()-12);
  } else {
    tempSsidPasswordEntered = " "+tempSsidPasswordEntered;
  }
  oledText[0] = MSG_ENTER_PASSWORD;
  oledText[2] = tempSsidPasswordEntered;
  // oledText[5] = menu_enter_ssid_password;
  setMenuTextForOled(menu_enter_ssid_password);
  writeOledArray(false, true);
}

void writeOledMenu(String soFar) {
  lastOledScreen = last_oled_screen_menu;
  lastOledStringParameter = soFar;

  debug_print("writeOledMenu() : "); debug_println(soFar);
  menuIsShowing = true;
  bool drawTopLine = false;
  if (soFar == "") { // nothing entered yet
    clearOledArray();
    int j = 0;
/*
    for (int i=1; i<10; i++) {
      j = (i<6) ? i : i+1;
      oledText[j-1] = String(i) + ": " + menuText[i][0];
    }
    oledText[10] = "0: " + menuText[0][0];
*/
    for (int i=0; i<10; i++) { // HMX 2024-05-12
      j = i;
      oledText[j] = String(i) + ": " + menuText[i][0];
    }

    // oledText[5] = menu_cancel;
    setMenuTextForOled(menu_cancel);
    writeOledArray(false, false);
  } else {
    int cmd = menuCommand.substring(0, 1).toInt();

    clearOledArray();

    oledText[0] = ">> " + menuText[cmd][0] +":"; oledText[6] =  menuCommand.substring(1, menuCommand.length());
    oledText[10] = menuText[cmd][1];

    switch (soFar.charAt(0)) {
      case MENU_ITEM_DROP_LOCO: {
            if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar) > 0) {
              writeOledAllLocos(false);
              drawTopLine = true;
            }
          } // fall through
      case MENU_ITEM_FUNCTION:
      case MENU_ITEM_TOGGLE_DIRECTION: {
          if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar) <= 0 ) {
            oledText[2] = MSG_THROTTLE_NUMBER + String(currentThrottleIndex+1);
            oledText[3] = MSG_NO_LOCO_SELECTED;
            // oledText[5] = menu_cancel;
            setMenuTextForOled(menu_cancel);
          } 
          break;
        }
      case MENU_ITEM_EXTRAS: { // extra menu
          writeOledExtraSubMenu();
          drawTopLine = true;
          break;
        }
    }

    writeOledArray(false, false, true, drawTopLine);
  }
}

void writeOledExtraSubMenu() {
  lastOledScreen = last_oled_screen_extra_submenu;

  int j = 0;
  for (int i=0; i<8; i++) {
    j = (i<4) ? i : i+2;
    oledText[j+1] = (extraSubMenuText[i].length()==0) ? "" : String(i) + ": " + extraSubMenuText[i];
  }
}

void writeOledAllLocos(bool hideLeadLoco) {
  lastOledScreen = last_oled_screen_all_locos;
  lastOledBooleanParameter = hideLeadLoco;

  int startAt = (hideLeadLoco) ? 1 :0;  // for the loco heading menu, we don't want to show the loco 0 (lead) as an option.
  debug_println("writeOledAllLocos(): ");
  String loco;
  int j = 0; int i = 0;
  if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar) > 0) {
    for (int index=0; ((index < wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar)) && (i < 8)); index++) {  //can only show first 8
      j = (i<4) ? i : i+2;
      loco = wiThrottleProtocol.getLocomotiveAtPosition(currentThrottleIndexChar, index);
      if (i>=startAt) {
        oledText[j+1] = String(i) + ": " + loco;
        if (wiThrottleProtocol.getDirection(currentThrottleIndexChar, loco) == Reverse) {
          oledTextInvert[j+1] = true;
        }
      }
      i++;      
    } 
  }
}

void writeOledEditConsist() {
  lastOledScreen = last_oled_screen_edit_consist;

  menuIsShowing = false;
  clearOledArray();
  debug_println("writeOledEditConsist(): ");
  keypadUseType = KEYPAD_USE_EDIT_CONSIST;
  writeOledAllLocos(true);
  oledText[0] = menuText[11][0];
  oledText[10] = menuText[11][1];
  writeOledArray(false, false);
}

void writeHeartbeatCheck() {
  menuIsShowing = false;
  clearOledArray();
  oledText[0] = menuText[10][0];
  if (heartbeatCheckEnabled) {
    oledText[1] = MSG_HEARTBEAT_CHECK_ENABLED; 
  } else {
    oledText[1] = MSG_HEARTBEAT_CHECK_DISABLED; 
  }
  oledText[10] = menuText[10][1];
  writeOledArray(false, false);
}

void writeOledSpeed() {
  lastOledScreen = last_oled_screen_speed;

  // debug_println("writeOledSpeed() ");
  
  menuIsShowing = false;
  String sLocos = "";
  String sSpeed = "";
  String sDirection = "";

  bool foundNextThrottle = false;
  String sNextThrottleNo = "";
  String sNextThrottleSpeedAndDirection = "";

  clearOledArray();
  
  boolean drawTopLine = false;

  if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar) > 0 ) {
    // oledText[0] = label_locos; oledText[2] = label_speed;
  
    for (int i=0; i < wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar); i++) {
      // sLocos = sLocos + " " + wiThrottleProtocol.getLocomotiveAtPosition(currentThrottleIndexChar, i);
      sLocos = sLocos + " " + getDisplayLocoString(currentThrottleIndex, i);
    }
    // sSpeed = String(currentSpeed[currentThrottleIndex]);
    sSpeed = String(getDisplaySpeed(currentThrottleIndex));
    sDirection = (currentDirection[currentThrottleIndex]==Forward) ? DIRECTION_FORWARD_TEXT : DIRECTION_REVERSE_TEXT;

    //find the next Throttle that has any locos selected - if there is one
    if (maxThrottles > 1) {
      int nextThrottleIndex = currentThrottleIndex + 1;

      for (int i = nextThrottleIndex; i<maxThrottles; i++) {
        if (wiThrottleProtocol.getNumberOfLocomotives(getMultiThrottleChar(i)) > 0 ) {
          foundNextThrottle = true;
          nextThrottleIndex = i;
          break;
        }
      }
      if ( (!foundNextThrottle) && (currentThrottleIndex>0) ) {
        for (int i = 0; i<currentThrottleIndex; i++) {
          if (wiThrottleProtocol.getNumberOfLocomotives(getMultiThrottleChar(i)) > 0 ) {
            foundNextThrottle = true;
            nextThrottleIndex = i;
            break;
          }
        }
      }
      if (foundNextThrottle) {
        sNextThrottleNo =  String(nextThrottleIndex+1);
        int speed = getDisplaySpeed(nextThrottleIndex);
        sNextThrottleSpeedAndDirection = String(speed);
        // if (speed>0) {
          if (currentDirection[nextThrottleIndex]==Forward) {
            sNextThrottleSpeedAndDirection = sNextThrottleSpeedAndDirection + DIRECTION_FORWARD_TEXT_SHORT;
          } else {
            sNextThrottleSpeedAndDirection = DIRECTION_REVERSE_TEXT_SHORT + sNextThrottleSpeedAndDirection;
          }
        // }
        // + " " + ((currentDirection[nextThrottleIndex]==Forward) ? DIRECTION_FORWARD_TEXT_SHORT : DIRECTION_REVERSE_TEXT_SHORT);
        sNextThrottleSpeedAndDirection = "     " + sNextThrottleSpeedAndDirection ;
        sNextThrottleSpeedAndDirection = sNextThrottleSpeedAndDirection.substring(sNextThrottleSpeedAndDirection.length()-5);
      }
    }

    oledText[0] = "   "  + sLocos; 
    //oledText[7] = "     " + sDirection;  // old function state format

    drawTopLine = true;

  } else {
    setAppnameForOled();
    oledText[2] = MSG_THROTTLE_NUMBER + String(currentThrottleIndex+1);
    oledText[3] = MSG_NO_LOCO_SELECTED;
    drawTopLine = true;
  }

  if (!hashShowsFunctionsInsteadOfKeyDefs) {
      // oledText[5] = menu_menu;
      setMenuTextForOled(menu_menu);
    } else {
    // oledText[5] = menu_menu_hash_is_functions;
    setMenuTextForOled(menu_menu_hash_is_functions);
  }

  writeOledArray(false, false, false, drawTopLine);

  if (wiThrottleProtocol.getNumberOfLocomotives(currentThrottleIndexChar) > 0 ) {
    writeOledFunctions();

     // throttle number
    //u8g2.setDrawColor(0);
    //u8g2.drawBox(0,0,12,16);
    //u8g2.setDrawColor(1);
    //u8g2.setFont(FONT_THROTTLE_NUMBER); // medium
    //u8g2.drawStr(2,15, String(currentThrottleIndex+1).c_str());
    buf.setTextColor(BLACK);
    buf.fillRect(0, 15, 12, 16, BLACK);
    buf.setTextColor(WHITE);
    buf.setTextFont(0); // HMX
    buf.drawString(String(currentThrottleIndex + 1).c_str(), 2, 15);
  }

  if (speedStep != currentSpeedStep[currentThrottleIndex]) {
    // oledText[3] = "X " + String(speedStepCurrentMultiplier);
    //u8g2.setDrawColor(1);
    //u8g2.setFont(FONT_SPEED_STEP);
    //u8g2.drawGlyph(1, 38, glyph_speed_step);
    //u8g2.setFont(FONT_DEFAULT);
    //u8g2.drawStr(9, 37, String(speedStepCurrentMultiplier).c_str());
    buf.setTextColor(WHITE);
    buf.setTextFont(0); // HMX
    buf.setTextFont(0); // HMX
    buf.drawString(("X " + String(speedStepCurrentMultiplier)).c_str(), 0, 37);
  }

  if (trackPower == PowerOn) {
    // u8g2.drawBox(0,41,15,8);
    //u8g2.drawRBox(0,40,9,9,1);
    //u8g2.setDrawColor(0);
    buf.fillRect(0, 48 + 50, 7, 9, WHITE);
    buf.setTextColor(BLACK);
  }

  //u8g2.setFont(FONT_TRACK_POWER);
  //u8g2.drawGlyph(1, 48, glyph_track_power);
  //u8g2.setDrawColor(1);
  buf.setTextFont(0); // HMX
  buf.drawString("P", 1 , 48 + 51 );
  buf.setTextColor(WHITE);

  if (!heartbeatCheckEnabled) {
    //u8g2.setFont(FONT_HEARTBEAT);
    //u8g2.drawGlyph(13, 49, glyph_heartbeat_off);
    //u8g2.setDrawColor(2);
    //u8g2.drawLine(13, 48, 20, 41);
    //u8g2.setDrawColor(1);
    buf.setTextFont(0); // HMX
    buf.drawLine(13, 48, 20, 41, WHITE);
    buf.setTextColor(WHITE);
  }

  // direction
  // needed for new function state format
  //u8g2.setFont(FONT_DIRECTION); // medium
  //u8g2.drawStr(79,36, sDirection.c_str());
  buf.setTextFont(0); // HMX
  buf.drawString(sDirection.c_str(), 79 + 20, 36);

  // speed
  const char *cSpeed = sSpeed.c_str();
  // u8g2.setFont(u8g2_font_inb21_mn); // big
  //u8g2.setFont(FONT_SPEED); // big
  //int width = u8g2.getStrWidth(cSpeed);
  //u8g2.drawStr(22+(55-width),45, cSpeed);
  buf.setTextFont(7); // HMX
  buf.setTextColor(GREEN);
  buf.drawRightString(cSpeed, 22+55+20,45, 7); // HMX 2023-OCT-29 //

  // speed and direction of next throttle
  if ( (maxThrottles > 1) && (foundNextThrottle) ) {
    //u8g2.setFont(FONT_NEXT_THROTTLE);
    //u8g2.drawStr(85+34,38, sNextThrottleNo.c_str() );
    //u8g2.drawStr(85+12,48, sNextThrottleSpeedAndDirection.c_str() );
    buf.setTextFont(0); // HMX
    buf.setTextColor(WHITE);
    buf.drawString(sNextThrottleNo.c_str(), 85 + 34 + 0, 38 + 40);
    buf.drawString(sNextThrottleSpeedAndDirection.c_str(), 85 + 12+ 0, 48 + 40);
  }

  //u8g2.sendBuffer();
  pushBuffer();

  // debug_println("writeOledSpeed(): end");
}

void writeOledFunctions() {
  lastOledScreen = last_oled_screen_speed;

  debug_println("writeOledFunctions():");
  //  int x = 99;
  // bool anyFunctionsActive = false;
   for (int i=0; i < MAX_FUNCTIONS; i++) {
     if (functionStates[currentThrottleIndex][i]) {
      // old function state format
  //     //  debug_print("Fn On "); debug_println(i);
  //     if (i < 12) {
  //     int y = (i+2)*10-8;
  //     if ((i>=4) && (i<8)) { 
  //       x = 109; 
  //       y = (i-2)*10-8;
  //     } else if (i>=8) { 
  //       x = 119; 
  //       y = (i-6)*10-8;
  //     }
      
  //     u8g2.drawBox(x,y,8,8);
  //     u8g2.setDrawColor(0);
  //     u8g2.setFont(u8g2_font_profont10_tf);
  //     u8g2.drawStr( x+2, y+7, String( (i<10) ? i : i-10 ).c_str());
  //     u8g2.setDrawColor(1);
  //   //  } else {
  //   //    debug_print("Fn Off "); debug_println(i);

      // new function state format
      // anyFunctionsActive = true;
      // u8g2.drawBox(i*4+12,12,5,7);
      // u8g2.drawRBox(i*4+12,12+1,5,7,2);
      // u8g2.setDrawColor(0);
      // u8g2.setFont(FONT_FUNCTION_INDICATORS);   
      // u8g2.drawStr( i*4+1+12, 18+1, String( (i<10) ? i : ((i<20) ? i-10 : i-20)).c_str());
      // u8g2.setDrawColor(1);
      buf.setTextColor(BLACK);
      buf.setTextColor(WHITE); // HMX temp
      buf.setTextFont(0); // HMX
      buf.drawString(String( (i < 10) ? i : ((i < 20) ? i - 10 : i - 20)).c_str(), i * 4 + 1 + 12, 18 + 1);
      buf.setTextColor(WHITE);
     }
    //  if (anyFunctionsActive) {
    //     u8g2.drawStr( 0, 18, (function_states).c_str());
    // //     u8g2.drawHLine(0,19,128);
    //  }
   }
  debug_println("writeOledFunctions(): end");
}

void writeOledArray(boolean isThreeColums, boolean isPassword) {
  writeOledArray(isThreeColums, isPassword, true, false);
}

void writeOledArray(boolean isThreeColums, boolean isPassword, boolean sendBuffer) {
  writeOledArray(isThreeColums, isPassword, sendBuffer, false);
}

void writeOledArray(boolean isThreeColums, boolean isPassword, boolean sendBuffer, boolean drawTopLine) {
  // debug_println("Start writeOledArray()");
  //u8g2.clearBuffer();					// clear the internal memory
  buf.fillRect(0, 0, 128, 128, BLACK);
  //u8g2.setFont(FONT_DEFAULT); // small
  buf.setTextFont(0); // HMX
  
  int x=0;
  int y=10;
  int xInc = 64; 
  int max = 12;
  if (isThreeColums) {
    xInc = 42;
    max = 18;
  }

  for (int i=0; i < max; i++) {
    const char *cLine1 = oledText[i].c_str();
  // if ((isPassword) && (i==2)) u8g2.setFont(FONT_PASSWORD); 
    if ((isPassword) && (i==2)) buf.setTextFont(0); 

    if (oledTextInvert[i]) {
      //u8g2.drawBox(x,y-8,62,10);
      //u8g2.setDrawColor(0);
      buf.fillRect(x,y-8,62,10, WHITE);
      buf.setTextColor(BLACK);
    }
    //u8g2.drawStr(x,y, cLine1);
    //u8g2.setDrawColor(1);
    buf.drawString(cLine1, x, y);
    buf.setTextColor(WHITE);

    // if ((isPassword) && (i==2)) u8g2.setFont(FONT_DEFAULT); 
    if ((isPassword) && (i==2)) buf.setTextFont(0); 
    y = y + 10;
//    if ((i==5) || (i==11)) { // HMX 2024-05-12
    if ((i==10) || (i==21)) { // HMX 2024-05-12
      x = x + xInc;
      y = 10;
    }
  }

  // if (drawTopLine) u8g2.drawHLine(0,11,128);
  //u8g2.drawHLine(0,51,128);
  if (drawTopLine) buf.drawLine(0, 11+7, 128, 11+7, WHITE); // HMX 2024-05-12
  buf.drawLine(0, 51+57, 128, 51+57, WHITE); // HMX 2024-05-12

  //if (sendBuffer) u8g2.sendBuffer();					// transfer internal memory to the display
  if (sendBuffer) pushBuffer();

}

void clearOledArray() {
  for (int i=0; i < 15; i++) {
    oledText[i] = "";
    oledTextInvert[i] = false;
  }
}

void writeOledDirectCommands() {
  lastOledScreen = last_oled_screen_direct_commands;

  oledDirectCommandsAreBeingDisplayed = true;
  clearOledArray();
  oledText[0] = DIRECT_COMMAND_LIST;
 
  oledText[1] = directCommandText[0][0];
  oledText[2] = directCommandText[0][1];
  oledText[3] = directCommandText[0][2];
  oledText[4] = directCommandText[1][0];
  oledText[5] = directCommandText[1][1];
  oledText[6] = directCommandText[1][2];
  oledText[7] = directCommandText[2][0];
  oledText[8] = directCommandText[2][1];
  oledText[9] = directCommandText[2][2];
  oledText[12] = directCommandText[3][1];
  oledText[13] = directCommandText[3][0];
  oledText[14] = directCommandText[3][2];
  
  /*
  for (int i=0; i < 4; i++) {
    oledText[i+1] = directCommandText[i][0];
  }
  int j = 0;
  for (int i=6; i < 10; i++) {
    oledText[i+1] = directCommandText[j][1];
    j++;
  }
  j=0;
  for (int i=12; i < 16; i++) {
    oledText[i+1] = directCommandText[j][2];
    j++;
  }
 */
  writeOledArray(true, false);
  menuCommandStarted = false;
}
#endif

// *********************************************************************************
