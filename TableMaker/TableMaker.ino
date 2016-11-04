#include <RBD_Button.h>
#include <RBD_Timer.h>
#include "SimbleeTable.h"
#include <stdlib.h>


#include <dmx.h>
#include <fastpin.h>
#include <fastspi.h>
#include <cpp_compat.h>
#include <pixeltypes.h>
#include <colorutils.h>
#include <chipsets.h>
#include <noise.h>
#include <colorpalettes.h>
#include <fastled_progmem.h>
#include <led_sysdefs.h>
#include <lib8tion.h>
#include <bitswap.h>
#include <hsv2rgb.h>
#include <fastspi_types.h>
#include <fastled_config.h>
#include <fastspi_dma.h>
#include <controller.h>
#include <color.h>
#include <pixelset.h>
#include <platforms.h>
#include <fastled_delay.h>
#include <fastspi_nop.h>
#include <fastspi_bitbang.h>
#include <fastspi_ref.h>
#include <FastLED.h>
#include <power_mgt.h>


#define LED_PIN 5
#define NUM_LEDS 6
#define SW1 2
#define SW2 3
#define SW3 4
#define SW4 7
#define SW5 8
#define SW6 9
#define color CRGB::Purple
#define INSERT_TIMER_VALUE 3000
#define FIND_BOT 15000

std::map<String, int> MatrixPos;

uint8_t eventID;
int currentScreen;
Record* selectedBottle;
char updateTrue = 0;

String switchValue = "";
int receivedLoc = -1;
vector<Record> removedBottles;
cString wineBottleToBeRemoved;

//  Extraction Screen Buttons
uint8_t extractionDrinkingButton;
uint8_t extractionRecyclingButton;

//  List Screen Buttons
uint8_t listPurchasedButton;
uint8_t listBuyButton;
uint8_t listViewInventoryButton;
uint8_t returnToTitleScreenButton;

std::map<uint8_t, uint8_t> resetButtons;

//	Thanks screen timer
unsigned long thankYouTimer;

SimbleeTable wineTable = SimbleeTable();

CRGB leds[NUM_LEDS];

RBD::Button button1(SW1, true);
RBD::Button button2(SW2, true);
RBD::Button button3(SW3, true);
RBD::Button button4(SW4, true);
RBD::Button button5(SW5, true);
RBD::Button button6(SW6, true);

RBD::Timer button1Timer(INSERT_TIMER_VALUE);
RBD::Timer button2Timer(INSERT_TIMER_VALUE);
RBD::Timer button3Timer(INSERT_TIMER_VALUE);
RBD::Timer button4Timer(INSERT_TIMER_VALUE);
RBD::Timer button5Timer(INSERT_TIMER_VALUE);
RBD::Timer button6Timer(INSERT_TIMER_VALUE);

void SimbleeForMobile_onConnect() {
	currentScreen = -1;
}

void SimbleeForMobile_onDisconnect() {
	currentScreen  = -1;
  Serial.println(18);
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
    selectedBottle = wineTable.get_record_by_loc(receivedLoc);
    if (selectedBottle == NULL) {
        return;
    }
    wineBottleToBeRemoved = selectedBottle->getWineName();
    selectedBottle->updateLocation(-1);
    selectedBottle->updateState('l');
    selectedBottle = NULL;
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
	listBuyButton = SimbleeForMobile.drawButton(60, 150, 200, "ADD TO INVENTORY", fuschia, BOX_TYPE);
	listViewInventoryButton = SimbleeForMobile.drawButton(60, 200, 200, "VIEW INVENTORY", fuschia, BOX_TYPE);
    createResetButton();

	SimbleeForMobile.setEvents(listBuyButton, EVENT_RELEASE);
    SimbleeForMobile.setEvents(listViewInventoryButton, EVENT_RELEASE);
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
	color_t fuschia = rgb(255, 0, 128);
	SimbleeForMobile.beginScreen(WHITE);

    removeFromInventory();
     
	int extractionText = SimbleeForMobile.drawText(30, 90,  "WINE REMOVED", BLACK, 40);
	int wineRemovedText = SimbleeForMobile.drawText(40, 140, wineBottleToBeRemoved.c_str(), BLACK, 40);

	extractionDrinkingButton = SimbleeForMobile.drawButton(110, 200, 100, "DRANK", fuschia, BOX_TYPE);
	extractionRecyclingButton = SimbleeForMobile.drawButton(110, 250, 100, "GIFTED", fuschia, BOX_TYPE);

	SimbleeForMobile.setEvents(extractionDrinkingButton, EVENT_RELEASE);
	SimbleeForMobile.setEvents(extractionRecyclingButton, EVENT_RELEASE);
  createResetButton();
  wineBottleToBeRemoved = "";
	SimbleeForMobile.endScreen();
}

