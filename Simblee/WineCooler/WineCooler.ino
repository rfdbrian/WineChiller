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

//12/4 - Details for Wine, before Removal

#include "Slot.h"
#include "Wine.h"
#include <SimbleeForMobile.h>
#include <vector>

#include "WineSplashScreen_jpg.h"
#include "Wine2_png.h"
#include "camera_png.h"
#include "wineLabel_png.h"


#define GE_RED rgb(158,11,15)
#define GE_YELLOW rgb(243,165,25)
#define GE_GRAY rgb(159,159,159)
#define GE_DARKGRAY rgb(123,123,123)
#define GE_LIGHTGRAY rgb(194,193,193)
#define GE_BEIGE rgb(216,208,200)

//APP STATUS
//0 = default
//1 = insert wine
uint8_t APP_STATUS = 0;

bool initialized = false;
bool updatePage = false;
uint32_t nextButtonState[25];
uint32_t currentButtonState[25];
uint32_t wineStore[25];

bool disconnected = false;

int count = 0;

//UI ELEMENTS
uint8_t previousPage, nextPage, pageValue,
        wineText1, wineText2, wineText3, wineText4,
        addButton1, addButton2, addButton3, addButton4,
        insertScreen1, insertScreen2, insertScreen3, insertScreen4, insertScreen5, insertScreen6, insertScreen7,
        inventoryButton, catalogButton, scanButton,
        wineImage1, wineImage2, wineImage3, wineImage4,
        removeButton1, removeButton2, removeButton3, removeButton4,
        cellNumber1, cellNumber2, cellNumber3, cellNumber4,
        backButton, cameraButton, wineLabel, acceptButton,
        text1, text2, text3,
        addButton,
        removeScreen1, removeScreen2, removeScreen3, removeScreen4, removeScreen5, removeScreen6, removeScreen7,
        removeButton, returnButton, buyButton, consumeButton,
        VineyardUI[5], WineName1UI[5], WineName2UI[5], CountryUI[5], PriceUI[5], LocationUI[5], OverlayUI[5],
        VineyardUISingle, WineName1UISingle, WineName2UISingle, CountryUISingle, PriceUISingle, LocationUISingle,
        addPopUpDetectedText, addPopUpRackText, addPopUpRackNumber, addPopUpSelectText,
        removePopUpDetectedText, removePopUpRackText, removePopUpRackNumber, removePopUpSelectText,
        screen2Title, screen2Background;

uint8_t wineRect[25];


//0 = in possesion
//1 = in cooler
//2 = buy
uint8_t wineStatus[25] = {0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0
                         };
uint8_t chillerSlot[25] = {255, 255, 255, 255, 255,
                           255, 255, 255, 255, 255,
                           255, 255, 255, 255, 255,
                           255, 255, 255, 255, 255,
                           255, 255, 255, 255, 255
                          };

uint8_t rect[25];

int8_t winePage = 1,
       addedWine = 0,
       wineChosen = 0,
       addDetected = -1,
       removeDetected = -1,
       inventory = 0,
       scanned = 0;

int8_t     switchDetected = -1;


char *Vineyard[25] = {"Columbia Crest", "Carmen", "Francis Ford Coppola", "La Tercera", "Louis M. Martini", "Francis Ford Coppola", "Chateau Saint-Didier-Parnac", "Perez Cruz", "Marques De Riscal", "Baron De Marny"};
char *WineName1[25] = {"Grand Estates", "Grand Reserva", "Rosso & Blonco", "Reserva Rioja", "Sonoma County", "Diamond Collection Ivory", "Prieure de Cenac", "Reserva", "Proximo Roja", "Coteaux Du Languedoc"};
char *WineName2[25] = {"2003", "2012", "2011", "2010", "2013", "2013", "2014", "2013", "2010", "2012"};
char *Country[25] = {"Cabernet Sauvignon", "Cabernet Sauvignon", "Shiraz", "Spanish Red", "Cabernet Sauvignon", "Cabernet Sauvignon", "Malbec", "Cabernet Sauvignon", "Spanish Red", "Red"};
char *Price[25] = {"$11.25", "$14.50", "$12.00", "$17.98", "$14.99", "$14.98", "$7.98", "$14.46", "$9.48", "$10.99"};
char *Location[25] = {"", "", "", "", "",
                      "", "", "", "", "",
                      "", "", "", "", "",
                      "", "", "", "", "",
                      "", "", "", "", ""
                     };

char *Switch[25] = {"A1", "A2", "A3", "A4", "A5",
                    "B1", "B2", "B3", "B4", "B5",
                    "C1", "C2", "C3", "C4", "C5",
                    "D1", "D2", "D3", "D4", "D5",
                    "E1", "E2", "E3", "E4", "E5"
                   };

//  Wine.h describes a basic "Wine" class. There are the classic getter
//    methods for various static values. There exists only one setter value
//    for "index," which would be the index to the Slots vector defined below.
Wine wine1 = Wine("Columbia Crest", "Grand Estates", 2003, "Cabernet Sauvignon", "$11.24", 0);

uint8_t clickedOverlay = 0;

bool addMode = false;
bool removeMode = false;

#define color CRGB::Red
#define color2 CRGB::Green

#define LED_PIN 5
#define NUM_LEDS 25

CRGB leds[NUM_LEDS];


std::vector<Slot> Slots;

// Inializing vector of Slots for TESTBOX units.
//   Should not have problems with corresponding
//   switch and led positions. 



//SCREEN 1 BASE
void splashScreen()
{
  SimbleeForMobile.beginScreen(WHITE, PORTRAIT);
  SimbleeForMobile.drawRect(0, 0, 320, 570, GE_RED);
  SimbleeForMobile.drawText(20, 200, "LOADING", WHITE, 50);
  SimbleeForMobile.drawText(20, 250, "INTERFACE", WHITE, 50);
  //SimbleeForMobile.imageSource(1, JPG, WineSplashScreen_jpg, WineSplashScreen_jpg_len);
  //SimbleeForMobile.drawImage(1, 0, 0);
  SimbleeForMobile.endScreen();
}

