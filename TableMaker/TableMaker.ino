/*
   Copyright (c) 2016 RF Digital Corp. All Rights Reserved.

   The source code contained in this file and all intellectual property embodied in
   or covering the source code is the property of RF Digital Corp. or its licensors.
   Your right to use this source code and intellectual property is non-transferable,
   non-sub licensable, revocable, and subject to terms and conditions of the
   SIMBLEE SOFTWARE LICENSE AGREEMENT.
   http://www.simblee.com/licenses/SimbleeSoftwareLicenseAgreement.txt

   THE SOURCE CODE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND.

   This heading must NOT be removed from this file.
*/

/*
   Table of values is drawn and updated on Simblee
*/

#include "SimbleeTable.h"
#include <stdlib.h>

uint8_t eventID;
const char* STASIS = "INVENTORY";
const char* ANOMALY = "WINE TO STORE";

const char* const bullDozer[2] = {STASIS, ANOMALY};
int currentScreen;
Record* selectedBottle;
uint8_t extractionDrinkingButton;
uint8_t extractionRecyclingButton;
char updateTrue = 0;

char switchValue[64];
int receivedLoc = -1;
int switchOnOff = -1;
vector<Record> removedBottles;
std::map<int, long> cellTimings;

SimbleeTable wineTable = SimbleeTable();

void SimbleeForMobile_onConnect() {
  currentScreen = -1;
}

void createStasisScreen() {
  color_t darkgray = rgb(85, 85, 85);
  SimbleeForMobile.beginScreen(darkgray);
  wineTable.draw_table(100, "WINE INVENTORY");
  SimbleeForMobile.endScreen();
}

void createAnomalyScreen() {
  color_t darkgray = rgb(85, 85, 85);
  SimbleeForMobile.beginScreen(darkgray);

  wineTable.draw_table(100, "WINE LIMBO");

  SimbleeForMobile.endScreen();
}

void createExtractionScreen() {
  color_t darkgray = rgb(85, 85, 85);
  SimbleeForMobile.beginScreen(darkgray);

  int extractionText = SimbleeForMobile.drawText(80, 60,  "Problems yo", BLACK, 40);
  extractionDrinkingButton = SimbleeForMobile.drawButton(100, 200, 100, "Drank");
  extractionRecyclingButton = SimbleeForMobile.drawButton(100, 250, 100, "Gifted");

  SimbleeForMobile.setEvents(extractionDrinkingButton, EVENT_RELEASE);
  SimbleeForMobile.setEvents(extractionRecyclingButton, EVENT_RELEASE);

  SimbleeForMobile.endScreen();
}

void createInsertionScreen() {
  color_t darkgray = rgb(85, 85, 85);
  SimbleeForMobile.beginScreen(darkgray);
  wineTable.draw_table(100, "SELECT BOTTLE");
  SimbleeForMobile.endScreen();
}

void printEvent(event_t &event) {
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

void setup() {
  Serial.begin(9600);
  SimbleeForMobile.advertisementData = "DasChill";
  SimbleeForMobile.begin();
  wineTable.add_record("Los Reveles");
  wineTable.add_record("Francis Coppola");
  wineTable.add_record("Proximo");
  wineTable.add_record("Perez Cruz");
  wineTable.add_record("Baron de Marny");
  wineTable.add_record("Prieure de Cenac");
  Serial.println("Are we here");
}

int htoi (char c) {  //does not check that input is valid
   if (c >= '0' && c<='9')
       return c-'0';
   if (c >= 'A' && c<='Z')
       return c -'A'+10;
   if (c >= 'a' && c<='z')
       return c -'a'+10;
   return 0;
}

int crcBase36(char* test) {
    int result = htoi(test[0]) * 36 + htoi(test[1]);
    return result;
}

void loop() {
  if (Serial.available() > 0) {
    Serial.readBytesUntil('\n', switchValue, 32);
    char test[8];
    sscanf(switchValue, "%2s:%d", test, &switchOnOff);
    receivedLoc = crcBase36(test);
    if (switchOnOff == 0) {
//      cellTimings[receivedLoc] = millis();
    }
    Serial.print("Location: ");
    Serial.println(String(receivedLoc, 36));
    Serial.print("Switch state: ");
    Serial.println(switchOnOff);
  }

  if (switchOnOff == 1) {
      SimbleeForMobile.showScreen(4);
    switchOnOff = -1;
  } else if (switchOnOff == 0) {
    SimbleeForMobile.showScreen(3);
    switchOnOff = -1;
  }

  if (SimbleeForMobile.updatable) {
    if (updateTrue != 0) {
        wineTable.update_table(updateTrue);
        wineTable.update_table(updateTrue);
        wineTable.update_table(updateTrue);
        updateTrue = 0;
    }
  }

  SimbleeForMobile.process();
}

void ui() {
  if (SimbleeForMobile.screen == currentScreen) return;

  currentScreen = SimbleeForMobile.screen;
  switch (SimbleeForMobile.screen) {
    case 1:
      createStasisScreen();
      updateTrue = 's';
//      wineTable.update_table('s');
      break;

    case 2:
      createAnomalyScreen();
      updateTrue = 'l';
//        wineTable.update_table('l');
      break;

    case 3:
      createExtractionScreen();
//      updateTrue = 'l';
//        wineTable.update_table('l');
      break;

    case 4:
      createInsertionScreen();
      updateTrue = 'l';
//    wineTable.update_table('l');
      break;

    default:
      Serial.print("ui: Unknown screen requested: ");
      Serial.println(SimbleeForMobile.screen);
  }
}

void addToInventory(uint8_t inputEventID) {
  selectedBottle = wineTable.get_record_by_button_id(inputEventID, 'l');
  selectedBottle->updateLocation(receivedLoc);
  selectedBottle->updateState('s');
  selectedBottle = NULL;
}

void removeFromInventory(uint8_t inputEventID) {
    Serial.println("Okay");
  selectedBottle = wineTable.get_record_by_button_id(inputEventID, 's');
  selectedBottle->updateLocation(-1);
  selectedBottle->updateState('l');
  selectedBottle = NULL;
}

void ui_event(event_t &event) {
  eventID = event.id;

  printEvent(event);
  if (wineTable.find_button_id(eventID) && event.type == EVENT_RELEASE && currentScreen == 1) {
    SimbleeForMobile.showScreen(2);
  } else if (wineTable.find_button_id(eventID) && event.type == EVENT_RELEASE && currentScreen == 2) {
    SimbleeForMobile.showScreen(1);
  } else if (wineTable.find_button_id(eventID) && event.type == EVENT_RELEASE && currentScreen == 4) {
    addToInventory(eventID);
    SimbleeForMobile.showScreen(1);
  } else if (eventID == extractionDrinkingButton && event.type == EVENT_RELEASE && currentScreen == 3) {
    removeFromInventory(eventID);
    SimbleeForMobile.showScreen(1);
  } else if (eventID == extractionRecyclingButton && event.type == EVENT_RELEASE && currentScreen == 3) {
    removeFromInventory(eventID);
    SimbleeForMobile.showScreen(1);
  }

  //  if (stasisTable.find_button_id(eventID)) {
  //    SimbleeForMobile.updateText(eventID, "CLICKED");
  //  //	SimbleeForMobile.showScreen(2);
  //  }

  //  if (stasisTable.find_label_id(eventID)) {
  //    SimbleeForMobile.updateText(eventID, "CLICKED");
  //  }
}
