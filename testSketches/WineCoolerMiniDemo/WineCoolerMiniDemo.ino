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
#include "camera_png.h"
#include "wineLabel_jpg.h"
#include "camera_jpg.h"

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

#define WineSpashScreen 1
#define WineBottleImage 2
//#define ColumbiaCrest1 2
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
#define cameraImage 20
#define wineLabelImage 21



CRGB leds[NUM_LEDS];

std::vector<Slot> Slots;

bool disconnected = false;
bool initialized = false;
bool updatePage = true;
bool addMode = false;
bool cameraMode = false;

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
        screen2Title, screen2Background,
        CarmenImage, CoppolaShirazImage, LaTerceraImage, cameraButton,
        text1, text2, text3, wineLabel, backButton, acceptButton,
        scanScreen1, scanScreen2, scanScreen3, takePhotoButton;

int8_t winePage = 1,
       addDetected = -1,
       removeDetected = -1,
       switchDetected = -1,
       clickedOverlay = 0,
       wineChosen = 0;

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
void splashScreen()
{
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
void inventoryScreen()
{
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
  SimbleeForMobile.imageSource(WineBottleImage, PNG, WineBottleImage_png, WineBottleImage_png_len);
  SimbleeForMobile.drawImage(WineBottleImage, 25, 66);
  SimbleeForMobile.drawImage(WineBottleImage, 25, 138);
  SimbleeForMobile.drawImage(WineBottleImage, 25, 210);
  SimbleeForMobile.drawImage(WineBottleImage, 25, 282);
  SimbleeForMobile.drawImage(WineBottleImage, 25, 354);

   //PAGE SELECTION
  //SimbleeForMobile.drawText(134 , 437, "Page", WHITE, 16);
  pageValue = SimbleeForMobile.drawText(295, 230, 1, WHITE, 20);
  previousPage = SimbleeForMobile.drawButton(280, 70, 40, "^", WHITE, 1);
  SimbleeForMobile.setEvents(previousPage, EVENT_PRESS);
  nextPage = SimbleeForMobile.drawButton(280, 380, 40, "v", WHITE, 1);
  SimbleeForMobile.setEvents(nextPage, EVENT_PRESS);

  //TITLE
  screen2Title = SimbleeForMobile.drawText(90, 25, "Wine Inventory", WHITE, 25);

  int boxY[5] = {64, 136, 208, 280, 352};

  int overlayY[5] = {61, 133, 205, 277, 349};

  for (int i = 0; i < 5; i++) {
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

  cameraButton = SimbleeForMobile.drawButton(80,440,150,"Scan Wine",WHITE, 1);
  //cameraButton = SimbleeForMobile.drawImage(cameraImage,125,420);
  //SimbleeForMobile.imageSource(cameraImage, PNG, camera_jpg, camera_jpg_len);
  SimbleeForMobile.setEvents(cameraButton, EVENT_PRESS);

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

  scanScreen1 = SimbleeForMobile.drawRect(0, 0, 320, 480, GE_RED);
  SimbleeForMobile.setVisible(scanScreen1, false);
  scanScreen2 = SimbleeForMobile.drawRect(0, 60, 320,360, GE_LIGHTGRAY);
  SimbleeForMobile.setVisible(scanScreen2, false);
  scanScreen3 = SimbleeForMobile.drawRect(30,90, 260, 300,GE_DARKGRAY);
  SimbleeForMobile.setVisible(scanScreen3, false);
  
  //WINE
  //SimbleeForMobile.imageSource(wineLabelImage, PNG, wineLabel_jpg, wineLabel_jpg_len);
  //wineLabel = SimbleeForMobile.drawImage(wineLabelImage, 30, 90);
  //SimbleeForMobile.setVisible(wineLabel, false);

  //Label
  text1 = SimbleeForMobile.drawText(118, 190, "POSITION THE", WHITE, 14);
  SimbleeForMobile.setVisible(text1, false);
  text2 = SimbleeForMobile.drawText(90, 200, "WINE LABEL", WHITE, 28);
  SimbleeForMobile.setVisible(text2, false);
  text3 = SimbleeForMobile.drawText(100, 230, "WITHIN THIS FRAME", WHITE, 14);
  SimbleeForMobile.setVisible(text3, false);


  //BOTTOM MENU
  backButton = SimbleeForMobile.drawButton(40, 390, 40, "BACK", WHITE, 1);
  SimbleeForMobile.setEvents(backButton, EVENT_PRESS);
  SimbleeForMobile.setVisible(backButton, false);
  //SimbleeForMobile.imageSource(cameraImage, PNG, camera_png, camera_png_len);
  //cameraButton = SimbleeForMobile.drawImage(cameraImage, 125, 420);
  //SimbleeForMobile.setEvents(cameraButton, EVENT_PRESS);
  acceptButton = SimbleeForMobile.drawButton(220, 390, 60, "ACCEPT", WHITE, 1);
  SimbleeForMobile.setEvents(acceptButton, EVENT_PRESS);
  SimbleeForMobile.setVisible(acceptButton, false);

  takePhotoButton = SimbleeForMobile.drawButton(100,440,120,"TAKE PHOTO", WHITE,1);
  SimbleeForMobile.setEvents(takePhotoButton, EVENT_PRESS);
  SimbleeForMobile.setVisible(takePhotoButton, false);
 
  Serial.println("before end screen");
  SimbleeForMobile.endScreen();
  
  Serial.println("after end Screen");
}


//************************************************************************************************

//SCREEN 5 BASE*********************************************************************************
void addScreen()
{
   
  SimbleeForMobile.beginScreen();
  
  //BACKGROUND
  SimbleeForMobile.drawRect(0, 0, 320, 480, GE_RED);


  Serial.println(wineChosen);
  //REDUCE AND ADD TO WINE STRUCTURE LATER....
  
    SimbleeForMobile.drawImage(ColumbiaCrest2, 0, 60);
    SimbleeForMobile.drawImage(Carmen1, 0, 60);
    SimbleeForMobile.drawImage(CoppolaShiraz1, 0, 60);
    SimbleeForMobile.drawImage(LaTercera1, 0, 60);
    SimbleeForMobile.drawImage(Martini2, 0, 60);
    SimbleeForMobile.drawImage(CoppolaCab2, 0, 60);
    SimbleeForMobile.drawImage(Prieure1, 0, 60);
    SimbleeForMobile.drawImage(PerezCruz2, 0, 60);
    SimbleeForMobile.drawImage(Proximo2, 0, 60);

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

   if (wineChosen == 0) {
    SimbleeForMobile.imageSource(ColumbiaCrest2, JPG, ColumbiaCrest2_jpg, ColumbiaCrest2_jpg_len);

  }
  else if (wineChosen == 1) {
    SimbleeForMobile.imageSource(Carmen1, JPG, Carmen1_jpg, Carmen1_jpg_len);
    
  }
  else if (wineChosen == 2) {
    SimbleeForMobile.imageSource(CoppolaShiraz1, JPG, CoppolaShiraz1_jpg, CoppolaShiraz1_jpg_len);
    
  }
  else if (wineChosen == 3) {
    SimbleeForMobile.imageSource(LaTercera1, JPG, LaTercera1_jpg, LaTercera1_jpg_len);
    
  }
  else if (wineChosen == 4) {
    SimbleeForMobile.imageSource(Martini2, JPG, Martini2_jpg, Martini2_jpg_len);
    
  }
  else if (wineChosen == 5) {
    SimbleeForMobile.imageSource(CoppolaCab2, JPG, CoppolaCab2_jpg, CoppolaCab2_jpg_len);
    
  }
  else if (wineChosen == 6) {
    SimbleeForMobile.imageSource(Prieure2, JPG, Prieure2_jpg, Prieure2_jpg_len);
    
  }
  else if (wineChosen == 7) {
    SimbleeForMobile.imageSource(PerezCruz2, JPG, PerezCruz2_jpg, PerezCruz2_jpg_len);
    
  }
  else if (wineChosen == 8) {
    SimbleeForMobile.imageSource(Proximo2, JPG, Proximo2_jpg, Proximo2_jpg_len);
    
  }
  else
    SimbleeForMobile.drawRect(0, 60, 320,  180, GE_LIGHTGRAY);
}
//************************************************************************************************


void cameraScreen(){
  SimbleeForMobile.beginScreen();
  SimbleeForMobile.drawRect(0, 0, 320, 480, GE_RED);
  SimbleeForMobile.drawRect(0, 60, 320,360, GE_LIGHTGRAY);
  SimbleeForMobile.drawRect(30,90, 260, 300,GE_DARKGRAY);

  //WINE
  SimbleeForMobile.imageSource(wineLabelImage, PNG, wineLabel_jpg, wineLabel_jpg_len);
  wineLabel = SimbleeForMobile.drawImage(wineLabelImage, 30, 90);
 //SimbleeForMobile.setVisible(wineLabel, false);

  //Label
  text1 = SimbleeForMobile.drawText(118, 190, "POSITION THE", WHITE, 14);
  text2 = SimbleeForMobile.drawText(90, 200, "WINE LABEL", WHITE, 28);
  text3 = SimbleeForMobile.drawText(100, 230, "WITHIN THIS FRAME", WHITE, 14);


  //BOTTOM MENU
  backButton = SimbleeForMobile.drawButton(40, 390, 40, "BACK", WHITE, 1);
  SimbleeForMobile.setEvents(backButton, EVENT_PRESS);
  //SimbleeForMobile.imageSource(cameraImage, PNG, camera_png, camera_png_len);
  //cameraButton = SimbleeForMobile.drawImage(cameraImage, 125, 420);
  //SimbleeForMobile.setEvents(cameraButton, EVENT_PRESS);
  acceptButton = SimbleeForMobile.drawButton(220, 390, 60, "ACCEPT", WHITE, 1);
  SimbleeForMobile.setEvents(acceptButton, EVENT_PRESS);
  //SimbleeForMobile.setVisible(acceptButton, false);

  SimbleeForMobile.endScreen();
}

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

//  I've just used this function as an example to demonstrate the classes I've implemented.
//    Remember: onPressed() returns true for a pressed button.
//      Progressive presses will return false.
void checkAdd() {
  for (int i = 0; i < 25; i++) {
    if (currentButtonState[i] < nextButtonState[i]) {
      addDetected = i;
      currentButtonState[i] = nextButtonState[i];
      updatePage = true;
      addMode = 1;
      leds[recalc(addDetected)] = color;
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
  while (switchDetected == -1 && millis() - count < 5000) {
    checkAllButtons();
    for (int i = 0; i < 25; i++) {
      if (currentButtonState[i] < nextButtonState[i]) {
        switchDetected = i;
        currentButtonState[i] = nextButtonState[i];
      }
    }
  }
  leds[recalc(switchDetected)] = color;
  FastLED.show();
  if (switchDetected == -1) {
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
  delay(100);
  compareStates();
}

void removeBottle()
{
  SimbleeForMobile.setVisible(removeScreen1, true);
  SimbleeForMobile.setVisible(removeScreen2, true);
  SimbleeForMobile.setVisible(removeScreen3, true);
  SimbleeForMobile.updateText(removeScreen4, ChillerSlot[wine[clickedOverlay + ((winePage - 1) * 5)].getIndex()]);
  SimbleeForMobile.setVisible(removeScreen4, true);
  for (int i = 0; i < 25; i++) {
    if (ChillerSlot[i] == ChillerSlot[wine[clickedOverlay + ((winePage - 1) * 5)].getIndex()]) {
      leds[recalc(i)] = color2;
      break;
    }
  }
  FastLED.show();
  while (nextButtonState[wine[clickedOverlay + ((winePage - 1) * 5)].getIndex()] > 0)
  {
    checkAllButtons();
  }
  wine[clickedOverlay + ((winePage - 1) * 5)].setIndex(-1);
  SimbleeForMobile.setVisible(removeScreen4, false);
  SimbleeForMobile.setVisible(removeScreen1, false);
  SimbleeForMobile.setVisible(removeScreen2, false);
  SimbleeForMobile.setVisible(removeScreen3, false);
  FastLED.clear();
  FastLED.show();
  delay(100);
  SimbleeForMobile.showScreen(2);
}


// Inializing vector of Slots for TESTBOX units.
//   Should not have problems with corresponding
//   switch and led positions.
void setup()
{
  //Serial.begin(9600);

  Slot temp1 = Slot(2, leds[0]);
  Slot temp2 = Slot(3, leds[1]);
  Slot temp3 = Slot(4, leds[2]);
  Slot temp4 = Slot(6, leds[3]);
  Slot temp5 = Slot(7, leds[4]);
  Slot temp6 = Slot(8, leds[5]);
  Slot temp7 = Slot(9, leds[6]);
  Slot temp8 = Slot(10, leds[7]);
  Slot temp9 = Slot(11, leds[8]);
  Slot temp10 = Slot(12, leds[9]);
  Slot temp11 = Slot(13, leds[10]);
  Slot temp12 = Slot(14, leds[11]);
  Slot temp13 = Slot(15, leds[12]);
  Slot temp14 = Slot(16, leds[13]);
  Slot temp15 = Slot(17, leds[14]);

  Slot temp16 = Slot(18, leds[15]);
  Slot temp17 = Slot(19, leds[16]);
  Slot temp18 = Slot(20, leds[17]);
  Slot temp19 = Slot(21, leds[18]);
  Slot temp20 = Slot(22, leds[19]);

  Slot temp21 = Slot(23, leds[20]);
  Slot temp22 = Slot(24, leds[21]);
  Slot temp23 = Slot(25, leds[22]);
  Slot temp24 = Slot(28, leds[23]);
  Slot temp25 = Slot(29, leds[24]);


  Slots.push_back(temp1);
  Slots.push_back(temp2);
  Slots.push_back(temp3);
  Slots.push_back(temp4);
  Slots.push_back(temp5);
  Slots.push_back(temp6);
  Slots.push_back(temp7);
  Slots.push_back(temp8);
  Slots.push_back(temp9);
  Slots.push_back(temp10);
  Slots.push_back(temp11);
  Slots.push_back(temp12);
  Slots.push_back(temp13);
  Slots.push_back(temp14);
  Slots.push_back(temp15);
  Slots.push_back(temp16);
  Slots.push_back(temp17);
  Slots.push_back(temp18);
  Slots.push_back(temp19);
  Slots.push_back(temp20);
  Slots.push_back(temp21);
  Slots.push_back(temp22);
  Slots.push_back(temp23);
  Slots.push_back(temp24);
  Slots.push_back(temp25);

  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
  checkAllButtons();
  SimbleeForMobile.deviceName = "Wine";
  SimbleeForMobile.advertisementData = "Chiller";
  SimbleeForMobile.domain = "FirstBuild4.simblee.com";
  SimbleeForMobile.begin("");
}

void loop()
{
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
      if(cameraMode == true){
        SimbleeForMobile.setVisible(scanScreen1, true);
        SimbleeForMobile.setVisible(scanScreen2, true);
        SimbleeForMobile.setVisible(scanScreen3, true);
        SimbleeForMobile.setVisible(text1, true);
        SimbleeForMobile.setVisible(text2, true);
        SimbleeForMobile.setVisible(text3, true);
        SimbleeForMobile.setVisible(backButton, true);
        SimbleeForMobile.setVisible(takePhotoButton, true);
        
      }
      else if(cameraMode == false){
        SimbleeForMobile.setVisible(scanScreen1, false);
        SimbleeForMobile.setVisible(scanScreen2, false);
        SimbleeForMobile.setVisible(scanScreen3, false);
        SimbleeForMobile.setVisible(text1, false);
        SimbleeForMobile.setVisible(text2, false);
        SimbleeForMobile.setVisible(text3, false);
        SimbleeForMobile.setVisible(backButton, false);
        SimbleeForMobile.setVisible(acceptButton, false);
        SimbleeForMobile.setVisible(takePhotoButton, false);
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

void ui_event(event_t &event)
{
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
      wineChosen = clickedOverlay + ((winePage - 1) * 5);
      updatePage = true;
      if (!addMode) {
        SimbleeForMobile.showScreen(5);
      }
      else {
        SimbleeForMobile.updateColor(screen2Background, GE_RED);
        SimbleeForMobile.updateText(screen2Title, "Wine Inventory");
        wine[wineChosen].setIndex(addDetected);
        addMode = false;
        addDetected = -1;
        updatePage = true;
      }
    }
    else if (event.id == OverlayUI[1]) {
      clickedOverlay = 1;
      wineChosen = clickedOverlay + ((winePage - 1) * 5);
      updatePage = true;
      if (!addMode)
        SimbleeForMobile.showScreen(5);
      else {
        SimbleeForMobile.updateColor(screen2Background, GE_RED);
        SimbleeForMobile.updateText(screen2Title, "Wine Inventory");
        wine[wineChosen].setIndex(addDetected);
        addMode = false;
        addDetected = -1;
        updatePage = true;
      }

    }
    else if (event.id == OverlayUI[2]) {
      clickedOverlay = 2;
      wineChosen = clickedOverlay + ((winePage - 1) * 5);
      updatePage = true;
      if (!addMode)
        SimbleeForMobile.showScreen(5);
      else {
        SimbleeForMobile.updateColor(screen2Background, GE_RED);
        SimbleeForMobile.updateText(screen2Title, "Wine Inventory");
        wine[wineChosen].setIndex(addDetected);
        addMode = false;
        addDetected = -1;
        updatePage = true;
      }
    }
    else if (event.id == OverlayUI[3]) {
      clickedOverlay = 3;
      wineChosen = clickedOverlay + ((winePage - 1) * 5);
      updatePage = true;
      if (!addMode)
        SimbleeForMobile.showScreen(5);
      else {
        SimbleeForMobile.updateColor(screen2Background, GE_RED);
        SimbleeForMobile.updateText(screen2Title, "Wine Inventory");
        wine[wineChosen].setIndex(addDetected);
        addMode = false;
        addDetected = -1;
        updatePage = true;
      }
    }
    else if (event.id == OverlayUI[4]) {
      clickedOverlay = 4;
      wineChosen = clickedOverlay + ((winePage - 1) * 5);
      updatePage = true;
      if (!addMode)
        SimbleeForMobile.showScreen(5);
      else {
        SimbleeForMobile.updateColor(screen2Background, GE_RED);
        SimbleeForMobile.updateText(screen2Title, "Wine Inventory");
        wine[wineChosen].setIndex(addDetected);
        addMode = false;
        addDetected = -1;
        updatePage = true;
      }
    }
    else if (event.id == cameraButton){
      cameraMode = true;
    }
    else if (event.id == backButton){
      cameraMode = false;
    }
    else if (event.id == takePhotoButton){
      
    }
  }
  else if (SimbleeForMobile.screen == 3){
    if(event.id == backButton){
      Serial.println("going back to screen 2");
      updatePage = true;
      SimbleeForMobile.showScreen(2);
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

void ui()
{
  Serial.println(SimbleeForMobile.screen);
  switch (SimbleeForMobile.screen)
  {
    case 1:
      splashScreen();
      delay(3000);
      SimbleeForMobile.showScreen(2);
      break;

    case 2:
      inventoryScreen();
      Serial.println("here");
      break;

    case 3:
      cameraScreen();
      break;

    case 4:
      break;

    case 5:
      Serial.println("showscreen 5");
      addScreen();
      break;

    default:
      splashScreen();
      break;
  }
}

void SimbleeForMobile_onDisconnect() {
  disconnected = true;
  updatePage = true;
}