//SCREEN 2 BASE
void catalogScreen()
{
  SimbleeForMobile.beginScreen(WHITE, PORTRAIT);

  //BACKGROUND
  screen2Background = SimbleeForMobile.drawRect(0, 0, 320, 480, GE_RED);
  SimbleeForMobile.drawRect(0, 60, 320,  360, GE_LIGHTGRAY);
  SimbleeForMobile.drawRect(0, 61, 320, 70, WHITE);
  SimbleeForMobile.drawRect(0, 133, 320, 70, WHITE);
  SimbleeForMobile.drawRect(0, 205, 320, 70, WHITE);
  SimbleeForMobile.drawRect(0, 277, 320, 70, WHITE);
  SimbleeForMobile.drawRect(0, 349, 320, 70, WHITE);




  //IMAGES
  SimbleeForMobile.drawRect(10, 66, 50, 60, BLACK);
  SimbleeForMobile.drawRect(10, 138, 50, 60, BLACK);
  SimbleeForMobile.drawRect(10, 210, 50, 60, BLACK);
  SimbleeForMobile.drawRect(10, 282, 50, 60, BLACK);
  SimbleeForMobile.drawRect(10, 354, 50, 60, BLACK);


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
    SimbleeForMobile.drawText(260, boxY[i], "LOCATION", GE_LIGHTGRAY, 12);
    //LOCATION
    LocationUI[i] = SimbleeForMobile.drawText(272, boxY[i] + 15, "", BLACK, 30);
    //INVISBLE OVERLAY
    OverlayUI[i] = SimbleeForMobile.drawRect(0, overlayY[i], 320, 80, rgba(0, 0, 0, 255));
    SimbleeForMobile.setEvents(OverlayUI[i], EVENT_PRESS);
  }











  //SEGMENT MENU
  //char *titles[] = { "Purchased", "Wish List", "Favorites" };
  //SimbleeForMobile.drawSegment(10, 60, 300, titles, 3, GE_YELLOW);

  //WINE IMAGES
  //SimbleeForMobile.imageSource(4, PNG, Wine2_png, Wine2_png_len);

  //WINE BOX 1
  //  wineImage1 = SimbleeForMobile.drawImage(4, 55, 105);
  //  wineText1 = SimbleeForMobile.drawText(10, 220, "Wine 1", GE_RED, 11);

  //WINE BOX 3
  //  wineImage3 = SimbleeForMobile.drawImage(4, 55, 305);
  //  wineText3 = SimbleeForMobile.drawText(170, 220, "Wine 2", GE_RED, 11);

  //WINE BOX 2
  //wineImage2 = SimbleeForMobile.drawImage(4, 215, 105);
  //  wineText2 = SimbleeForMobile.drawText(10, 420, "Wine 3", GE_RED, 11);

  //WINE BOX 4
  //wineImage4 = SimbleeForMobile.drawImage(4, 215, 305);
  //  wineText4 = SimbleeForMobile.drawText(170, 420, "Wine 4", GE_RED, 11);




  //  addButton1 = SimbleeForMobile.drawButton(100, 230, 80, "ADD", GE_YELLOW, 1);
  //  SimbleeForMobile.setEvents(addButton1, EVENT_PRESS);
  //  addButton2 = SimbleeForMobile.drawButton(260, 230, 80, "ADD", GE_YELLOW, 1);
  //  SimbleeForMobile.setEvents(addButton2, EVENT_PRESS);
  //  addButton3 = SimbleeForMobile.drawButton(100, 430, 80, "ADD", GE_YELLOW, 1);
  //  SimbleeForMobile.setEvents(addButton3, EVENT_PRESS);
  //  addButton4 = SimbleeForMobile.drawButton(260, 430, 80, "ADD", GE_YELLOW, 1);
  //  SimbleeForMobile.setEvents(addButton4, EVENT_PRESS);

  //SHOWWINEINSERTSCREEN

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


  //PAGE SELECTION
  SimbleeForMobile.drawText(134 , 437, "Page", WHITE, 16);
  pageValue = SimbleeForMobile.drawText(174, 437, 1, WHITE, 16);
  previousPage = SimbleeForMobile.drawButton(2, 432, 20, "<<", WHITE, 1);
  SimbleeForMobile.setEvents(previousPage, EVENT_PRESS);
  nextPage = SimbleeForMobile.drawButton(300, 432, 20, ">>", WHITE, 1);
  SimbleeForMobile.setEvents(nextPage, EVENT_PRESS);

  SimbleeForMobile.endScreen();
}

