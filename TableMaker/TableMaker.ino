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
int currentScreen;
Record* selectedBottle;
char updateTrue = 0;

char switchValue[64];
int receivedLoc = -1;
int switchOnOff = -1;
vector<Record> removedBottles;

//  Extraction Screen Buttons
uint8_t extractionDrinkingButton;
uint8_t extractionRecyclingButton;

//  List Screen Buttons
uint8_t listPurchasedButton;
uint8_t listBuyButton;
uint8_t listViewInventoryButton;

std::map<uint8_t, uint8_t> resetButtons;

//	Thanks screen timer
unsigned long thankYouTimer;

SimbleeTable wineTable = SimbleeTable();

void SimbleeForMobile_onConnect() {
	currentScreen = -1;
}

void SimbleeForMobile_onDisconnect() {
	currentScreen  = -1;
	switchOnOff = -1;
	receivedLoc = -1;
}

void createResetButton() {
    color_t fuschia = rgb(255, 0, 128);
    uint8_t buyScreenResetButton = SimbleeForMobile.drawButton(PERM_X - 105, PERM_Y - 30, 100, "RESET DEMO", fuschia, TEXT_TYPE);
    SimbleeForMobile.setEvents(buyScreenResetButton, EVENT_RELEASE);
    resetButtons[currentScreen] = buyScreenResetButton;
}

void createBuyScreen() {
	SimbleeForMobile.beginScreen(WHITE);
	color_t fuschia = rgb(255, 0, 128);

	SimbleeForMobile.drawText(100, 60,  "GLASSY", BLACK, 40);
	listBuyButton = SimbleeForMobile.drawButton(60, 150, 200, "BUY WINE", fuschia, BOX_TYPE);
	listViewInventoryButton = SimbleeForMobile.drawButton(60, 200, 200, "VIEW INVENTORY", fuschia, BOX_TYPE);
    createResetButton();

	SimbleeForMobile.setEvents(listBuyButton, EVENT_RELEASE);
	SimbleeForMobile.endScreen();
}

void createBottlePlacementInstructionScreen() {
	SimbleeForMobile.beginScreen(WHITE);
	SimbleeForMobile.drawText(35, 200,  "PLACE BOTTLE\n IN WINE RACK", BLACK, 40);
    createResetButton();
	SimbleeForMobile.endScreen();
}

void createBottleRemovalInstructionScreen() {
	SimbleeForMobile.beginScreen(WHITE);
	SimbleeForMobile.drawText(25, 160,  "REMOVE BOTTLE\n     FROM WINE\n           RACK", BLACK, 40);
    createResetButton();
	SimbleeForMobile.endScreen();
}

void createThanksScreen() {
	SimbleeForMobile.beginScreen(WHITE);
	int listText = SimbleeForMobile.drawText(50, 200,  "THANK YOU!!", BLACK, 40);
	thankYouTimer = millis();
    createResetButton();
	SimbleeForMobile.endScreen();
}

void createExtractionScreen() {
	color_t darkgray = rgb(85, 85, 85);
	color_t fuschia = rgb(255, 0, 128);
	SimbleeForMobile.beginScreen(WHITE);

	int extractionText = SimbleeForMobile.drawText(30, 90,  "WINE REMOVED", BLACK, 40);
	extractionDrinkingButton = SimbleeForMobile.drawButton(110, 200, 100, "DRANK", fuschia, BOX_TYPE);
	extractionRecyclingButton = SimbleeForMobile.drawButton(110, 250, 100, "GIFTED", fuschia, BOX_TYPE);

	SimbleeForMobile.setEvents(extractionDrinkingButton, EVENT_RELEASE);
	SimbleeForMobile.setEvents(extractionRecyclingButton, EVENT_RELEASE);
    createResetButton();
	SimbleeForMobile.endScreen();
}

void createInsertionScreen() {
	color_t darkgray = rgb(85, 85, 85);
	SimbleeForMobile.beginScreen(WHITE);
	wineTable.draw_table(100, "SELECT BOTTLE");
    createResetButton();
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

void addToInventory(uint8_t inputEventID) {
	selectedBottle = wineTable.get_record_by_button_id(inputEventID, 'l');
	if (selectedBottle == NULL) {
		return;
	}
	selectedBottle->updateLocation(receivedLoc);
	selectedBottle->updateState('s');
	selectedBottle = NULL;
}

void removeFromInventory() {
	Serial.println("Okay");
	selectedBottle = wineTable.get_record_by_loc(receivedLoc);
	if (selectedBottle == NULL) {
		return;
	}
	selectedBottle->updateLocation(-1);
	selectedBottle->updateState('l');
	selectedBottle = NULL;
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

void resetDemo() {
    wineTable.resetRowsTo('l');
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

void loop() {
	if (Serial.available() > 0) {
		Serial.readBytesUntil('\n', switchValue, 32);
		char test[8];
		sscanf(switchValue, "%2s:%1d", test, &switchOnOff);
		receivedLoc = crcBase36(test);
		Serial.print("Location: ");
		Serial.println(String(receivedLoc, 36));
		Serial.print("Switch state: ");
		Serial.println(switchOnOff);
	}

	if (SimbleeForMobile.updatable) {
		if (switchOnOff == 1) {
			SimbleeForMobile.showScreen(3);
			switchOnOff = -1;
		} else if (switchOnOff == 0) {
			SimbleeForMobile.showScreen(6);
			switchOnOff = -1;
		}

		if (updateTrue != 0) {
			wineTable.update_table(updateTrue);
			updateTrue = 0;
		}

		if (currentScreen == 4 || currentScreen == 7) {
			if ((millis() - thankYouTimer) > 2000) {
				thankYouTimer = 0;
				int test = (currentScreen + 1) % 7;
				SimbleeForMobile.showScreen(test);
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
			createBuyScreen();
			break;

		case 2:
			createBottlePlacementInstructionScreen();
			break;

		case 3:
			createInsertionScreen();
			updateTrue = 'l';
			break;
		
		case 4:
			createThanksScreen();
			break;

		case 5:
			createBottleRemovalInstructionScreen();
			break;

		case 6:
			createExtractionScreen();
			break;

		case 7:
			createThanksScreen();
			break;

		default:
			Serial.print("ui: Unknown screen requested: ");
			Serial.println(SimbleeForMobile.screen);
	}
}

void ui_event(event_t &event) {
	eventID = event.id;

	printEvent(event);

//    std::map<uint8_t, uint8_t>::iterator buttonID_it = resetButtons.begin();

	if (eventID == listBuyButton && event.type == EVENT_RELEASE && currentScreen == 1) {
		SimbleeForMobile.showScreen(2);
	} else if (eventID == listPurchasedButton && event.type == EVENT_RELEASE && currentScreen == 2) {
		SimbleeForMobile.showScreen(3);
	} else if (wineTable.find_button_id(eventID) && event.type == EVENT_RELEASE && currentScreen == 3) {
		addToInventory(eventID);
		SimbleeForMobile.showScreen(4);
	} else if (eventID == extractionDrinkingButton && event.type == EVENT_RELEASE && currentScreen == 6) {
		removeFromInventory();
		SimbleeForMobile.showScreen(7);
	} else if (eventID == extractionRecyclingButton && event.type == EVENT_RELEASE && currentScreen == 6) {
		removeFromInventory();
		SimbleeForMobile.showScreen(7);
	} else if (resetButtons.find(currentScreen)->second == eventID && event.type == EVENT_RELEASE) {
        resetDemo();
        SimbleeForMobile.showScreen(1);
	}
}
