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

uint8_t eventID;
const char* STASIS = "INVENTORY";
const char* ANOMALY = "WINE TO STORE";

const char* const bullDozer[2] = {STASIS, ANOMALY};
int currentScreen;
Record* selectedBottle;
uint8_t extractionDrinkingButton;
uint8_t extractionRecyclingButton;

char switchValue[64];
String receivedLoc = "";
int switchOnOff = -1;
vector<Record> removedBottles;
std::map<int, long> cellTimings;

SimbleeTable stasisTable = SimbleeTable();
SimbleeTable anomalyTable = SimbleeTable();

void SimbleeForMobile_onConnect() {
  currentScreen = -1;
}

void createStasisScreen() {
  color_t darkgray = rgb(85, 85, 85);
  SimbleeForMobile.beginScreen(darkgray);
  stasisTable.draw_table(100, "WINE INVENTORY");
  stasisTable.update_table();
  SimbleeForMobile.endScreen();
}

void createAnomalyScreen() {
  color_t darkgray = rgb(85, 85, 85);
  SimbleeForMobile.beginScreen(darkgray);
  
  anomalyTable.draw_table(100, "WINE LIMBO");
  
  anomalyTable.add_record(Record("Los Reveles"));
  anomalyTable.add_record(Record("Francis Coppola"));
  anomalyTable.add_record(Record("Proximo"));
  anomalyTable.add_record(Record("Perez Cruz"));
  anomalyTable.add_record(Record("Baron de Marny"));
  anomalyTable.add_record(Record("Prieure de Cenac"));

  anomalyTable.update_table();
  
  SimbleeForMobile.endScreen();
}

void createExtractionScreen() {
  color_t darkgray = rgb(85, 85, 85);
  SimbleeForMobile.beginScreen(darkgray);

  int extractionText = SimbleeForMobile.drawText(80, 60,  selectedBottle->getWineName().c_str(), BLACK, 40);
  extractionDrinkingButton = SimbleeForMobile.drawButton(100, 200, 100, "Drank");
  extractionRecyclingButton = SimbleeForMobile.drawButton(100, 200, 100, "Gifted");

  SimbleeForMobile.setEvents(extractionDrinkingButton, EVENT_RELEASE);
  SimbleeForMobile.setEvents(extractionRecyclingButton, EVENT_RELEASE);
    
  SimbleeForMobile.endScreen();
}

void createInsertionScreen() {
  color_t darkgray = rgb(85, 85, 85);
  SimbleeForMobile.beginScreen(darkgray);
  anomalyTable.draw_table(100, "SELECT BOTTLE");
  anomalyTable.update_table();
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



int charToValueInBase36(char inputChar) {
  if (inputChar >= '0' && inputChar <= '9') {
    return inputChar - '0';
  }
  if (inputChar >= 'A' && inputChar <= 'Z') {
    return inputChar - 'A' + 10;
  }
  return 0;
}

uint16_t strToBase36(String inputString) {
  int secondTerm = charToValueInBase36(inputString[0]) * 36;
  int firstTerm = charToValueInBase36(inputString[1]);
  
  return (firstTerm + secondTerm);
}



void setup() {
  Serial.begin(9600);
  SimbleeForMobile.advertisementData = "DasChill";
  SimbleeForMobile.begin();
}

void loop() {
  if (Serial.available() > 0) {
    Serial.readBytesUntil('\n', switchValue, 32);
    char test[8];
    sscanf(switchValue, "%2s:%d", test, &switchOnOff);
    receivedLoc = String(test).substr(0,2);
    if (switchOnOff == 0) {
      cellTimings[strToBase36(receivedLoc)] = millis();
    }
    Serial.print("Location: ");
    Serial.println(receivedLoc.c_str());
    Serial.print("Switch state: ");
    Serial.println(switchOnOff);
  }

  if (switchOnOff == 1) {
    //  Wine bottle has been inserted.
    std::map<int, long>::iterator it = cellTimings.find(strToBase36(receivedLoc));
    if (it != cellTimings.end() && ((millis() - it->second) < 5000)) {
      cellTimings.erase(it);
      return;
    } else if (it == cellTimings.end()) {
      anomalyTable.update_table();
      SimbleeForMobile.showScreen(4);
    }
    switchOnOff = -1;
  }

  if (SimbleeForMobile.updatable) {
    if (currentScreen != 3 && currentScreen != 4) {
      for (std::map<int, long>::iterator it = cellTimings.begin(); it != cellTimings.end(); ++it) {
        if ((millis() - it->second) > 5000) {
          selectedBottle = stasisTable.get_record_by_loc(it->first);
          SimbleeForMobile.showScreen(3);
          switchOnOff = -1;
          return;
        }
      }
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
      break;

    case 2:
      createAnomalyScreen();
      break;

    case 3:
      createExtractionScreen();
      break;

    case 4:
      createInsertionScreen();
      break;

    default:
      Serial.print("ui: Unknown screen requested: ");
      Serial.println(SimbleeForMobile.screen);
  }
}

void addToInventory(uint8_t inputEventID) {
  selectedBottle = anomalyTable.get_record_by_button_id(inputEventID);

  Record newBottle = Record(selectedBottle->getWineName());
  newBottle.updateLocation(strToBase36(receivedLoc));
  anomalyTable.del_record(selectedBottle);
  Serial.println(newBottle.getWineName().c_str());
  stasisTable.add_record(newBottle);

//  stasisTable.update_table(); 
}

void ui_event(event_t &event) {
  eventID = event.id;

  printEvent(event);
  if (stasisTable.find_button_id(eventID) && event.type == EVENT_RELEASE && currentScreen == 1) {
    SimbleeForMobile.showScreen(2);
  } else if (anomalyTable.find_button_id(eventID) && event.type == EVENT_RELEASE && currentScreen == 2) {
    SimbleeForMobile.showScreen(1);
    Serial.println(stasisTable.get_total_records());
  } else if (anomalyTable.find_button_id(eventID) && event.type == EVENT_RELEASE && currentScreen == 4) {
    addToInventory(eventID);
    SimbleeForMobile.showScreen(1);
  } else if (eventID == extractionDrinkingButton && event.type == EVENT_RELEASE && currentScreen == 3) {
//    stasisTable.del_record(selectedBottle);
    SimbleeForMobile.showScreen(1);
  } else if (eventID == extractionRecyclingButton && event.type == EVENT_RELEASE && currentScreen == 3) {
//    stasisTable.del_record(selectedBottle);
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
