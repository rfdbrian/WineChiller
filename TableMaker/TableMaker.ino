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
Record selectedBottle;
uint8_t extractionDrinkingButton;
uint8_t extractionRecyclingButton;

char switchValue[64];
String receivedLoc = "";
int switchOnOff = -1;
vector<Record> removedBottles;
std::map<int,long> cellTimings;

SimbleeTable stasisTable;
SimbleeTable anomalyTable; 
SimbleeTable bottleSelectionTable;


void SimbleeForMobile_onConnect() {
  currentScreen = -1;
}  

/*
 * Create the first screen.
 * 
 * The screen simply consists of a label displaying the screen number
 * and a button which swaps screens. Register for events on the button
 * such that we receive notifications when it is pressed.
 */
void createStasisScreen() {
  //
  // Portrait mode is the default, so that isn't required
  // here, but shown for completeness. LANDSCAPE could be
  // used for that orientation.
  //
  SimbleeForMobile.beginScreen(YELLOW, PORTRAIT);

//  int textID = SimbleeForMobile.drawText(80, 60, "Screen 1", BLACK, 40);
//  toScreen2ButtonID = SimbleeForMobile.drawButton(100, 200, 100, "Screen 2");

  color_t darkgray = rgb(50,50,50);
  SimbleeForMobile.beginScreen(darkgray);
  stasisTable = SimbleeTable(100);
  SimbleeForMobile.endScreen();

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

void createAnomalyScreen() {
  //
  // Default to Portrait orientation
  //


  color_t darkgray = rgb(85,85,85);
  SimbleeForMobile.beginScreen(darkgray);
  anomalyTable = SimbleeTable(100);
  SimbleeForMobile.endScreen();
}

void createExtractionScreen() {
  //
  // Portrait mode is the default, so that isn't required
  // here, but shown for completeness. LANDSCAPE could be
  // used for that orientation.
  //
  color_t darkgray = rgb(50,50,50);
  SimbleeForMobile.beginScreen(darkgray);

  int extractionText = SimbleeForMobile.drawText(80, 60,  selectedBottle.getWineName().c_str(), BLACK, 40);
  extractionDrinkingButton = SimbleeForMobile.drawButton(100, 200, 100, "Drank");
  extractionRecyclingButton = SimbleeForMobile.drawButton(100, 200, 100, "Gifted");

  SimbleeForMobile.setEvents(extractionDrinkingButton, EVENT_RELEASE);
  SimbleeForMobile.setEvents(extractionRecyclingButton, EVENT_RELEASE);
  SimbleeForMobile.endScreen();
}

void createInsertionScreen() {
  //
  // Portrait mode is the default, so that isn't required
  // here, but shown for completeness. LANDSCAPE could be
  // used for that orientation.
  //
//  int textID = SimbleeForMobile.drawText(80, 60, "Screen 1", BLACK, 40);
//  toScreen2ButtonID = SimbleeForMobile.drawButton(100, 200, 100, "Screen 2");

  color_t darkgray = rgb(50,50,50);
  SimbleeForMobile.beginScreen(darkgray);
  bottleSelectionTable = SimbleeTable(100);
  SimbleeForMobile.endScreen();

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
  
/**
 * TODO: Reset UI-demo
 */
void SimbleeForMobile_onDisconnect() {
}

int charToValueInBase36(char inputChar) {
    if (inputChar >= '0' && inputChar <= '9') {
        return inputChar - '0'; 
    } 
    if (inputChar >= 'A' && inputChar <= 'Z') {
        return inputChar - 'A' + 10;
    }
    return -1;
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
		sscanf(switchValue, "%s:%d", &receivedLoc, &switchOnOff);
		if (switchOnOff == 0) {
			cellTimings[strToBase36(receivedLoc)] = millis();
		}
		Serial.println(switchValue);
	}

  if (SimbleeForMobile.updatable) {
		if (currentScreen != 3 && currentScreen != 4) {
			for (std::map<int,long>::iterator it = cellTimings.begin(); it != cellTimings.end(); ++it) {
				if ((millis() - it->second) > 5000) {
					selectedBottle = stasisTable.get_record_by_loc(it->first);
					SimbleeForMobile.showScreen(3);
					return;
				}
			}
		}
					 	
		if (switchOnOff == 1) {
			//	Wine bottle has been inserted.
			std::map<int, long>::iterator it = cellTimings.find(strToBase36(receivedLoc));
			if (it != cellTimings.end() && ((millis() - it->second) < 5000)) {
				cellTimings.erase(it);
				return;
			} else if (it != cellTimings.end()) {
				SimbleeForMobile.showScreen(4);
		  	switchOnOff = -1;
			}
		}
	}

  SimbleeForMobile.process();
}

void ui() {
  if(SimbleeForMobile.screen == currentScreen) return;
  
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
	selectedBottle = bottleSelectionTable.get_record_by_button_id(inputEventID);
	bottleSelectionTable.del_record(selectedBottle);

	selectedBottle.updateLocation(strToBase36(receivedLoc));
	stasisTable.add_record(selectedBottle);
}

void ui_event(event_t &event) {
  eventID = event.id;

  printEvent(event);
  if (stasisTable.find_button_id(eventID) && event.type == EVENT_RELEASE && currentScreen == 1) {
    SimbleeForMobile.showScreen(2);
  } else if (anomalyTable.find_button_id(eventID) && event.type == EVENT_RELEASE && currentScreen == 2) {
    SimbleeForMobile.showScreen(1);
  } else if (bottleSelectionTable.find_button_id(eventID) && event.type == EVENT_RELEASE && currentScreen == 4) {
		addToInventory(eventID);	
		SimbleeForMobile.showScreen(1);
	} else if (eventID == extractionDrinkingButton && event.type == EVENT_RELEASE && currentScreen == 3) {
		stasisTable.del_record(selectedBottle);
		SimbleeForMobile.showScreen(1);
	} else if (eventID == extractionRecyclingButton && event.type == EVENT_RELEASE && currentScreen == 3) {
		stasisTable.del_record(selectedBottle);
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