//SCREEN 3 BASE
void myInventoryScreen()
{
  SimbleeForMobile.beginScreen(WHITE, PORTRAIT);
  SimbleeForMobile.drawRect(0, 0, 320, 570, GE_RED);
  SimbleeForMobile.drawRect(0, 60, 320, 400, WHITE);
  SimbleeForMobile.drawRect(159, 60, 2, 400, GRAY);
  SimbleeForMobile.drawRect(0, 259, 320, 2, GRAY);
  SimbleeForMobile.drawText(80, 30, "Wine Inventory", WHITE, 25);
  catalogButton = SimbleeForMobile.drawButton(60, 520, 200, "Catalog", GE_YELLOW);
  SimbleeForMobile.setEvents(catalogButton, EVENT_PRESS);
  //SimbleeForMobile.imageSource(4, PNG, Wine2_png, Wine2_png_len);
  wineImage1 = SimbleeForMobile.drawImage(4, 55, 70);
  SimbleeForMobile.setEvents(wineImage1, EVENT_PRESS);
  SimbleeForMobile.setVisible(wineImage1, false);
  wineImage3 = SimbleeForMobile.drawImage(4, 55, 270);
  SimbleeForMobile.setEvents(wineImage3, EVENT_PRESS);
  SimbleeForMobile.setVisible(wineImage3, false);
  wineImage2 = SimbleeForMobile.drawImage(4, 215, 70);
  SimbleeForMobile.setEvents(wineImage2, EVENT_PRESS);
  SimbleeForMobile.setVisible(wineImage2, false);
  wineImage4 = SimbleeForMobile.drawImage(4, 215, 270);
  SimbleeForMobile.setEvents(wineImage4, EVENT_PRESS);
  SimbleeForMobile.setVisible(wineImage4, false);
  wineText1 = SimbleeForMobile.drawText(10, 220, "Wine 1", GE_RED, 11);
  SimbleeForMobile.setVisible(wineText1, false);
  wineText3 = SimbleeForMobile.drawText(170, 220, "Wine 2", GE_RED, 11);
  SimbleeForMobile.setVisible(wineText3, false);
  wineText2 = SimbleeForMobile.drawText(10, 420, "Wine 3", GE_RED, 11);
  SimbleeForMobile.setVisible(wineText2, false);
  wineText4 = SimbleeForMobile.drawText(170, 420, "Wine 4", GE_RED, 11);
  SimbleeForMobile.setVisible(wineText4, false);
  SimbleeForMobile.drawText(134 , 480, "Page", WHITE, 18);
  pageValue = SimbleeForMobile.drawText(174, 480, 1, WHITE, 18);
  previousPage = SimbleeForMobile.drawButton(20, 475, 100, "Previous", WHITE);
  nextPage = SimbleeForMobile.drawButton(200, 475, 100, "Next", WHITE);
  removeButton1 = SimbleeForMobile.drawButton(75, 230, 100, "SELECT", GE_YELLOW, 1);
  SimbleeForMobile.setEvents(removeButton1, EVENT_PRESS);
  SimbleeForMobile.setVisible(removeButton1, false);
  removeButton2 = SimbleeForMobile.drawButton(235, 230, 100, "SELECT", GE_YELLOW, 1);
  SimbleeForMobile.setEvents(removeButton2, EVENT_PRESS);
  SimbleeForMobile.setVisible(removeButton2, false);
  removeButton3 = SimbleeForMobile.drawButton(75, 430, 100, "SELECT", GE_YELLOW, 1);
  SimbleeForMobile.setEvents(removeButton3, EVENT_PRESS);
  SimbleeForMobile.setVisible(removeButton3, false);
  removeButton4 = SimbleeForMobile.drawButton(235, 430, 100, "SELECT", GE_YELLOW, 1);
  SimbleeForMobile.setEvents(removeButton4, EVENT_PRESS);
  SimbleeForMobile.setVisible(removeButton4, false);
  SimbleeForMobile.setEvents(previousPage, EVENT_PRESS);
  SimbleeForMobile.setEvents(nextPage, EVENT_PRESS);

  //SHOWWINEINSERTSCREEN
  insertScreen1 = SimbleeForMobile.drawRect(0, 0, 320, 570, rgba(155, 155, 155, 50));
  SimbleeForMobile.setVisible(insertScreen1, false);
  insertScreen2 = SimbleeForMobile.drawRect(20, 200, 280, 150, GE_RED);
  SimbleeForMobile.setVisible(insertScreen2, false);
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
  cellNumber1 = SimbleeForMobile.drawText(2, 62, "1", GE_YELLOW, 20);
  cellNumber2 = SimbleeForMobile.drawText(162, 62, "2", GE_YELLOW, 20);
  cellNumber3 = SimbleeForMobile.drawText(2, 262, "3", GE_YELLOW, 20);
  cellNumber4 = SimbleeForMobile.drawText(162, 262, "4", GE_YELLOW, 20);



  //SHOW REMOVE SCREEN
  removeScreen1 = SimbleeForMobile.drawRect(0, 0, 320, 570, rgba(155, 155, 155, 50));
  SimbleeForMobile.setVisible(removeScreen1, false);
  removeScreen2 = SimbleeForMobile.drawRect(20, 200, 280, 150, GE_RED);
  SimbleeForMobile.setVisible(removeScreen2, false);
  removeScreen3 = SimbleeForMobile.drawText(60, 220, "Please remove wine from rack:", WHITE, 18);
  SimbleeForMobile.setVisible(removeScreen3, false);
  removeScreen4 = SimbleeForMobile.drawText(150, 260, 1, WHITE, 40);
  SimbleeForMobile.setVisible(removeScreen4, false);
  //removeScreen5 = SimbleeForMobile.drawText(110, 240, 1, WHITE, 22);
  // SimbleeForMobile.setVisible(removeScreen5, false);
  // removeScreen6 = SimbleeForMobile.drawText(60, 240, "Wine", WHITE, 22);
  // SimbleeForMobile.setVisible(removeScreen6, false);
  // removeScreen7 = SimbleeForMobile.drawText(60, 260, "Placed in rack #", WHITE, 22);
  // SimbleeForMobile.setVisible(removeScreen7, false);
  winePage = 1;
  SimbleeForMobile.endScreen();

}


//SCREEN 4 BASE
void scanScreen()
{
  SimbleeForMobile.beginScreen();
  //BACKGROUND
  SimbleeForMobile.drawRect(0, 0, 320, 480, GE_RED);
  SimbleeForMobile.drawRect(0, 60, 320,  180, GE_LIGHTGRAY);
  SimbleeForMobile.drawRect(0, 240, 320, 180, WHITE);

  //WINE
  SimbleeForMobile.imageSource(7, PNG, wineLabel_png, wineLabel_png_len);
  wineLabel = SimbleeForMobile.drawImage(7, 30, 90);
  SimbleeForMobile.setVisible(wineLabel, false);

  //Label
  text1 = SimbleeForMobile.drawText(118, 190, "POSITION THE", WHITE, 14);
  text2 = SimbleeForMobile.drawText(90, 200, "WINE LABEL", WHITE, 28);
  text3 = SimbleeForMobile.drawText(100, 230, "WITHIN THIS FRAME", WHITE, 14);


  //BOTTOM MENU
  backButton = SimbleeForMobile.drawButton(40, 390, 40, "BACK", WHITE, 1);
  SimbleeForMobile.setEvents(backButton, EVENT_PRESS);
  SimbleeForMobile.imageSource(5, PNG, camera_png, camera_png_len);
  cameraButton = SimbleeForMobile.drawImage(5, 125, 495);
  SimbleeForMobile.setEvents(cameraButton, EVENT_PRESS);
  acceptButton = SimbleeForMobile.drawButton(220, 390, 60, "ACCEPT", WHITE, 1);
  SimbleeForMobile.setEvents(acceptButton, EVENT_PRESS);
  SimbleeForMobile.setVisible(acceptButton, false);

  SimbleeForMobile.endScreen();
}


