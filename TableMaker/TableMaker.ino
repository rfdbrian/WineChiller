/*
 * Copyright (c) 2016 RF Digital Corp. All Rights Reserved.
 *
 * The source code contained in this file and all intellectual property embodied in
 * or covering the source code is the property of RF Digital Corp. or its licensors.
 * Your right to use this source code and intellectual property is non-transferable,
 * non-sub licensable, revocable, and subject to terms and conditions of the
 * SIMBLEE SOFTWARE LICENSE AGREEMENT.
 * http://www.simblee.com/licenses/SimbleeSoftwareLicenseAgreement.txt
 *
 * THE SOURCE CODE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND.
 *
 * This heading must NOT be removed from this file.
 */

 /*
  * Table of values is drawn and updated on Simblee
  */

#include "SimbleeTable.h"

uint8_t eventID;
const char* STASIS = "INVENTORY";
const char* ANOMALY = "WINE TO STORE";

const char* const bullDozer[2] = {STASIS, ANOMALY};
int currentScreen;
SimbleeTable basicTable;



void SimbleeForMobile_onConnect()
{
  currentScreen = -1;
}  



/*
 * Create the first screen.
 * 
 * The screen simply consists of a label displaying the screen number
 * and a button which swaps screens. Register for events on the button
 * such that we receive notifications when it is pressed.
 */
void createStasisScreen()
{
  //
  // Portrait mode is the default, so that isn't required
  // here, but shown for completeness. LANDSCAPE could be
  // used for that orientation.
  //
  SimbleeForMobile.beginScreen(YELLOW, PORTRAIT);

//  int textID = SimbleeForMobile.drawText(80, 60, "Screen 1", BLACK, 40);
//  toScreen2ButtonID = SimbleeForMobile.drawButton(100, 200, 100, "Screen 2");

  //
  // Receive notifications when a "release" occurs on the button.
  // Other event notifications are supported and can be combined in this call.
  // I.e. to receive press and release events you'd pass:
  //
  // EVENT_PRESS | EVENT_RELEASE
  //
  // into the function:
  //
  // SimbleeForMobile.setEvents(toScreen2ButtonID, EVENT_PRESS | EVENT_RELEASE);
  //
  // However, you don't want to register for events which are not required as
  // that results in extra traffic.
  //
//  SimbleeForMobile.setEvents(toScreen2ButtonID, h);
  SimbleeForMobile.endScreen();
}
void createAnomalyScreen()
{
  //
  // Default to Portrait orientation
  //

  color_t darkgray = rgb(85,85,85);
  SimbleeForMobile.beginScreen(darkgray);
  basicTable = SimbleeTable(100);
  SimbleeForMobile.endScreen();

//  SimbleeForMobile.setEvents(toScreen1ButtonID, EVENT_RELEASE);
}

void printEvent(event_t &event)
{
  Serial.println("Event:");
  Serial.print("  ID: ");
  Serial.println(event.id);
  
  Serial.print("   Type: ");
  Serial.println(event.type);  

  Serial.print("   Value: ");
  Serial.println(event.value);
  
  Serial.print("   Text: ");
  Serial.println(event.text);  
  
  Serial.print("   Coords: ");
  Serial.print(event.x);  
  Serial.print(",");  
  Serial.println(event.y);  
}
  
/**
 * TODO: Reset UI-demo
 */
void SimbleeForMobile_onDisconnect()
{
}

void setup(){
  SimbleeForMobile.advertisementData = "WineAndChill";

  SimbleeForMobile.begin();
}

void loop(){
  if(SimbleeForMobile.updatable){
        
  }
  SimbleeForMobile.process();
}

void ui(){

  if(SimbleeForMobile.screen == currentScreen) return;
  
  currentScreen = SimbleeForMobile.screen;
  switch(SimbleeForMobile.screen)
  {
    case 1:
      createStasisScreen();
      break;
       
    case 2:
      createAnomalyScreen();
      break;
            
   default:
      Serial.print("ui: Uknown screen requested: ");
      Serial.println(SimbleeForMobile.screen);
  }
  
}

void ui_event(event_t &event) {
  eventID = event.id;

  printEvent(event);
//  if(event.id == toScreen1ButtonID && event.type == EVENT_RELEASE && currentScreen == 2)
//  {
//    SimbleeForMobile.showScreen(1);
//  } else if(event.id == toScreen2ButtonID && event.type == EVENT_RELEASE && currentScreen == 1) 
//  {
//    SimbleeForMobile.showScreen(2);
//  }
  
  if (basicTable.find_button_id(eventID)) {
    SimbleeForMobile.updateText(eventID, "CLICKED");
  }
  if (basicTable.find_label_id(eventID)) {
    SimbleeForMobile.updateText(eventID, "CLICKED");
  }
}
