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
#include "Slot.h"
#include "Wine.h"
#include <SimbleeForMobile.h>
#include <vector>

#include "WineImages.h"

#define GE_RED rgb(158,11,15)
#define GE_YELLOW rgb(243,165,25)
#define GE_GRAY rgb(159,159,159)
#define GE_DARKGRAY rgb(123,123,123)
#define GE_LIGHTGRAY rgb(194,193,193)
#define GE_BEIGE rgb(216,208,200)

#define color CRGB::Red
#define color2 CRGB::Green

#define LED_PIN 5
#define NUM_LEDS 25

#define numberOfWines 10
#define INSERT_WINE_TIMEOUT 3000

#define DEMO 1

#define WineSpashScreen 1
#define ColumbiaCrest1 2
#define ColumbiaCrest2 3
#define Carmen1 4
#define Carmen2 5
#define CoppolaShiraz1 6
#define CoppolaShiraz2 7
#define LaTercera1 8
#define LaTercera2 9
#define Martini1 10
#define Martini2 11
#define CoppolaCab1 12
#define CoppolaCab2 13
#define Prieure1 14
#define Prieure2 15
#define PerezCruz1 16
#define PerezCruz2 17
#define Proximo1 18
#define Proximo2 19

CRGB leds[NUM_LEDS];

std::vector<Slot> Slots;

bool disconnected = false;
bool initialized = false;
bool updatePage = true;
bool addMode = false;

uint32_t nextButtonState[25];
uint32_t currentButtonState[25];
int count = 0;

//UI ELEMENTS
uint8_t previousPage, nextPage, pageValue,
        insertScreen1, insertScreen2, insertScreen3, insertScreen4, insertScreen5, insertScreen6, insertScreen7,
        addButton,
        removeScreen1, removeScreen2, removeScreen3, removeScreen4,
        removeButton, returnButton,
        VineyardUI[5], WineName1UI[5], WineName2UI[5], CountryUI[5], PriceUI[5], LocationUI[5], OverlayUI[5],
        VineyardUISingle, WineName1UISingle, WineName2UISingle, CountryUISingle, PriceUISingle, LocationUISingle,
        addPopUpDetectedText, addPopUpRackText, addPopUpRackNumber, addPopUpSelectText,
        removePopUpDetectedText, removePopUpRackText, removePopUpRackNumber, removePopUpSelectText,
        screen2Title, screen2Background;

int8_t winePage = 1,
       addDetected = -1,
       removeDetected = -1,
       switchDetected = -1,
       clickedOverlay = 0;

char *ChillerSlot[25] = {"A1", "A2", "A3", "A4", "A5",
                         "B1", "B2", "B3", "B4", "B5",
                         "C1", "C2", "C3", "C4", "C5",
                         "D1", "D2", "D3", "D4", "D5",
                         "E1", "E2", "E3", "E4", "E5"
                        };

//  Wine.h describes a basic "Wine" class. There are the classic getter
//    methods for various static values. There exists only one setter value
//    for "index," which would be the index to the Slots vector defined below.

Wine wine[25] = {Wine("Grand Estates", "Columbia Crest",  "2003", "Cabernet Sauvignon", "$11.24", -1, 0),
                 Wine("Grand Reserva", "Carmen", "2012", "Carbenet Sauvignon", "$14.50", -1, 0),
                 Wine("Rosso & Blonco", "Francis Ford Coppola", "2011", "Shiraz", "$12.00", -1, 0),
                 Wine("Reserva Rioja", "La Tercera", "2010", "Spanish Red", "$17.98", -1, 0),
                 Wine("Sonoma County", "Louis M. Martini", "2013", "Cabernet Sauvignon", "$14.99", -1, 0),
                 Wine("Diamond Collection Ivory", "Francis Ford Coppola", "2013", "Cabernet Sauvignon", "$14.98", -1, 0),
                 Wine("Prieure de Cenac", "Chateau Saint-Didier-Parnac", "2014", "Malbec", "$7.98", -1, 0),
                 Wine("Reserva", "Perez Cruz", "2013", "Cabernet Sauvignon", "$14.46", -1, 0),
                 Wine("Proximo Roja", "Marquest De Riscal", "2010", "Spanish Red", "$9.48", -1, 0),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),

//SCREEN 1 BASE*********************************************************************************
void splashScreen() {
  SimbleeForMobile.beginScreen(WHITE, PORTRAIT);
  SimbleeForMobile.drawRect(0, 0, 320, 570, GE_RED);
  SimbleeForMobile.drawText(20, 200, "LOADING", WHITE, 50);
  SimbleeForMobile.drawText(20, 250, "INTERFACE", WHITE, 50);
  SimbleeForMobile.imageSource(1,JPG, WineSplashScreen_jpg, WineSplashScreen_jpg_len);
  SimbleeForMobile.drawImage(1, 0, 0);
  SimbleeForMobile.endScreen();
}