//SCREEN 5 BASE
void addScreen()
{
  SimbleeForMobile.beginScreen();

  //BACKGROUND
  SimbleeForMobile.drawRect(0, 0, 320, 480, GE_RED);
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


  //WINE IMAGE
  //  SimbleeForMobile.imageSource(4, PNG, Wine2_png, Wine2_png_len);
  //  SimbleeForMobile.drawImage(4, 135, 100);

  //INFO
  consumeButton = SimbleeForMobile.drawButton(40, 330, 240, "CONSUME/GIFT", BLACK, 2);
  SimbleeForMobile.setEvents(consumeButton, EVENT_PRESS);
  SimbleeForMobile.setVisible(consumeButton, false);
  addButton = SimbleeForMobile.drawButton(40, 375, 240, "ADD TO CHILLER", BLACK, 2);
  SimbleeForMobile.setEvents(addButton, EVENT_PRESS);
  SimbleeForMobile.setVisible(addButton, false);
  removeButton = SimbleeForMobile.drawButton(40, 375, 240, "REMOVE FROM CHILLER", RED, 2);
  SimbleeForMobile.setEvents(removeButton, EVENT_PRESS);
  SimbleeForMobile.setVisible(removeButton, false);
  returnButton = SimbleeForMobile.drawButton(40, 435, 240, "RETURN TO INVENTORY", BLACK, 1);
  SimbleeForMobile.setEvents(returnButton, EVENT_PRESS);
  //SimbleeForMobile.setVisible(returnButton,false);
  buyButton = SimbleeForMobile.drawButton(40, 375, 240, "PURCHASE WINE", GREEN, 2);
  SimbleeForMobile.setEvents(buyButton, EVENT_PRESS);
  SimbleeForMobile.setVisible(buyButton, false);



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



void checkButtonState(Slot toButton, int cellID) {
  if (toButton.isPressed()) {
    if (initialized == false) {
      currentButtonState[cellID] = true;
      //someLED = color;
    }
    else
      nextButtonState[cellID] = true;
   // someLED = color;
  }
  else if (toButton.isReleased()) {
    if (initialized == false) {
      currentButtonState[cellID] = false;

    }
    else
      nextButtonState[cellID] = false;
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
    if (Slots[i].onPressed()) {
      Slots[i].updateColor(color2);
    } else if (Slots[i].onReleased()) {
      Slots[i].updateColor(color);
    }
  }
}

void checkRemove() {
  for (int i = 0; i < 25; i++) {
    if (currentButtonState[i] > nextButtonState[i]) {
      removeDetected = i;
      currentButtonState[i] = nextButtonState[i];
      updatePage = true;
      removeMode = true;
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
  leds[switchDetected] = color;
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
  //FastLED.show();
  delay(100);
  initialized = true;
  compareStates();
}


void removeBottle()
{
  SimbleeForMobile.setVisible(removeScreen1, true);
  SimbleeForMobile.setVisible(removeScreen2, true);
  SimbleeForMobile.setVisible(removeScreen3, true);
  SimbleeForMobile.updateText(removeScreen4, Location[clickedOverlay + ((winePage - 1) * 5)]);
  SimbleeForMobile.setVisible(removeScreen4, true);
  for(int i=0;i<25;i++){
    if(Switch[i] == Location[clickedOverlay + ((winePage - 1) * 5)]){
      leds[i] = color2;
      break;
    }
  }
  //Serial.println(removeDetected);
  FastLED.show();
  while (nextButtonState[chillerSlot[clickedOverlay + ((winePage - 1) * 5)]] == true)
  {
    checkAllButtons();
  }
  wineStatus[clickedOverlay + ((winePage - 1) * 5)] = 0;
  chillerSlot[clickedOverlay + ((winePage - 1) * 5)] = 255;
  Location[clickedOverlay + ((winePage - 1) * 5)] = "";
  SimbleeForMobile.setVisible(removeScreen4, false);
  SimbleeForMobile.setVisible(removeScreen1, false);
  SimbleeForMobile.setVisible(removeScreen2, false);
  SimbleeForMobile.setVisible(removeScreen3, false);
  FastLED.clear();
  FastLED.show();
  delay(100);
  SimbleeForMobile.showScreen(2);
}

void consumeBottle()
{
  SimbleeForMobile.setVisible(removeScreen1, true);
  SimbleeForMobile.setVisible(removeScreen2, true);
  SimbleeForMobile.setVisible(removeScreen3, true);
  SimbleeForMobile.updateText(removeScreen4, Location[clickedOverlay + ((winePage - 1) * 5)]);
  SimbleeForMobile.setVisible(removeScreen4, true);
  FastLED.clear();
  leds[((winePage - 1) * 4) + wineChosen - 1] = color2;
  FastLED.show();
  while (nextButtonState[chillerSlot[clickedOverlay + ((winePage - 1) * 5)]] == true)
  {
    checkAllButtons();
  }
  wineStatus[clickedOverlay + ((winePage - 1) * 5)] = 2;
  chillerSlot[clickedOverlay + ((winePage - 1) * 5)] = 255;
  Location[clickedOverlay + ((winePage - 1) * 5)] = "";
  SimbleeForMobile.setVisible(removeScreen4, false);
  SimbleeForMobile.setVisible(removeScreen1, false);
  SimbleeForMobile.setVisible(removeScreen2, false);
  SimbleeForMobile.setVisible(removeScreen3, false);
  FastLED.clear();
  FastLED.show();
  delay(100);
  SimbleeForMobile.showScreen(2);
}

void buyBottle()
{
  wineStatus[clickedOverlay + ((winePage - 1) * 5)] = 0;
}


void setup()
{
    
    for (int i = 0; i < 25; i++) {
      if (i == 1) {
        Slot tempSlotEdgeOne = Slot(30, leds[1]); 
        Slots.push_back(tempSlotEdgeOne);
      } else if (i > 4) {
        Slot tempSlotEdgeTwo = Slot(i + 1, leds[i]);
        Slots.push_back(tempSlotEdgeTwo);
      }
    }    

  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
  checkAllButtons();
  SimbleeForMobile.deviceName = "Wine";
  SimbleeForMobile.advertisementData = "Chiller";
  SimbleeForMobile.domain = "FirstBuild.simblee.com";
  SimbleeForMobile.begin();
}

void loop()
{
  //Serial.println(chillerSlot[5]);
  //printf("%d,%d\n",SimbleeForMobile.screenWidth,SimbleeForMobile.screenHeight);
  if (disconnected == true)
  {
    SimbleeForMobile.showScreen(2);
    disconnected = false;
  }

  checkAllButtons();
  checkAdd();
  checkRemove();


  if (SimbleeForMobile.updatable)
  {
    //SCREEN 1 UPDATES
    if (SimbleeForMobile.screen == 1)
    {
    }
    //SCREEN 2 UPDATES
    else if (SimbleeForMobile.screen == 2) {
      if (updatePage == true)
      {

        
        SimbleeForMobile.updateValue(pageValue, winePage);

                if (addDetected >= 0) {
                  FastLED.show();
          SimbleeForMobile.updateText(addPopUpRackNumber, Switch[addDetected]);
          SimbleeForMobile.setVisible(insertScreen1, true);
          SimbleeForMobile.setVisible(insertScreen2, true);
          SimbleeForMobile.setVisible(addPopUpDetectedText, true);
          SimbleeForMobile.setVisible(addPopUpRackText, true);
          SimbleeForMobile.setVisible(addPopUpRackNumber, true);
          SimbleeForMobile.setVisible(addPopUpSelectText, true);
          //SimbleeForMobile.setVisible(insertScreen8, true);
          delay(3000);
          SimbleeForMobile.setVisible(insertScreen1, false);
          SimbleeForMobile.setVisible(insertScreen2, false);
          SimbleeForMobile.setVisible(addPopUpDetectedText, false);
          SimbleeForMobile.setVisible(addPopUpRackText, false);
          SimbleeForMobile.setVisible(addPopUpRackNumber, false);
          SimbleeForMobile.setVisible(addPopUpSelectText, false);
          //SimbleeForMobile.setVisible(insertScreen8, false);

          SimbleeForMobile.updateColor(screen2Background, BLUE);
          SimbleeForMobile.updateText(screen2Title, "Select Wine");
          FastLED.clear();
          FastLED.show();

        }
        if (removeDetected >= 0) {
       
          int bottle;
          for (int i = 0; i < 25; i++) {
            if (Location[i] == Switch[removeDetected]) {
              bottle = i;
            }
          }
          Serial.println(bottle);
          SimbleeForMobile.updateText(removePopUpRackNumber, Switch[removeDetected]);
          SimbleeForMobile.setVisible(insertScreen1, true);
          SimbleeForMobile.setVisible(insertScreen2, true);
          SimbleeForMobile.setVisible(removePopUpDetectedText, true);
          SimbleeForMobile.setVisible(removePopUpRackText, true);
          SimbleeForMobile.setVisible(removePopUpRackNumber, true);
          wineStatus[bottle] = 0;

          chillerSlot[bottle] = removeDetected;
          delay(3000);
          SimbleeForMobile.setVisible(insertScreen1, false);
          SimbleeForMobile.setVisible(insertScreen2, false);
          SimbleeForMobile.setVisible(removePopUpDetectedText, false);
          SimbleeForMobile.setVisible(removePopUpRackText, false);
          SimbleeForMobile.setVisible(removePopUpRackNumber, false);
          removeDetected = -1;

        }

        
        for (int i = 0; i < 5; i++) {
          SimbleeForMobile.updateText(VineyardUI[i], Vineyard[i + ((winePage - 1) * 5)]);
          SimbleeForMobile.updateText(WineName1UI[i], WineName1[i + ((winePage - 1) * 5)]);
          SimbleeForMobile.updateText(WineName2UI[i], WineName2[i + ((winePage - 1) * 5)]);
          SimbleeForMobile.updateText(CountryUI[i], Country[i + ((winePage - 1) * 5)]);
          SimbleeForMobile.updateText(PriceUI[i], Price[i + ((winePage - 1) * 5)]);
          if (wineStatus[i + ((winePage - 1) * 5)] == 0) {
            SimbleeForMobile.updateColor(LocationUI[i], BLACK);
            SimbleeForMobile.updateText(LocationUI[i], "00");
          }
          else if (wineStatus[i + ((winePage - 1) * 5)] == 1) {
            SimbleeForMobile.updateColor(LocationUI[i], BLUE);
            SimbleeForMobile.updateText(LocationUI[i], Location[i + ((winePage - 1) * 5)]);
          }
          else if (wineStatus[i + ((winePage - 1) * 5)] == 2) {
            SimbleeForMobile.updateColor(LocationUI[i], RED);
            SimbleeForMobile.updateText(LocationUI[i], "NA");
          }
        }

        updatePage = false;
      }


    }
    //    else if (SimbleeForMobile.screen == 2)
    //    {
    //      if (addDetected > 0){
    //        SimbleeForMobile.setVisible(insertScreen1, true);
    //        SimbleeForMobile.setVisible(insertScreen2, true);
    //        //SimbleeForMobile.setVisible(insertScreen8, true);
    //        delay(3000);
    //        SimbleeForMobile.setVisible(insertScreen1, false);
    //        SimbleeForMobile.setVisible(insertScreen2, false);
    //        //SimbleeForMobile.setVisible(insertScreen8, false);
    //        addDetected = 0;
    //      }
    //      if (switchDetected > 0)
    //      {
    //        SimbleeForMobile.updateValue(insertScreen5, addedWine);
    //        SimbleeForMobile.updateValue(insertScreen4, switchDetected + 1);
    //        SimbleeForMobile.setVisible(insertScreen4, true);
    //        SimbleeForMobile.setVisible(insertScreen5, true);
    //        SimbleeForMobile.setVisible(insertScreen6, true);
    //        SimbleeForMobile.setVisible(insertScreen7, true);
    //        delay(3000);
    //        SimbleeForMobile.setVisible(insertScreen1, false);
    //        SimbleeForMobile.setVisible(insertScreen2, false);
    //        SimbleeForMobile.setVisible(insertScreen3, false);
    //        SimbleeForMobile.setVisible(insertScreen4, false);
    //        SimbleeForMobile.setVisible(insertScreen5, false);
    //        SimbleeForMobile.setVisible(insertScreen6, false);
    //        SimbleeForMobile.setVisible(insertScreen7, false);
    //        initialized = false;
    //        FastLED.clear();
    //        FastLED.show();
    //        switchDetected = 0;
    //      }
    //      if (updatePage = true)
    //      {
    //        SimbleeForMobile.updateValue(pageValue, winePage);
    //        switch (winePage)
    //        {
    //          case 1:
    //            SimbleeForMobile.setVisible(wineImage1, true);
    //            SimbleeForMobile.setVisible(wineImage2, true);
    //            SimbleeForMobile.setVisible(wineImage3, true);
    //            SimbleeForMobile.setVisible(wineImage4, true);
    //            SimbleeForMobile.updateText(wineText1, "Wine 1");
    //            SimbleeForMobile.updateText(wineText2, "Wine 2");
    //            SimbleeForMobile.updateText(wineText3, "Wine 3");
    //            SimbleeForMobile.updateText(wineText4, "Wine 4");
    //            break;
    //          case 2:
    //            SimbleeForMobile.setVisible(wineImage1, true);
    //            SimbleeForMobile.setVisible(wineImage2, true);
    //            SimbleeForMobile.setVisible(wineImage3, true);
    //            SimbleeForMobile.setVisible(wineImage4, true);
    //            SimbleeForMobile.updateText(wineText1, "Wine 5");
    //            SimbleeForMobile.updateText(wineText2, "Wine 6");
    //            SimbleeForMobile.updateText(wineText3, "Wine 7");
    //            SimbleeForMobile.updateText(wineText4, "Wine 8");
    //            break;
    //          case 3:
    //            SimbleeForMobile.setVisible(wineImage1, true);
    //            SimbleeForMobile.setVisible(wineImage2, true);
    //            SimbleeForMobile.setVisible(wineImage3, true);
    //            SimbleeForMobile.setVisible(wineImage4, true);
    //            SimbleeForMobile.updateText(wineText1, "Wine 9");
    //            SimbleeForMobile.updateText(wineText2, "Wine 10");
    //            SimbleeForMobile.updateText(wineText3, "Wine 11");
    //            SimbleeForMobile.updateText(wineText4, "Wine 12");
    //            break;
    //          case 4:
    //            SimbleeForMobile.setVisible(wineImage1, true);
    //            SimbleeForMobile.setVisible(wineImage2, true);
    //            SimbleeForMobile.setVisible(wineImage3, true);
    //            SimbleeForMobile.setVisible(wineImage4, true);
    //            SimbleeForMobile.updateText(wineText1, "Wine 13");
    //            SimbleeForMobile.updateText(wineText2, "Wine 14");
    //            SimbleeForMobile.updateText(wineText3, "Wine 15");
    //            SimbleeForMobile.updateText(wineText4, "Wine 16");
    //            break;
    //          case 5:
    //            SimbleeForMobile.setVisible(wineImage1, true);
    //            SimbleeForMobile.setVisible(wineImage2, true);
    //            SimbleeForMobile.setVisible(wineImage3, true);
    //            SimbleeForMobile.setVisible(wineImage4, true);
    //            SimbleeForMobile.updateText(wineText1, "Wine 17");
    //            SimbleeForMobile.updateText(wineText2, "Wine 18");
    //            SimbleeForMobile.updateText(wineText3, "Wine 19");
    //            SimbleeForMobile.updateText(wineText4, "Wine 20");
    //            break;
    //          case 6:
    //            SimbleeForMobile.setVisible(wineImage1, true);
    //            SimbleeForMobile.setVisible(wineImage2, true);
    //            SimbleeForMobile.setVisible(wineImage3, true);
    //            SimbleeForMobile.setVisible(wineImage4, true);
    //            SimbleeForMobile.updateText(wineText1, "Wine 21");
    //            SimbleeForMobile.updateText(wineText2, "Wine 22");
    //            SimbleeForMobile.updateText(wineText3, "Wine 23");
    //            SimbleeForMobile.updateText(wineText4, "Wine 24");
    //            break;
    //          case 7:
    //            SimbleeForMobile.updateText(wineText1, "Wine 25");
    //            SimbleeForMobile.updateText(wineText2, "");
    //            SimbleeForMobile.updateText(wineText3, "");
    //            SimbleeForMobile.updateText(wineText4, "");
    //            SimbleeForMobile.setVisible(wineImage2, false);
    //            SimbleeForMobile.setVisible(wineImage3, false);
    //            SimbleeForMobile.setVisible(wineImage4, false);
    //            break;
    //        }
    //        updatePage = false;
    //      }
    //    }
    //SCREEN 3 UPDATES
    else if (SimbleeForMobile.screen == 3)
    {
      if (updatePage = true)
      {
        SimbleeForMobile.updateValue(pageValue, winePage);
        switch (winePage)
        {
          case 1:
            SimbleeForMobile.updateText(cellNumber1, "1");
            SimbleeForMobile.updateText(cellNumber2, "2");
            SimbleeForMobile.updateText(cellNumber3, "3");
            SimbleeForMobile.updateText(cellNumber4, "4");
            if (currentButtonState[0] == 1)
            {
              SimbleeForMobile.setVisible(wineImage1, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage1, false);
            }
            if (currentButtonState[1] == 1) {
              SimbleeForMobile.setVisible(wineImage2, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage2, false);
            }
            if (currentButtonState[2] == 1) {
              SimbleeForMobile.setVisible(wineImage3, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage3, false);
            }
            if (currentButtonState[3] == 1) {
              SimbleeForMobile.setVisible(wineImage4, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage4, false);
            }
            break;
          case 2:
            SimbleeForMobile.updateText(cellNumber1, "5");
            SimbleeForMobile.updateText(cellNumber2, "6");
            SimbleeForMobile.updateText(cellNumber3, "7");
            SimbleeForMobile.updateText(cellNumber4, "8");
            if (currentButtonState[4] == 1)
            {
              SimbleeForMobile.setVisible(wineImage1, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage1, false);
            }
            if (currentButtonState[5] == 1) {
              SimbleeForMobile.setVisible(wineImage2, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage2, false);
            }
            if (currentButtonState[6] == 1) {
              SimbleeForMobile.setVisible(wineImage3, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage3, false);
            }
            if (currentButtonState[7] == 1) {
              SimbleeForMobile.setVisible(wineImage4, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage4, false);
            }
            break;
          case 3:
            SimbleeForMobile.updateText(cellNumber1, "9");
            SimbleeForMobile.updateText(cellNumber2, "10");
            SimbleeForMobile.updateText(cellNumber3, "11");
            SimbleeForMobile.updateText(cellNumber4, "12");
            if (currentButtonState[8] == 1)
            {
              SimbleeForMobile.setVisible(wineImage1, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage1, false);
            }
            if (currentButtonState[9] == 1) {
              SimbleeForMobile.setVisible(wineImage2, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage2, false);
            }
            if (currentButtonState[10] == 1) {
              SimbleeForMobile.setVisible(wineImage3, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage3, false);
            }
            if (currentButtonState[11] == 1) {
              SimbleeForMobile.setVisible(wineImage4, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage4, false);
            }
            break;
          case 4:
            SimbleeForMobile.updateText(cellNumber1, "13");
            SimbleeForMobile.updateText(cellNumber2, "14");
            SimbleeForMobile.updateText(cellNumber3, "15");
            SimbleeForMobile.updateText(cellNumber4, "16");
            if (currentButtonState[12] == 1)
            {
              SimbleeForMobile.setVisible(wineImage1, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage1, false);
            }
            if (currentButtonState[13] == 1) {
              SimbleeForMobile.setVisible(wineImage2, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage2, false);
            }
            if (currentButtonState[14] == 1) {
              SimbleeForMobile.setVisible(wineImage3, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage3, false);
            }
            if (currentButtonState[15] == 1) {
              SimbleeForMobile.setVisible(wineImage4, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage4, false);
            }
            break;
          case 5:
            SimbleeForMobile.updateText(cellNumber1, "17");
            SimbleeForMobile.updateText(cellNumber2, "18");
            SimbleeForMobile.updateText(cellNumber3, "19");
            SimbleeForMobile.updateText(cellNumber4, "20");
            if (currentButtonState[16] == 1)
            {
              SimbleeForMobile.setVisible(wineImage1, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage1, false);
            }
            if (currentButtonState[17] == 1) {
              SimbleeForMobile.setVisible(wineImage2, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage2, false);
            }
            if (currentButtonState[18] == 1) {
              SimbleeForMobile.setVisible(wineImage3, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage3, false);
            }
            if (currentButtonState[19] == 1) {
              SimbleeForMobile.setVisible(wineImage4, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage4, false);
            }
            break;
          case 6:
            SimbleeForMobile.updateText(cellNumber1, "21");
            SimbleeForMobile.updateText(cellNumber2, "22");
            SimbleeForMobile.updateText(cellNumber3, "23");
            SimbleeForMobile.updateText(cellNumber4, "24");
            if (currentButtonState[20] == 1)
            {
              SimbleeForMobile.setVisible(wineImage1, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage1, false);
            }
            if (currentButtonState[21] == 1) {
              SimbleeForMobile.setVisible(wineImage2, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage2, false);
            }
            if (currentButtonState[22] == 1) {
              SimbleeForMobile.setVisible(wineImage3, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage3, false);
            }
            if (currentButtonState[23] == 1) {
              SimbleeForMobile.setVisible(wineImage4, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage4, false);
            }
            break;
          case 7:
            SimbleeForMobile.updateText(cellNumber1, "25");
            SimbleeForMobile.updateText(cellNumber2, "");
            SimbleeForMobile.updateText(cellNumber3, "");
            SimbleeForMobile.updateText(cellNumber4, "");
            if (currentButtonState[24] == 1)
            {
              SimbleeForMobile.setVisible(wineImage1, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage1, false);
            }
            SimbleeForMobile.setVisible(wineImage2, false);
            SimbleeForMobile.setVisible(wineImage3, false);
            SimbleeForMobile.setVisible(wineImage4, false);

            break;
        }

        updatePage = false;
      }
    }
    else if (SimbleeForMobile.screen == 4)
    {
    }



    //SCREEN 5 *******************************************************************************************
    else if (SimbleeForMobile.screen == 5)
    {
      SimbleeForMobile.updateText(VineyardUISingle, Vineyard[clickedOverlay + ((winePage - 1) * 5)]);
      SimbleeForMobile.updateText(WineName1UISingle, WineName1[clickedOverlay + ((winePage - 1) * 5)]);
      SimbleeForMobile.updateText(WineName2UISingle, WineName2[clickedOverlay + ((winePage - 1) * 5)]);
      SimbleeForMobile.updateText(CountryUISingle, Country[clickedOverlay + ((winePage - 1) * 5)]);
      SimbleeForMobile.updateText(PriceUISingle, Price[clickedOverlay + ((winePage - 1) * 5)]);
      SimbleeForMobile.updateText(LocationUISingle, Location[clickedOverlay + ((winePage - 1) * 5)]);
      if (wineStatus[clickedOverlay + ((winePage - 1) * 5)] == 0) {

        SimbleeForMobile.setVisible(removeButton, false);
        SimbleeForMobile.setVisible(buyButton, false);
        SimbleeForMobile.setVisible(consumeButton, true);
        SimbleeForMobile.setVisible(addButton, true);
      }
      else if (wineStatus[clickedOverlay + ((winePage - 1) * 5)] == 1) {
        SimbleeForMobile.setVisible(addButton, false);
        SimbleeForMobile.setVisible(buyButton, false);
        SimbleeForMobile.setVisible(consumeButton, true);
        SimbleeForMobile.setVisible(removeButton, true);
      }
      else if (wineStatus[clickedOverlay + ((winePage - 1) * 5)] == 2) {
        SimbleeForMobile.setVisible(addButton, false);
        SimbleeForMobile.setVisible(removeButton, false);
        SimbleeForMobile.setVisible(consumeButton, false);
        SimbleeForMobile.setVisible(buyButton, true);
      }
      if (switchDetected >= 0)
      {
        SimbleeForMobile.updateText(insertScreen5, WineName1[clickedOverlay + ((winePage - 1) * 5)]);
        SimbleeForMobile.updateValue(insertScreen4, switchDetected);
        wineStatus[clickedOverlay + ((winePage - 1) * 5)] = 1;
        Location[clickedOverlay + ((winePage - 1) * 5)] = Switch[switchDetected];
        chillerSlot[clickedOverlay + ((winePage - 1) * 5)] = switchDetected;
        //chillerSlot[switchDetected] = clickedOverlay + ((winePage - 1) * 5);
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
  //SCREEN 1 EVENTS
  if (SimbleeForMobile.screen == 1)
  {
  }

  //SCREEN 2 EVENTS
  else if (SimbleeForMobile.screen == 2)
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
        wineStatus[clickedOverlay + ((winePage - 1) * 5)] = 1;
        SimbleeForMobile.updateColor(screen2Background, GE_RED);
        SimbleeForMobile.updateText(screen2Title, "Wine Inventory");
        Location[clickedOverlay + ((winePage - 1) * 5)] = Switch[addDetected];
        chillerSlot[clickedOverlay + ((winePage - 1) * 5)] = addDetected;
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
        wineStatus[clickedOverlay + ((winePage - 1) * 5)] = 1;
        SimbleeForMobile.updateColor(screen2Background, GE_RED);
        SimbleeForMobile.updateText(screen2Title, "Wine Inventory");
        Location[clickedOverlay + ((winePage - 1) * 5)] = Switch[addDetected];
        chillerSlot[clickedOverlay + ((winePage - 1) * 5)] = addDetected;
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
        wineStatus[clickedOverlay + ((winePage - 1) * 5)] = 1;
        SimbleeForMobile.updateColor(screen2Background, GE_RED);
        SimbleeForMobile.updateText(screen2Title, "Wine Inventory");
        Location[clickedOverlay + ((winePage - 1) * 5)] = Switch[addDetected];
        chillerSlot[clickedOverlay + ((winePage - 1) * 5)] = addDetected;
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
        wineStatus[clickedOverlay + ((winePage - 1) * 5)] = 1;
        SimbleeForMobile.updateColor(screen2Background, GE_RED);
        SimbleeForMobile.updateText(screen2Title, "Wine Inventory");
        Location[clickedOverlay + ((winePage - 1) * 5)] = Switch[addDetected];
        chillerSlot[clickedOverlay + ((winePage - 1) * 5)] = addDetected;
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
        wineStatus[clickedOverlay + ((winePage - 1) * 5)] = 1;
        SimbleeForMobile.updateColor(screen2Background, GE_RED);
        SimbleeForMobile.updateText(screen2Title, "Wine Inventory");
        Location[clickedOverlay + ((winePage - 1) * 5)] = Switch[addDetected];
        chillerSlot[clickedOverlay + ((winePage - 1) * 5)] = addDetected;
        addMode = false;
        addDetected = -1;
        updatePage = true;
      }
    }
    //    else if (event.id == addButton1)
    //    {
    //      addedWine = (winePage * 4) - 3;
    //      showWineInsertScreen();
    //    }
    //    else if (event.id == addButton2)
    //    {
    //      addedWine = (winePage * 4) - 2;
    //      showWineInsertScreen();
    //    }
    //    else if (event.id == addButton3)
    //    {
    //      addedWine = (winePage * 4) - 1;
    //      showWineInsertScreen();
    //    }
    //    else if (event.id == addButton4)
    //    {
    //      addedWine = (winePage * 4) - 0;
    //      showWineInsertScreen();
    //    }
    //    else if (event.id == inventoryButton)
    //    {
    //      updatePage = true;
    //      SimbleeForMobile.showScreen(3);
    //    }
    //    else if (event.id == scanButton)
    //    {
    //      SimbleeForMobile.showScreen(4);
    //    }
  }

  //SCREEN 3 EVENTS
  else if (SimbleeForMobile.screen == 3) {
    if (event.id == catalogButton) {
      SimbleeForMobile.showScreen(2);
    }
    else if (event.id == nextPage) {
      updatePage = true;
      winePage++;
      if (winePage > 7) {
        winePage = 1;
      }
    }
    else if (event.id == previousPage) {
      updatePage = true;
      winePage--;
      if (winePage == 0) {
        winePage = 7;
      }
    }
    else if (event.id == wineImage1)
    {
      wineChosen = 1;
      inventory = true;
      SimbleeForMobile.showScreen(5);
      //removeBottle();
    }
    else if (event.id == wineImage2)
    {
      wineChosen = 2;
      inventory = true;
      SimbleeForMobile.showScreen(5);
      //removeBottle();
    }
    else if (event.id == wineImage3)
    {
      wineChosen = 3;
      inventory = true;
      SimbleeForMobile.showScreen(5);
      //removeBottle();
    }
    else if (event.id == wineImage4)
    {
      wineChosen = 4;
      inventory = true;
      SimbleeForMobile.showScreen(5);
      //removeBottle();
    }
  }

  else if (SimbleeForMobile.screen == 4)
  {
    if (event.id == backButton)
    {
      SimbleeForMobile.showScreen(2);
    }
    else if (event.id == cameraButton)
    {
      SimbleeForMobile.setVisible(text1, false);
      SimbleeForMobile.setVisible(text2, false);
      SimbleeForMobile.setVisible(text3, false);
      SimbleeForMobile.setVisible(wineLabel, true);
      SimbleeForMobile.setVisible(acceptButton, true);
    }
    else if (event.id == acceptButton)
    {
      SimbleeForMobile.showScreen(5);
      scanned = true;
    }
  }

  else if (SimbleeForMobile.screen == 5)
  {
    if (event.id == addButton) {
      //addedWine = WineName1[clickedOverlay+((winePage-1)*5)];
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
    else if (event. id == consumeButton) {
      consumeBottle();
      updatePage = true;
    }
    else if (event.id == buyButton) {
      buyBottle();
      updatePage = true;
    }
  }
}


void ui()
{
  switch (SimbleeForMobile.screen)
  {
    case 1:
      splashScreen();
      //delay(3000);
      SimbleeForMobile.showScreen(2);
      break;

    case 2:
      catalogScreen();
      //addScreen();
      break;

    case 3:
      myInventoryScreen();
      break;

    case 4:
      scanScreen();
      break;

    case 5:
      addScreen();
      break;

    default:
      splashScreen();
      break;
  }
}

void SimbleeForMobile_onDisconnect() {
  disconnected = true;
}