void createInsertionScreen() {
	SimbleeForMobile.beginScreen(WHITE);
	wineTable.draw_table(100, "SELECT BOTTLE");
    createResetButton();
	SimbleeForMobile.endScreen();
}

void createInventoryScreen() {
  color_t fuschia = rgb(255, 0, 128);
	SimbleeForMobile.beginScreen(WHITE);
	wineTable.draw_table(100, "WINE INVENTORY");
	returnToTitleScreenButton = SimbleeForMobile.drawButton(10, 100, 100, "MAIN", fuschia, BOX_TYPE);
  createResetButton();
  SimbleeForMobile.setEvents(returnToTitleScreenButton, EVENT_RELEASE);
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

int htoi (char c) {  //does not check that input is valid
	if (c >= '0' && c<='9')
		return c-'0';
	if (c >= 'A' && c<='Z')
		return c -'A'+10;
	if (c >= 'a' && c<='z')
		return c -'a'+10;
	return 0;
}

int crcBase36(const String test) {
	int result = htoi(test[0]) * 36 + htoi(test[1]);
	return result;
}

void resetDemo() {
    wineTable.resetRowsTo('l');
}

void checkButtonState(RBD::Button& toButton, RBD::Timer& toTimer, CRGB& someLED, int cellID) {
  if (toButton.onPressed()) {
    Serial.println("Button2 Pressed");
    someLED = color;
    toTimer.restart();
	SimbleeForMobile.showScreen(3);
//    receivedLoc = crcBase36(MatrixPos[cellID]);

    std::map<String, int>::iterator it;

    for (it = MatrixPos.begin(); it != MatrixPos.end(); ++it ) {
        if (it->second == cellID) {
            receivedLoc = crcBase36(it->first);
            break;
        }
    }

    if (it == MatrixPos.end()) {
        receivedLoc = 0;
    }
   
    
  } else if (toButton.onReleased()) {
	SimbleeForMobile.showScreen(6);
  } else if (toTimer.onExpired()) {
    someLED = CRGB::Black;
    toTimer.setTimeout(INSERT_TIMER_VALUE);
  }
}
 
void setup() {
	Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);

	SimbleeForMobile.advertisementData = "WineChiller";
	SimbleeForMobile.begin();
    MatrixPos["a1"] = 0;
    MatrixPos["a2"] = 1;
    MatrixPos["a3"] = 2;
    MatrixPos["b1"] = 3;
    MatrixPos["b2"] = 4;
    MatrixPos["b3"] = 5;
	wineTable.add_record("Los Reveles");
	wineTable.add_record("Francis Coppola");
	wineTable.add_record("Proximo");
	wineTable.add_record("Perez Cruz");
	wineTable.add_record("Baron de Marny");
	wineTable.add_record("Prieure de Cenac");
}

void loop() {

  checkButtonState(button1, button1Timer, leds[3], 0);
  checkButtonState(button2, button2Timer, leds[4], 1);
  checkButtonState(button3, button3Timer, leds[5], 2);
  checkButtonState(button4, button4Timer, leds[2], 3);
  checkButtonState(button5, button5Timer, leds[1], 4);
  checkButtonState(button6, button6Timer, leds[0], 5);

//	if (Serial.available() > 0) {
//		Serial.readBytesUntil('\n', switchValue, 32);
//		char test[8];
//		sscanf(switchValue, "%2s:%1d", test, &switchOnOff);
//		receivedLoc = crcBase36(test);
//#ifdef DEBUG
//		Serial.print("Location: ");
//		Serial.println(String(receivedLoc, 36));
//		Serial.print("Switch state: ");
//		Serial.println(switchOnOff);
//#endif        
//	}

	if (SimbleeForMobile.updatable) {
		if (updateTrue != 0) {
			wineTable.update_table(updateTrue);
			updateTrue = 0;
		}

		if (currentScreen == 4 || currentScreen == 7) {
			if ((millis() - thankYouTimer) > 2000) {
				thankYouTimer = 0;
				SimbleeForMobile.showScreen(1);
			}
		}
	}


  int lumens = quadwave8(beat8(72));
  FastLED.setBrightness(lumens);
  FastLED.show();
	SimbleeForMobile.process();
}