//SCREEN 2 BASE*********************************************************************************
void inventoryScreen() {
  SimbleeForMobile.beginScreen(WHITE, PORTRAIT);

  //BACKGROUND
  SimbleeForMobile.drawRect(0, 0, 320, 570, GE_RED);
  SimbleeForMobile.drawRect(0, 100, 320, 400, WHITE);
  SimbleeForMobile.drawRect(159, 100, 2, 400, GRAY);
  SimbleeForMobile.drawRect(0, 299, 320, 2, GRAY);
=======
        removeScreen1, removeScreen2, removeScreen3, removeScreen4,
        removeButton, returnButton,
        VineyardUI[5], WineName1UI[5], WineName2UI[5], CountryUI[5], PriceUI[5], LocationUI[5], OverlayUI[5],
        VineyardUISingle, WineName1UISingle, WineName2UISingle, CountryUISingle, PriceUISingle, LocationUISingle,
        addPopUpDetectedText, addPopUpRackText, addPopUpRackNumber, addPopUpSelectText,
        removePopUpDetectedText, removePopUpRackText, removePopUpRackNumber, removePopUpSelectText,
        screen2Title, screen2Background;

int8_t winePage = 1,
       addDetected = -1,
       removeDetected = -1,
       switchDetected = -1,
       clickedOverlay = 0;

char *ChillerSlot[25] = {"A1", "A2", "A3", "A4", "A5",
                         "B1", "B2", "B3", "B4", "B5",
                         "C1", "C2", "C3", "C4", "C5",
                         "D1", "D2", "D3", "D4", "D5",
                         "E1", "E2", "E3", "E4", "E5"
                        };

//  Wine.h describes a basic "Wine" class. There are the classic getter
//    methods for various static values. There exists only one setter value
//    for "index," which would be the index to the Slots vector defined below.

Wine wine[25] = {Wine("Grand Estates", "Columbia Crest",  "2003", "Cabernet Sauvignon", "$11.24", -1, 0),
                 Wine("Grand Reserva", "Carmen", "2012", "Carbenet Sauvignon", "$14.50", -1, 0),
                 Wine("Rosso & Blonco", "Francis Ford Coppola", "2011", "Shiraz", "$12.00", -1, 0),
                 Wine("Reserva Rioja", "La Tercera", "2010", "Spanish Red", "$17.98", -1, 0),
                 Wine("Sonoma County", "Louis M. Martini", "2013", "Cabernet Sauvignon", "$14.99", -1, 0),
                 Wine("Diamond Collection Ivory", "Francis Ford Coppola", "2013", "Cabernet Sauvignon", "$14.98", -1, 0),
                 Wine("Prieure de Cenac", "Chateau Saint-Didier-Parnac", "2014", "Malbec", "$7.98", -1, 0),
                 Wine("Reserva", "Perez Cruz", "2013", "Cabernet Sauvignon", "$14.46", -1, 0),
                 Wine("Proximo Roja", "Marquest De Riscal", "2010", "Spanish Red", "$9.48", -1, 0),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                 Wine(),
                };

int recalc(int test) {
  int ledPosition = test;
  int remainder = ledPosition % 5;
  
  if (remainder == 0) {
      ledPosition += 4;
  } else if (remainder == 1) {
      ledPosition += 2;
  } else if (remainder == 3) {
      ledPosition -= 2;
  } else if (remainder == 4) {
      ledPosition -= 4;
  }

  return ledPosition;
}

//SCREEN 1 BASE*********************************************************************************
void splashScreen() {
  SimbleeForMobile.beginScreen(WHITE, PORTRAIT);
  SimbleeForMobile.drawRect(0, 0, 320, 570, GE_RED);
  SimbleeForMobile.drawText(20, 200, "LOADING", WHITE, 50);
  SimbleeForMobile.drawText(20, 250, "INTERFACE", WHITE, 50);
  SimbleeForMobile.imageSource(1, JPG, WineSplashScreen_jpg, WineSplashScreen_jpg_len);
  SimbleeForMobile.drawImage(1, 0, 0);
  SimbleeForMobile.endScreen();
}
//************************************************************************************************

//SCREEN 2 BASE*********************************************************************************
void inventoryScreen() {
  SimbleeForMobile.beginScreen(WHITE, PORTRAIT);

  //BACKGROUND
  screen2Background = SimbleeForMobile.drawRect(0, 0, 320, 480, GE_RED);
  SimbleeForMobile.drawRect(0, 60, 280,  360, GE_LIGHTGRAY);
  SimbleeForMobile.drawRect(0, 61, 280, 70, WHITE);
  SimbleeForMobile.drawRect(0, 133, 280, 70, WHITE);
  SimbleeForMobile.drawRect(0, 205, 280, 70, WHITE);
  SimbleeForMobile.drawRect(0, 277, 280, 70, WHITE);
  SimbleeForMobile.drawRect(0, 349, 280, 70, WHITE);

  //IMAGES
  //SimbleeForMobile.imageSource(ColumbiaCrest2,JPG,ColumbiaCrest2_jpg,ColumbiaCrest2_jpg_len);
  //SimbleeForMobile.drawImage(ColumbiaCrest2,10,66);
  SimbleeForMobile.drawRect(10, 66, 50, 60, BLACK);
  //SimbleeForMobile.imageSource(Carmen2,JPG,Carmen2_jpg,Carmen2_jpg_len);
  //SimbleeForMobile.drawImage(Carmen2,10,138);
  SimbleeForMobile.drawRect(10, 138, 50, 60, BLACK);
  //SimbleeForMobile.imageSource(CoppolaShiraz2,JPG,CoppolaShiraz2_jpg,CoppolaShiraz2_jpg_len);
  //SimbleeForMobile.drawImage(CoppolaShiraz2,10,210);
  SimbleeForMobile.drawRect(10, 210, 50, 60, BLACK);
  //SimbleeForMobile.imageSource(LaTercera2,JPG,LaTercera2_jpg,LaTercera2_jpg_len);
  //SimbleeForMobile.drawImage(LaTercera2,10,282);
  SimbleeForMobile.drawRect(10, 282, 50, 60, BLACK);
  //SimbleeForMobile.imageSource(Martini1,JPG,Martini1_jpg,Martini1_jpg_len);
  //SimbleeForMobile.drawImage(Martini1,10,354);
  SimbleeForMobile.drawRect(10, 354, 50, 60, BLACK);
>>>>>>> Simblee

  //TITLE
  screen2Title = SimbleeForMobile.drawText(90, 25, "Wine Inventory", WHITE, 25);

  int boxY[5] = {64, 136, 208, 280, 352};

  int overlayY[5] = {61, 133, 205, 277, 349};

  for (int i = 0; i < 5; i++) {
    //VINEYARD
    VineyardUI[i] = SimbleeForMobile.drawText(70, boxY[i], "VINEYARD 1", GE_LIGHTGRAY, 12);
    //WINE NAME 1
    WineName1UI[i] = SimbleeForMobile.drawText(70, boxY[i] + 12, "WINE NAME", BLACK, 14);
    //WINE NAME 2
    WineName2UI[i] = SimbleeForMobile.drawText(70, boxY[i] + 25, "2006", BLACK, 14);
    //COUNTRY
    CountryUI[i] = SimbleeForMobile.drawText(70, boxY[i] + 39, "Napa Valley, United States", GE_LIGHTGRAY, 11);
    //PRICE
    PriceUI[i] = SimbleeForMobile.drawText(70, boxY[i] + 50 , "$25.00", GE_LIGHTGRAY, 10);
    //LOCATION TITLE
    SimbleeForMobile.drawText(220, boxY[i], "LOCATION", GE_LIGHTGRAY, 12);
    //LOCATION
    LocationUI[i] = SimbleeForMobile.drawText(232, boxY[i] + 15, "", BLACK, 30);
    //INVISBLE OVERLAY
    OverlayUI[i] = SimbleeForMobile.drawRect(0, overlayY[i], 280, 80, rgba(0, 0, 0, 255));
    SimbleeForMobile.setEvents(OverlayUI[i], EVENT_PRESS);
  }

  //PAGE SELECTION
  pageValue = SimbleeForMobile.drawText(295, 230, 1, WHITE, 20);
  SimbleeForMobile.drawText(297, 70, "^", WHITE, 20);
  previousPage = SimbleeForMobile.drawButton(280, 60, 40, "", WHITE, 2);
  SimbleeForMobile.setEvents(previousPage, EVENT_PRESS);
  SimbleeForMobile.drawText(297, 390, "v", WHITE, 12);
  nextPage = SimbleeForMobile.drawButton(280, 380, 40, "", WHITE, 2);
  SimbleeForMobile.setEvents(nextPage, EVENT_PRESS);

  //BLUR SCREEN
  insertScreen1 = SimbleeForMobile.drawRect(0, 0, 320, 570, rgba(155, 155, 155, 50));
  SimbleeForMobile.setVisible(insertScreen1, false);

  //POP BOX
  insertScreen2 = SimbleeForMobile.drawRect(20, 200, 280, 150, GE_RED);
  SimbleeForMobile.setVisible(insertScreen2, false);

  //TEXT
  insertScreen3 = SimbleeForMobile.drawText(60, 220, "Please insert wine into rack", WHITE, 18);
  SimbleeForMobile.setVisible(insertScreen3, false);
  insertScreen4 = SimbleeForMobile.drawText(200, 260, 1, WHITE, 22);
  SimbleeForMobile.setVisible(insertScreen4, false);
  insertScreen5 = SimbleeForMobile.drawText(110, 240, 1, WHITE, 22);
  SimbleeForMobile.setVisible(insertScreen5, false);
  insertScreen6 = SimbleeForMobile.drawText(60, 240, "Wine", WHITE, 22);
  SimbleeForMobile.setVisible(insertScreen6, false);
  insertScreen7 = SimbleeForMobile.drawText(60, 260, "Placed in rack #", WHITE, 22);
  SimbleeForMobile.setVisible(insertScreen7, false);

  addPopUpDetectedText = SimbleeForMobile.drawText(60, 220, "Bottle Detected", WHITE, 20);
  SimbleeForMobile.setVisible(addPopUpDetectedText, false);
  addPopUpRackText = SimbleeForMobile.drawText(60, 240, "In Rack#:", WHITE, 20);
  SimbleeForMobile.setVisible(addPopUpRackText, false);
  addPopUpRackNumber = SimbleeForMobile.drawText(140, 240, "A1", WHITE, 20);
  SimbleeForMobile.setVisible(addPopUpRackNumber, false);
  addPopUpSelectText = SimbleeForMobile.drawText(60, 260, "Please Select Wine", WHITE, 20);
  SimbleeForMobile.setVisible(addPopUpSelectText, false);

  removePopUpDetectedText = SimbleeForMobile.drawText(60, 220, "Bottle Removed", WHITE, 20);
  SimbleeForMobile.setVisible(removePopUpDetectedText, false);
  removePopUpRackText = SimbleeForMobile.drawText(60, 240, "In Rack#:", WHITE, 20);
  SimbleeForMobile.setVisible(removePopUpRackText, false);
  removePopUpRackNumber = SimbleeForMobile.drawText(140, 240, "A1", WHITE, 20);
  SimbleeForMobile.setVisible(removePopUpRackNumber, false);

  SimbleeForMobile.endScreen();
}

//************************************************************************************************

//SCREEN 5 BASE*********************************************************************************
void addScreen() {
  SimbleeForMobile.beginScreen();

  //BACKGROUND
  SimbleeForMobile.drawRect(0, 0, 320, 480, GE_RED);
  //SimbleeForMobile.imageSource(ColumbiaCrest2,JPG,ColumbiaCrest2_jpg,ColumbiaCrest2_jpg_len);
  //SimbleeForMobile.drawImage(ColumbiaCrest2,0,60);
  SimbleeForMobile.drawRect(0, 60, 320,  180, GE_LIGHTGRAY);
  SimbleeForMobile.drawRect(0, 240, 320, 180, WHITE);

  VineyardUISingle = SimbleeForMobile.drawText(30, 245, "Cherry Blossom", GE_LIGHTGRAY, 16);
  WineName1UISingle = SimbleeForMobile.drawText(30, 262, "Central Otago", BLACK, 18);
  WineName2UISingle = SimbleeForMobile.drawText(30, 280, "2014", BLACK, 18);
  CountryUISingle = SimbleeForMobile.drawText(30, 300, "Pinot Noir", GE_LIGHTGRAY, 16);
  PriceUISingle = SimbleeForMobile.drawText(200, 300, "$20.00", GE_LIGHTGRAY, 16);
  //LOCATION TITLE
  SimbleeForMobile.drawText(250, 245, "LOCATION", GE_LIGHTGRAY, 12);
  //LOCATION
  LocationUISingle = SimbleeForMobile.drawText(262, 260, "", BLACK, 30);

  //INFO
  addButton = SimbleeForMobile.drawButton(40, 375, 240, "ADD TO CHILLER", BLACK, 2);
  SimbleeForMobile.setEvents(addButton, EVENT_PRESS);
  SimbleeForMobile.setVisible(addButton, false);
  removeButton = SimbleeForMobile.drawButton(40, 375, 240, "REMOVE FROM CHILLER", RED, 2);
  SimbleeForMobile.setEvents(removeButton, EVENT_PRESS);
  SimbleeForMobile.setVisible(removeButton, false);
  returnButton = SimbleeForMobile.drawButton(40, 435, 240, "RETURN TO INVENTORY", BLACK, 1);
  SimbleeForMobile.setEvents(returnButton, EVENT_PRESS);

  //SHOWWINESCREEN
  insertScreen1 = SimbleeForMobile.drawRect(0, 0, 320, 570, rgba(155, 155, 155, 50));
  SimbleeForMobile.setVisible(insertScreen1, false);
  insertScreen2 = SimbleeForMobile.drawRect(20, 200, 280, 150, GE_RED);
  SimbleeForMobile.setVisible(insertScreen2, false);
  insertScreen3 = SimbleeForMobile.drawText(60, 220, "Please insert wine into rack", WHITE, 18);
  SimbleeForMobile.setVisible(insertScreen3, false);
  insertScreen4 = SimbleeForMobile.drawText(200, 260, 1, WHITE, 22);
  SimbleeForMobile.setVisible(insertScreen4, false);
  insertScreen5 = SimbleeForMobile.drawText(60, 240, 1, WHITE, 22);
  SimbleeForMobile.setVisible(insertScreen5, false);
  insertScreen6 = SimbleeForMobile.drawText(60, 240, "", WHITE, 22);
  SimbleeForMobile.setVisible(insertScreen6, false);
  insertScreen7 = SimbleeForMobile.drawText(60, 260, "Placed in rack #", WHITE, 22);
  SimbleeForMobile.setVisible(insertScreen7, false);

  removeScreen1 = SimbleeForMobile.drawRect(0, 0, 320, 570, rgba(155, 155, 155, 50));
  SimbleeForMobile.setVisible(removeScreen1, false);
  removeScreen2 = SimbleeForMobile.drawRect(20, 200, 280, 150, GE_RED);
  SimbleeForMobile.setVisible(removeScreen2, false);
  removeScreen3 = SimbleeForMobile.drawText(60, 220, "Please remove wine from rack:", WHITE, 18);
  SimbleeForMobile.setVisible(removeScreen3, false);
  removeScreen4 = SimbleeForMobile.drawText(150, 260, 1, WHITE, 40);
  SimbleeForMobile.setVisible(removeScreen4, false);

  SimbleeForMobile.endScreen();
}
//************************************************************************************************

void checkButtonState(Slot toButton, int cellID) {
  if (toButton.isPressed()) {
    if (initialized == false) {
      currentButtonState[cellID] = true;
    }
    else {
      nextButtonState[cellID] = true;
    }
  }
  else if (toButton.isReleased()) {
    if (initialized == false) {
      currentButtonState[cellID] = false;
    }
    else {
      nextButtonState[cellID] = false;
    }
  }
}

void checkAllButtons() {
  for (int i = 0; i < 25; i++) {
    checkButtonState(Slots[i], i);
  }
  initialized = true;
}

void checkAdd() {
  for (int i = 0; i < 25; i++) {
    if (currentButtonState[i] < nextButtonState[i]) {
      addDetected = i;
      currentButtonState[i] = nextButtonState[i];
      updatePage = true;
      addMode = 1;
  #ifdef DEMO 
      leds[recalc(addDetected)] = color;
  #else
      leds[addDetected] = color;
  #endif
    }
  }
}

void checkRemove() {
  for (int i = 0; i < 25; i++) {
    if (currentButtonState[i] > nextButtonState[i]) {
      removeDetected = i;
      currentButtonState[i] = nextButtonState[i];
      updatePage = true;
    }
  }
}

void compareStates() {
  count = millis();
  while (switchDetected == 0 && millis() - count < 5000) {
    checkAllButtons();
    for (int i = 0; i < 25; i++) {
      if (currentButtonState[i] < nextButtonState[i]) {
        switchDetected = i;
        wineStored[switchDetected] = addedWine;
      }
    }
  }
  if (switchDetected == 0) {
    SimbleeForMobile.setVisible(insertScreen1, false);
    SimbleeForMobile.setVisible(insertScreen2, false);
    SimbleeForMobile.setVisible(insertScreen3, false);
    FastLED.clear();
    FastLED.show();
  }
}

void showWineInsertScreen() {
  SimbleeForMobile.setVisible(insertScreen1, true);
  SimbleeForMobile.setVisible(insertScreen2, true);
  SimbleeForMobile.setVisible(insertScreen3, true);
  checkAllButtons();
  FastLED.show();
  delay(100);
  initialized = true;
  compareStates();
}

void removeBottle() {
  SimbleeForMobile.setVisible(removeScreen1, true);
  SimbleeForMobile.setVisible(removeScreen2, true);
  SimbleeForMobile.setVisible(removeScreen3, true);
  if(winePage == 1)
  {
    if(wineChosen == 1)
    {
      SimbleeForMobile.updateValue(removeScreen4, 1);
    }
    else if(wineChosen == 2)
    {
      SimbleeForMobile.updateValue(removeScreen4, 2);
    }
    else if(wineChosen == 3)
    {
      SimbleeForMobile.updateValue(removeScreen4, 3);
    }
    else if(wineChosen == 4)
    {
      SimbleeForMobile.updateValue(removeScreen4, 4);
    }
  }
  else if(winePage == 2)
  {
    if(wineChosen == 1)
    {
      SimbleeForMobile.updateValue(removeScreen4, 5);
    }
    else if(wineChosen == 2)
    {
      SimbleeForMobile.updateValue(removeScreen4, 6);
    }
    else if(wineChosen == 3)
    {
      SimbleeForMobile.updateValue(removeScreen4, 7);
    }
    else if(wineChosen == 4)
    {
      SimbleeForMobile.updateValue(removeScreen4, 8);
    }
  }
  else if(winePage == 3)
  {
    if(wineChosen == 1)
    {
      SimbleeForMobile.updateValue(removeScreen4, 9);
    }
    else if(wineChosen == 2)
    {
      SimbleeForMobile.updateValue(removeScreen4, 10);
    }
    else if(wineChosen == 3)
    {
      SimbleeForMobile.updateValue(removeScreen4, 11);
    }
    else if(wineChosen == 4)
    {
      SimbleeForMobile.updateValue(removeScreen4, 12);
    }
  }
  else if(winePage == 4)
  {
    if(wineChosen == 1)
    {
      SimbleeForMobile.updateValue(removeScreen4, 13);
    }
    else if(wineChosen == 2)
    {
      SimbleeForMobile.updateValue(removeScreen4, 14);
    }
    else if(wineChosen == 3)
    {
      SimbleeForMobile.updateValue(removeScreen4, 15);
    }
    else if(wineChosen == 4)
    {
      SimbleeForMobile.updateValue(removeScreen4, 16);
    }
  }
  else if(winePage == 5)
  {
    if(wineChosen == 1)
    {
      SimbleeForMobile.updateValue(removeScreen4, 17);
    }
    else if(wineChosen == 2)
    {
      SimbleeForMobile.updateValue(removeScreen4, 18);
    }
    else if(wineChosen == 3)
    {
      SimbleeForMobile.updateValue(removeScreen4, 19);
    }
    else if(wineChosen == 4)
    {
      SimbleeForMobile.updateValue(removeScreen4, 20);
    }
  }
  else if(winePage == 6)
  {
    if(wineChosen == 1)
    {
      SimbleeForMobile.updateValue(removeScreen4, 21);
    }
    else if(wineChosen == 2)
    {
      SimbleeForMobile.updateValue(removeScreen4, 22);
    }
    else if(wineChosen == 3)
    {
      SimbleeForMobile.updateValue(removeScreen4, 23);
    }
    else if(wineChosen == 4)
    {
      SimbleeForMobile.updateValue(removeScreen4, 24);
    }
  }
  else if(winePage == 7)
  {
    if(wineChosen == 1)
    {
      SimbleeForMobile.updateValue(removeScreen4, 25);
    }
  }
  SimbleeForMobile.setVisible(removeScreen4, true);

  int ledPosition = ((winePage-1)*4)+wineChosen-1;

  #ifdef DEMOUNIT

  //  Positions on demo unit are reversed. Therefore LED index for corresponding
  //    switch is the ceiling of SWH_POS in multiple of 4 - remainder of SWH_POS / 4
  int remainder = ledPosition % 5;

  if (remainder == 0) {
      ledPosition += 4;
  } else if (remainder == 1) {
      ledPosition += 2;
  } else if (remainder == 3) {
      ledPosition -= 2;
  } else if (remainder == 4) {
      ledPosition -= 4;
  }

  #endif
  
  FastLED.clear();
  leds[ledPosition] = color2;
  FastLED.show();
  delay(100);
  initialized = true;
  while(nextButtonState[ledPosition] == true)
  {
    checkAllButtons();
  }
  initialized = false;
  SimbleeForMobile.setVisible(removeScreen4, false);
  SimbleeForMobile.setVisible(removeScreen1,false);
  SimbleeForMobile.setVisible(removeScreen2,false);
  SimbleeForMobile.setVisible(removeScreen3,false);
  FastLED.clear();
  FastLED.show();
  delay(100);
}

void SimbleeForMobile_onDisconnect() {
  disconnected = true;
}

void setup() {
  Serial.begin(9600);
  initializePins();
  //initializePinsTestBox();

  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
  //checkAllButtons();
  SimbleeForMobile.deviceName = "Wine";
  SimbleeForMobile.advertisementData = "Chiller";
  SimbleeForMobile.domain = "FirstBuild4.simblee.com";
  SimbleeForMobile.begin();
}

void loop() {
  checkAllButtons();
  checkAdd();
  checkRemove();
  if (disconnected == true)
  {
    SimbleeForMobile.showScreen(2);
    disconnected = false;
  }
  if (SimbleeForMobile.updatable)
  {
    if (SimbleeForMobile.screen == 2) {
      if (updatePage == true)
      {
        SimbleeForMobile.updateValue(pageValue, winePage);
        if (addDetected >= 0) {
          FastLED.show();
          SimbleeForMobile.updateText(addPopUpRackNumber, ChillerSlot[addDetected]);
          SimbleeForMobile.setVisible(insertScreen1, true);
          SimbleeForMobile.setVisible(insertScreen2, true);
          SimbleeForMobile.setVisible(addPopUpDetectedText, true);
          SimbleeForMobile.setVisible(addPopUpRackText, true);
          SimbleeForMobile.setVisible(addPopUpRackNumber, true);
          SimbleeForMobile.setVisible(addPopUpSelectText, true);
          delay(3000);
          SimbleeForMobile.setVisible(insertScreen1, false);
          SimbleeForMobile.setVisible(insertScreen2, false);
          SimbleeForMobile.setVisible(addPopUpDetectedText, false);
          SimbleeForMobile.setVisible(addPopUpRackText, false);
          SimbleeForMobile.setVisible(addPopUpRackNumber, false);
          SimbleeForMobile.setVisible(addPopUpSelectText, false);
          SimbleeForMobile.updateColor(screen2Background, BLUE);
          SimbleeForMobile.updateText(screen2Title, "Select Wine");
          FastLED.clear();
          FastLED.show();
        }
        if (removeDetected >= 0) {
          int bottle;
          for (int i = 0; i < 25; i++) {
            if (wine[i].getIndex() == removeDetected) {
              bottle = i;
            }
          }
          Serial.println(bottle);
          leds[removeDetected] = color2;
          FastLED.show();
          SimbleeForMobile.updateText(removePopUpRackNumber, ChillerSlot[removeDetected]);
          SimbleeForMobile.setVisible(insertScreen1, true);
          SimbleeForMobile.setVisible(insertScreen2, true);
          SimbleeForMobile.setVisible(removePopUpDetectedText, true);
          SimbleeForMobile.setVisible(removePopUpRackText, true);
          SimbleeForMobile.setVisible(removePopUpRackNumber, true);
          wine[bottle].setIndex(-1);
          delay(3000);
          SimbleeForMobile.setVisible(insertScreen1, false);
          SimbleeForMobile.setVisible(insertScreen2, false);
          SimbleeForMobile.setVisible(removePopUpDetectedText, false);
          SimbleeForMobile.setVisible(removePopUpRackText, false);
          SimbleeForMobile.setVisible(removePopUpRackNumber, false);
          FastLED.clear();
          FastLED.show();
          removeDetected = -1;
        }

        for (int i = 0; i < 5; i++) {
          SimbleeForMobile.updateText(VineyardUI[i], wine[i + ((winePage - 1) * 5)].getVineyard());
          SimbleeForMobile.updateText(WineName1UI[i], wine[i + ((winePage - 1) * 5)].getName());
          SimbleeForMobile.updateText(WineName2UI[i], wine[i + ((winePage - 1) * 5)].getYear());
          SimbleeForMobile.updateText(CountryUI[i], wine[i + ((winePage - 1) * 5)].getwineType());
          SimbleeForMobile.updateText(PriceUI[i], wine[i + ((winePage - 1) * 5)].getPrice());
          if (wine[i + ((winePage - 1) * 5)].getIndex() == -1) {
            SimbleeForMobile.updateColor(LocationUI[i], BLACK);
            SimbleeForMobile.updateText(LocationUI[i], "");
          }
          else if (wine[i + ((winePage - 1) * 5)].getIndex() >= 0) {
            SimbleeForMobile.updateColor(LocationUI[i], BLUE);
            SimbleeForMobile.updateText(LocationUI[i], ChillerSlot[wine[i + ((winePage - 1) * 5)].getIndex()]);
          }
        }
        updatePage = false;
      }
    }
    //*******************************************************************************************************

    //UPDATES FOR SCREEN 5 (INFO SCREEN)***********************************************
    else if (SimbleeForMobile.screen == 5)
    {
      SimbleeForMobile.updateText(VineyardUISingle, wine[clickedOverlay + ((winePage - 1) * 5)].getVineyard());
      SimbleeForMobile.updateText(WineName1UISingle, wine[clickedOverlay + ((winePage - 1) * 5)].getName());
      SimbleeForMobile.updateText(WineName2UISingle, wine[clickedOverlay + ((winePage - 1) * 5)].getYear());
      SimbleeForMobile.updateText(CountryUISingle, wine[clickedOverlay + ((winePage - 1) * 5)].getwineType());
      SimbleeForMobile.updateText(PriceUISingle, wine[clickedOverlay + ((winePage - 1) * 5)].getPrice());
      if (wine[clickedOverlay + ((winePage - 1) * 5)].getIndex() == -1) {
        SimbleeForMobile.updateColor(LocationUISingle, BLACK);
        SimbleeForMobile.updateText(LocationUISingle, "");
        SimbleeForMobile.setVisible(removeButton, false);
        SimbleeForMobile.setVisible(addButton, true);
      }
      else if (wine[clickedOverlay + ((winePage - 1) * 5)].getIndex() >= 0) {
        SimbleeForMobile.updateColor(LocationUISingle, BLUE);
        SimbleeForMobile.updateText(LocationUISingle, ChillerSlot[wine[clickedOverlay + ((winePage - 1) * 5)].getIndex()]);
        SimbleeForMobile.setVisible(addButton, false);
        SimbleeForMobile.setVisible(removeButton, true);
      }
      if (switchDetected >= 0)
      {
        SimbleeForMobile.updateText(insertScreen5, wine[clickedOverlay + ((winePage - 1) * 5)].getName());
        SimbleeForMobile.updateText(insertScreen4, ChillerSlot[switchDetected]);
        wine[clickedOverlay + ((winePage - 1) * 5)].setIndex(switchDetected);
        SimbleeForMobile.setVisible(insertScreen4, true);
        SimbleeForMobile.setVisible(insertScreen5, true);
        SimbleeForMobile.setVisible(insertScreen6, true);
        SimbleeForMobile.setVisible(insertScreen7, true);
        delay(3000);
        SimbleeForMobile.setVisible(insertScreen1, false);
        SimbleeForMobile.setVisible(insertScreen2, false);
        SimbleeForMobile.setVisible(insertScreen3, false);
        SimbleeForMobile.setVisible(insertScreen4, false);
        SimbleeForMobile.setVisible(insertScreen5, false);
        SimbleeForMobile.setVisible(insertScreen6, false);
        SimbleeForMobile.setVisible(insertScreen7, false);
        initialized = false;
        FastLED.clear();
        FastLED.show();
        switchDetected = -1;
        SimbleeForMobile.showScreen(2);
      }
    }
  }
  SimbleeForMobile.process();
}

void ui_event(event_t &event) {
  //SCREEN 2 EVENTS
  if (SimbleeForMobile.screen == 2)
  {
    if (event.id == nextPage)
    {
      updatePage = true;
      winePage++;
      if (winePage > 5)
      {
        winePage = 1;
      }
    }
    else if (event.id == previousPage)
    {
      updatePage = true;
      winePage--;
      if (winePage == 0) {
        winePage = 5;
      }
    }
    else if (event.id == OverlayUI[0]) {
      clickedOverlay = 0;
      updatePage = true;
      if (!addMode)
        SimbleeForMobile.showScreen(5);
      else {
        SimbleeForMobile.updateColor(screen2Background, GE_RED);
        SimbleeForMobile.updateText(screen2Title, "Wine Inventory");
        wine[clickedOverlay + ((winePage - 1) * 5)].setIndex(addDetected);
        addMode = false;
        addDetected = -1;
        updatePage = true;
      }
    }
    else if (event.id == OverlayUI[1]) {
      clickedOverlay = 1;
      updatePage = true;
      if (!addMode)
        SimbleeForMobile.showScreen(5);
      else {
        SimbleeForMobile.updateColor(screen2Background, GE_RED);
        SimbleeForMobile.updateText(screen2Title, "Wine Inventory");
        wine[clickedOverlay + ((winePage - 1) * 5)].setIndex(addDetected);
        addMode = false;
        addDetected = -1;
        updatePage = true;
      }

    }
    else if (event.id == OverlayUI[2]) {
      clickedOverlay = 2;
      updatePage = true;
      if (!addMode)
        SimbleeForMobile.showScreen(5);
      else {
        SimbleeForMobile.updateColor(screen2Background, GE_RED);
        SimbleeForMobile.updateText(screen2Title, "Wine Inventory");
        wine[clickedOverlay + ((winePage - 1) * 5)].setIndex(addDetected);
        addMode = false;
        addDetected = -1;
        updatePage = true;
      }
    }
    else if (event.id == OverlayUI[3]) {
      clickedOverlay = 3;
      updatePage = true;
      if (!addMode)
        SimbleeForMobile.showScreen(5);
      else {
        SimbleeForMobile.updateColor(screen2Background, GE_RED);
        SimbleeForMobile.updateText(screen2Title, "Wine Inventory");
        wine[clickedOverlay + ((winePage - 1) * 5)].setIndex(addDetected);
        addMode = false;
        addDetected = -1;
        updatePage = true;
      }
    }
    else if (event.id == OverlayUI[4]) {
      clickedOverlay = 4;
      updatePage = true;
      if (!addMode)
        SimbleeForMobile.showScreen(5);
      else {
        SimbleeForMobile.updateColor(screen2Background, GE_RED);
        SimbleeForMobile.updateText(screen2Title, "Wine Inventory");
        wine[clickedOverlay + ((winePage - 1) * 5)].setIndex(addDetected);
        addMode = false;
        addDetected = -1;
        updatePage = true;
      }
    }
  }
  //SCREEN 5 EVENTS
  else if (SimbleeForMobile.screen == 5)
  {
    if (event.id == addButton) {
      showWineInsertScreen();
      updatePage = true;
    }
    else if (event.id == removeButton) {
      removeBottle();
      updatePage = true;
    }
    else if (event.id == returnButton) {
      SimbleeForMobile.showScreen(2);
      updatePage = true;
    }
  }
}

void ui() {
  switch (SimbleeForMobile.screen)
  {
    case 1:
      splashScreen();
      delay(3000);
      SimbleeForMobile.showScreen(2);
      break;

    case 2:
      inventoryScreen();
      break;

    case 3:
      break;

    case 4:
      break;

    case 5:
      addScreen();
      break;

    default:
      splashScreen();
      break;
  }
}