void toggleSearchLight(String& targLocWine) {
 // using std::find with vector and iterator:  
    int value = MatrixPos[targLocWine];
    switch (value) {
      case 0: 
        leds[3] = color;
        button1Timer.setTimeout(FIND_BOT);     
        button1Timer.restart();
        break;

      case 1: 
        leds[4] = color;
        button2Timer.setTimeout(FIND_BOT);     
        button2Timer.restart();
        break;

      case 2: 
        leds[5] = color;
        button3Timer.setTimeout(FIND_BOT);     
        button3Timer.restart();
        break;

      case 3: 
        leds[2] = color;
        button4Timer.setTimeout(FIND_BOT);     
        button4Timer.restart();
        break;

      case 4: 
        leds[1] = color;
        button5Timer.setTimeout(FIND_BOT);     
        button5Timer.restart();
        break;

      case 5: 
        leds[0] = color;
        button6Timer.setTimeout(FIND_BOT);     
        button6Timer.restart();
        break;

      default:
        Serial.println("Sorry....");
        break;
    }

  
    
}

void ui() {
	if (SimbleeForMobile.screen == currentScreen) return;

	currentScreen = SimbleeForMobile.screen;

	//	18, 21, and 42 refer to the IDs of "Store, Add, and Remove"
	//		storyboard web pages.
	switch (SimbleeForMobile.screen) {
		case 1:
			createBuyScreen();
			Serial.println(18);
			break;

		case 2:
			createBottlePlacementInstructionScreen();
			Serial.println(21);
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
			Serial.println(42);
			break;

		case 6:
			createExtractionScreen();
			break;

		case 7:
			createThanksScreen();
			break;

		case 8:
			createInventoryScreen();
			updateTrue = 's';
			break;

		default:
#ifdef DEBUG        
			Serial.print("ui: Unknown screen requested: ");
			Serial.println(SimbleeForMobile.screen);
#endif      
            break;
	}
}

void ui_event(event_t &event) {
	eventID = event.id;

#ifdef DEBUG
	printEvent(event);
#endif  

//    std::map<uint8_t, uint8_t>::iterator buttonID_it = resetButtons.begin();

	if (eventID == listBuyButton && event.type == EVENT_RELEASE && currentScreen == 1) {
		SimbleeForMobile.showScreen(2);
	} else if (eventID == listPurchasedButton && event.type == EVENT_RELEASE && currentScreen == 2) {
		SimbleeForMobile.showScreen(3);
	} else if (wineTable.find_button_id(eventID) && event.type == EVENT_RELEASE && currentScreen == 3) {
		addToInventory(eventID);
		SimbleeForMobile.showScreen(4);
	} else if (eventID == extractionDrinkingButton && event.type == EVENT_RELEASE && currentScreen == 6) {
		SimbleeForMobile.showScreen(7);
	} else if (eventID == extractionRecyclingButton && event.type == EVENT_RELEASE && currentScreen == 6) {
		SimbleeForMobile.showScreen(7);
	} else if (resetButtons.find(currentScreen)->second == eventID && event.type == EVENT_RELEASE) {
		resetDemo();
		SimbleeForMobile.showScreen(1);
	} else if (eventID == listViewInventoryButton && event.type == EVENT_RELEASE && currentScreen == 1) {
		SimbleeForMobile.showScreen(8);
	}	else if (eventID == returnToTitleScreenButton && event.type == EVENT_RELEASE && currentScreen == 8) {
		SimbleeForMobile.showScreen(1);
	} else if (wineTable.find_button_id(eventID) && event.type == EVENT_RELEASE && currentScreen == 8) {
        selectedBottle = wineTable.get_record_by_button_id(eventID, 's');
        String ohplease = String(selectedBottle->getWineLocation(), 36);
        Serial.println(ohplease);
        toggleSearchLight(ohplease);
  }
}
