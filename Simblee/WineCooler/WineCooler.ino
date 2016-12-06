/*
   Copyright (c) 2015 RF Digital Corp. All Rights Reserved.

   The source code contained in this file and all intellectual property embodied in
   or covering the source code is the property of RF Digital Corp. or its licensors.
   Your right to use this source code and intellectual property is non-transferable,
   non-sub licensable, revocable, and subject to terms and conditions of the
   SIMBLEE SOFTWARE LICENSE AGREEMENT.
   http://www.simblee.com/licenses/SimbleeSoftwareLicenseAgreement.txt

   THE SOURCE CODE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND.

   This heading must NOT be removed from this file.
*/
#include <RBD_Button.h>
#include <RBD_Timer.h>
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
#include "WineSplashScreen_jpg.h"
#include "Wine2_png.h"
#include "camera_png.h"
#include "wineLabel_png.h"
#include <SimbleeForMobile.h>

#define GE_RED rgb(158,11,15)
#define GE_YELLOW rgb(243,165,25)
#define GE_GRAY rgb(159,159,159)
#define GE_DARKGRAY rgb(123,123,123)
#define GE_LIGHTGRAY rgb(194,193,193)
#define GE_BEIGE rgb(216,208,200)

uint8_t currentButtonState[25],
        nextButtonState[25],
        wineStored[25];
bool initialized = false,
     updatePage = false;



#define DEMOUNIT


//APP STATUS
//0 = default
//1 = insert wine
uint8_t APP_STATUS = 0;

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
        removeScreen1, removeScreen2, removeScreen3, removeScreen4, removeScreen5, removeScreen6, removeScreen7;


uint8_t wineRect[25];

uint8_t rect[25];

uint8_t winePage = 1,
        addedWine = 0,
        switchDetected = 0,
        wineChosen = 0;

#ifdef TESTBOX

RBD::Button button0(0, true);
RBD::Button button1(30, true);
RBD::Button button2(2, true);
RBD::Button button3(3, true);
RBD::Button button4(4, true);
RBD::Button button6(6, true);
RBD::Button button7(7, true);
RBD::Button button8(8, true);
RBD::Button button9(9, true);
RBD::Button button10(10, true);
RBD::Button button11(11, true);
RBD::Button button12(12, true);
RBD::Button button13(13, true);
RBD::Button button14(14, true);
RBD::Button button15(15, true);
RBD::Button button16(16, true);
RBD::Button button17(17, true);
RBD::Button button18(18, true);
RBD::Button button19(19, true);
RBD::Button button20(20, true);
RBD::Button button21(21, true);
RBD::Button button22(22, true);
RBD::Button button23(23, true);
RBD::Button button24(24, true);
RBD::Button button25(25, true);

#elif DEMOUNIT

RBD::Button button0(2, true);
RBD::Button button1(3, true);
RBD::Button button2(4, true);
RBD::Button button3(6, true);
RBD::Button button4(7, true);

RBD::Button button6(8, true);
RBD::Button button7(9, true);
RBD::Button button8(10, true);
RBD::Button button9(11, true);
RBD::Button button10(12, true);

RBD::Button button11(13, true);
RBD::Button button12(14, true);
RBD::Button button13(15, true);
RBD::Button button14(16, true);
RBD::Button button15(17, true);

RBD::Button button16(18, true);
RBD::Button button17(19, true);
RBD::Button button18(20, true);
RBD::Button button19(21, true);
RBD::Button button20(22, true);

RBD::Button button21(23, true);
RBD::Button button22(24, true);
RBD::Button button23(25, true);
RBD::Button button24(28, true);
RBD::Button button25(29, true);

#endif


#define LED_PIN 5
#define NUM_LEDS 25
CRGB leds[NUM_LEDS];

#define color CRGB::Red
#define color2 CRGB::Green

//SCREEN 1 BASE
void splashScreen()
{
  SimbleeForMobile.beginScreen(WHITE, PORTRAIT);
  SimbleeForMobile.drawRect(0, 0, 320, 570, GE_RED);
  SimbleeForMobile.drawText(20, 200, "LOADING", WHITE, 50);
  SimbleeForMobile.drawText(20, 250, "INTERFACE", WHITE, 50);
  SimbleeForMobile.imageSource(1,JPG, WineSplashScreen_jpg, WineSplashScreen_jpg_len);
  SimbleeForMobile.drawImage(1, 0, 0);
  SimbleeForMobile.endScreen();
}

//SCREEN 2 BASE
void catalogScreen()
{
  SimbleeForMobile.beginScreen(WHITE, PORTRAIT);

  //BACKGROUND
  SimbleeForMobile.drawRect(0, 0, 320, 570, GE_RED);
  SimbleeForMobile.drawRect(0, 100, 320, 400, WHITE);
  SimbleeForMobile.drawRect(159, 100, 2, 400, GRAY);
  SimbleeForMobile.drawRect(0, 299, 320, 2, GRAY);

  //TITLE
  SimbleeForMobile.drawText(90, 25, "Wine Catalog", WHITE, 25);

  //SEGMENT MENU
  char *titles[] = { "Purchased", "Wish List", "Favorites" };
  SimbleeForMobile.drawSegment(10, 60, 300, titles, 3, GE_YELLOW);

  //WINE IMAGES
  SimbleeForMobile.imageSource(4, PNG, Wine2_png, Wine2_png_len);

  //WINE BOX 1
  wineImage1 = SimbleeForMobile.drawImage(4, 55, 105);
  wineText1 = SimbleeForMobile.drawText(10, 220, "Wine 1", GE_RED, 11);

  //WINE BOX 3
  wineImage3 = SimbleeForMobile.drawImage(4, 55, 305);
  wineText3 = SimbleeForMobile.drawText(170, 220, "Wine 2", GE_RED, 11);

  //WINE BOX 2
  wineImage2 = SimbleeForMobile.drawImage(4, 215, 105);
  wineText2 = SimbleeForMobile.drawText(10, 420, "Wine 3", GE_RED, 11);

  //WINE BOX 4
  wineImage4 = SimbleeForMobile.drawImage(4, 215, 305);
  wineText4 = SimbleeForMobile.drawText(170, 420, "Wine 4", GE_RED, 11);




  addButton1 = SimbleeForMobile.drawButton(100, 230, 80, "ADD", GE_YELLOW, 1);
  SimbleeForMobile.setEvents(addButton1, EVENT_PRESS);
  addButton2 = SimbleeForMobile.drawButton(260, 230, 80, "ADD", GE_YELLOW, 1);
  SimbleeForMobile.setEvents(addButton2, EVENT_PRESS);
  addButton3 = SimbleeForMobile.drawButton(100, 430, 80, "ADD", GE_YELLOW, 1);
  SimbleeForMobile.setEvents(addButton3, EVENT_PRESS);
  addButton4 = SimbleeForMobile.drawButton(260, 430, 80, "ADD", GE_YELLOW, 1);
  SimbleeForMobile.setEvents(addButton4, EVENT_PRESS);

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

  //PAGE SELECTION
  SimbleeForMobile.drawText(134 , 497, "Page", WHITE, 16);
  pageValue = SimbleeForMobile.drawText(174, 497, 1, WHITE, 16);
  previousPage = SimbleeForMobile.drawButton(2, 492, 20, "<<", WHITE, 1);
  SimbleeForMobile.setEvents(previousPage, EVENT_PRESS);
  nextPage = SimbleeForMobile.drawButton(300, 492, 20, ">>", WHITE, 1);
  SimbleeForMobile.setEvents(nextPage, EVENT_PRESS);

  //BOTTOM MENU
  inventoryButton = SimbleeForMobile.drawButton(30, 525, 100, "Inventory", GE_YELLOW);
  SimbleeForMobile.setEvents(inventoryButton, EVENT_PRESS);
  scanButton = SimbleeForMobile.drawButton(190, 525, 100, "Scan", GE_YELLOW);
  SimbleeForMobile.setEvents(scanButton, EVENT_PRESS);

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
  SimbleeForMobile.imageSource(4, PNG, Wine2_png, Wine2_png_len);
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
  SimbleeForMobile.drawRect(0, 0, 320, 570, GE_GRAY);
  SimbleeForMobile.drawRect(30, 90, 260, 371, GE_DARKGRAY);
  SimbleeForMobile.drawRect(0, 490, 320, 80, GE_RED);

  //WINE
  SimbleeForMobile.imageSource(7, PNG, wineLabel_png, wineLabel_png_len);
  wineLabel = SimbleeForMobile.drawImage(7, 30, 90);
  SimbleeForMobile.setVisible(wineLabel, false);

  //Label
  text1 = SimbleeForMobile.drawText(118, 190, "POSITION THE", WHITE, 14);
  text2 = SimbleeForMobile.drawText(90, 200, "WINE LABEL", WHITE, 28);
  text3 = SimbleeForMobile.drawText(100, 230, "WITHIN THIS FRAME", WHITE, 14);


  //BOTTOM MENU
  backButton = SimbleeForMobile.drawButton(40, 520, 40, "BACK", WHITE, 1);
  SimbleeForMobile.setEvents(backButton, EVENT_PRESS);
  SimbleeForMobile.imageSource(5, PNG, camera_png, camera_png_len);
  cameraButton = SimbleeForMobile.drawImage(5, 125, 495);
  SimbleeForMobile.setEvents(cameraButton, EVENT_PRESS);
  acceptButton = SimbleeForMobile.drawButton(220, 520, 60, "ACCEPT", WHITE, 1);
  SimbleeForMobile.setEvents(acceptButton, EVENT_PRESS);
  SimbleeForMobile.setVisible(acceptButton, false);

  SimbleeForMobile.endScreen();
}


//SCREEN 5 BASE
void addScreen()
{
  SimbleeForMobile.beginScreen();

  //BACKGROUND
  SimbleeForMobile.drawRect(0, 0, 320, 80, GE_RED);
  SimbleeForMobile.drawRect(0, 80, 320, 180, GE_BEIGE);
  SimbleeForMobile.drawRect(90, 90, 140, 160, WHITE);

  //TITLE
  SimbleeForMobile.drawText(45, 30, "Pinot Noir VQA by Konzelmann", WHITE, 18);

  //WINE IMAGE
  SimbleeForMobile.imageSource(4, PNG, Wine2_png, Wine2_png_len);
  SimbleeForMobile.drawImage(4, 135, 100);

  //INFO
  addButton = SimbleeForMobile.drawButton(60, 270, 200, "ADD TO INVENTORY", BLACK, 1);
  SimbleeForMobile.setEvents(addButton, EVENT_PRESS);

  //SHOWWINESCREEN
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

  SimbleeForMobile.endScreen();
}

void checkButtonState(RBD::Button & toButton, CRGB & someLED, int cellID) 
{
  if (toButton.isPressed()) {
    if (initialized == false) {
      currentButtonState[cellID] = true;
      someLED = color;
    }
    else
      nextButtonState[cellID] = true;
      someLED = color;
  }
  else if (toButton.isReleased()) {
    if (initialized == false) {
      currentButtonState[cellID] = false;
      

    }
    else
      nextButtonState[cellID] = false;
  }
}

void checkAllButtons() 
{

#ifdef TESTBOX

  checkButtonState(button0, leds[0], 0);
  checkButtonState(button1, leds[1], 1);
  checkButtonState(button2, leds[2], 2);
  checkButtonState(button3, leds[3], 3);
  checkButtonState(button4, leds[4], 4);
  checkButtonState(button6, leds[5], 5);
  checkButtonState(button7, leds[6], 6);
  checkButtonState(button8, leds[7], 7);
  checkButtonState(button9, leds[8], 8);
  checkButtonState(button10, leds[9], 9);
  checkButtonState(button11, leds[10], 10);
  checkButtonState(button12, leds[11], 11);
  checkButtonState(button13, leds[12], 12);
  checkButtonState(button14, leds[13], 13);
  checkButtonState(button15, leds[14], 14);
  checkButtonState(button16, leds[15], 15);
  checkButtonState(button17, leds[16], 16);
  checkButtonState(button18, leds[17], 17);
  checkButtonState(button19, leds[18], 18);
  checkButtonState(button20, leds[19], 19);
  checkButtonState(button21, leds[20], 20);
  checkButtonState(button22, leds[21], 21);
  checkButtonState(button23, leds[22], 22);
  checkButtonState(button24, leds[23], 23);
  checkButtonState(button25, leds[24], 24);

#elif DEMOUNIT

  checkButtonState(button0, leds[0], 4);
  checkButtonState(button1, leds[1], 3);
  checkButtonState(button2, leds[2], 2);
  checkButtonState(button3, leds[3], 1);
  checkButtonState(button4, leds[4], 0);

  checkButtonState(button6, leds[9], 5);
  checkButtonState(button7, leds[8], 6);
  checkButtonState(button8, leds[7], 7);
  checkButtonState(button9, leds[6], 8);
  checkButtonState(button10, leds[5], 9);

  checkButtonState(button11, leds[14], 10);
  checkButtonState(button12, leds[13], 11);
  checkButtonState(button13, leds[12], 12);
  checkButtonState(button14, leds[11], 13);
  checkButtonState(button15, leds[10], 14);

  checkButtonState(button16, leds[19], 15);
  checkButtonState(button17, leds[18], 16);
  checkButtonState(button18, leds[17], 17);
  checkButtonState(button19, leds[16], 18);
  checkButtonState(button20, leds[15], 19);

  checkButtonState(button21, leds[24], 20);
  checkButtonState(button22, leds[23], 21);
  checkButtonState(button23, leds[22], 22);
  checkButtonState(button24, leds[21], 23);
  checkButtonState(button25, leds[20], 24);

#endif

}

void compareStates() 
{
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

void showWineInsertScreen() 
{
  SimbleeForMobile.setVisible(insertScreen1, true);
  SimbleeForMobile.setVisible(insertScreen2, true);
  SimbleeForMobile.setVisible(insertScreen3, true);
  checkAllButtons();
  FastLED.show();
  delay(100);
  initialized = true;
  compareStates();
}

void removeBottle()
{
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
  #if DEMOUNIT

  //  Positions on demo unit are reversed. Therefore LED index for corresponding
  //    switch is the ceiling of SWH_POS in multiple of 4 - remainder of SWH_POS / 4
  int remainder = ledPosition % 4;
  int switchPosCeiling = ledPosition + 4 - remainder;
  int ledPosition = switchPosCeiling - remainder;

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

void setup()
{
  //Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
  SimbleeForMobile.deviceName = "Wine";
  SimbleeForMobile.advertisementData = "Cooler";
  SimbleeForMobile.domain = "FirstBuild.simblee.com";
  SimbleeForMobile.begin();
}

void loop()
{
  if (disconnected == true)
  {
    SimbleeForMobile.showScreen(2);
    disconnected = false;
  }


  if (SimbleeForMobile.updatable)
  {
    //SCREEN 1 UPDATES
    if (SimbleeForMobile.screen == 1)
    {
    }
    //SCREEN 2 UPDATES
    else if (SimbleeForMobile.screen == 2)
    {
      if (switchDetected > 0)
      {
        SimbleeForMobile.updateValue(insertScreen5, addedWine);
        SimbleeForMobile.updateValue(insertScreen4, switchDetected + 1);
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
        switchDetected = 0;
      }
      if (updatePage = true)
      {
        SimbleeForMobile.updateValue(pageValue, winePage);
        switch (winePage)
        {
          case 1:
            SimbleeForMobile.setVisible(wineImage1, true);
            SimbleeForMobile.setVisible(wineImage2, true);
            SimbleeForMobile.setVisible(wineImage3, true);
            SimbleeForMobile.setVisible(wineImage4, true);
            SimbleeForMobile.updateText(wineText1, "Wine 1");
            SimbleeForMobile.updateText(wineText2, "Wine 2");
            SimbleeForMobile.updateText(wineText3, "Wine 3");
            SimbleeForMobile.updateText(wineText4, "Wine 4");
            break;
          case 2:
            SimbleeForMobile.setVisible(wineImage1, true);
            SimbleeForMobile.setVisible(wineImage2, true);
            SimbleeForMobile.setVisible(wineImage3, true);
            SimbleeForMobile.setVisible(wineImage4, true);
            SimbleeForMobile.updateText(wineText1, "Wine 5");
            SimbleeForMobile.updateText(wineText2, "Wine 6");
            SimbleeForMobile.updateText(wineText3, "Wine 7");
            SimbleeForMobile.updateText(wineText4, "Wine 8");
            break;
          case 3:
            SimbleeForMobile.setVisible(wineImage1, true);
            SimbleeForMobile.setVisible(wineImage2, true);
            SimbleeForMobile.setVisible(wineImage3, true);
            SimbleeForMobile.setVisible(wineImage4, true);
            SimbleeForMobile.updateText(wineText1, "Wine 9");
            SimbleeForMobile.updateText(wineText2, "Wine 10");
            SimbleeForMobile.updateText(wineText3, "Wine 11");
            SimbleeForMobile.updateText(wineText4, "Wine 12");
            break;
          case 4:
            SimbleeForMobile.setVisible(wineImage1, true);
            SimbleeForMobile.setVisible(wineImage2, true);
            SimbleeForMobile.setVisible(wineImage3, true);
            SimbleeForMobile.setVisible(wineImage4, true);
            SimbleeForMobile.updateText(wineText1, "Wine 13");
            SimbleeForMobile.updateText(wineText2, "Wine 14");
            SimbleeForMobile.updateText(wineText3, "Wine 15");
            SimbleeForMobile.updateText(wineText4, "Wine 16");
            break;
          case 5:
            SimbleeForMobile.setVisible(wineImage1, true);
            SimbleeForMobile.setVisible(wineImage2, true);
            SimbleeForMobile.setVisible(wineImage3, true);
            SimbleeForMobile.setVisible(wineImage4, true);
            SimbleeForMobile.updateText(wineText1, "Wine 17");
            SimbleeForMobile.updateText(wineText2, "Wine 18");
            SimbleeForMobile.updateText(wineText3, "Wine 19");
            SimbleeForMobile.updateText(wineText4, "Wine 20");
            break;
          case 6:
            SimbleeForMobile.setVisible(wineImage1, true);
            SimbleeForMobile.setVisible(wineImage2, true);
            SimbleeForMobile.setVisible(wineImage3, true);
            SimbleeForMobile.setVisible(wineImage4, true);
            SimbleeForMobile.updateText(wineText1, "Wine 21");
            SimbleeForMobile.updateText(wineText2, "Wine 22");
            SimbleeForMobile.updateText(wineText3, "Wine 23");
            SimbleeForMobile.updateText(wineText4, "Wine 24");
            break;
          case 7:
            SimbleeForMobile.updateText(wineText1, "Wine 25");
            SimbleeForMobile.updateText(wineText2, "");
            SimbleeForMobile.updateText(wineText3, "");
            SimbleeForMobile.updateText(wineText4, "");
            SimbleeForMobile.setVisible(wineImage2, false);
            SimbleeForMobile.setVisible(wineImage3, false);
            SimbleeForMobile.setVisible(wineImage4, false);
            break;
        }
        updatePage = false;
      }
    }
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
            if (nextButtonState[0] == 1)
            {
              SimbleeForMobile.setVisible(wineImage1, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage1, false);
            }
            if (nextButtonState[1] == 1) {
              SimbleeForMobile.setVisible(wineImage2, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage2, false);
            }
            if (nextButtonState[2] == 1) {
              SimbleeForMobile.setVisible(wineImage3, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage3, false);
            }
            if (nextButtonState[3] == 1) {
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
            if (nextButtonState[4] == 1)
            {
              SimbleeForMobile.setVisible(wineImage1, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage1, false);
            }
            if (nextButtonState[5] == 1) {
              SimbleeForMobile.setVisible(wineImage2, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage2, false);
            }
            if (nextButtonState[6] == 1) {
              SimbleeForMobile.setVisible(wineImage3, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage3, false);
            }
            if (nextButtonState[7] == 1) {
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
            if (nextButtonState[8] == 1)
            {
              SimbleeForMobile.setVisible(wineImage1, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage1, false);
            }
            if (nextButtonState[9] == 1) {
              SimbleeForMobile.setVisible(wineImage2, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage2, false);
            }
            if (nextButtonState[10] == 1) {
              SimbleeForMobile.setVisible(wineImage3, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage3, false);
            }
            if (nextButtonState[11] == 1) {
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
            if (nextButtonState[12] == 1)
            {
              SimbleeForMobile.setVisible(wineImage1, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage1, false);
            }
            if (nextButtonState[13] == 1) {
              SimbleeForMobile.setVisible(wineImage2, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage2, false);
            }
            if (nextButtonState[14] == 1) {
              SimbleeForMobile.setVisible(wineImage3, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage3, false);
            }
            if (nextButtonState[15] == 1) {
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
            if (nextButtonState[16] == 1)
            {
              SimbleeForMobile.setVisible(wineImage1, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage1, false);
            }
            if (nextButtonState[17] == 1) {
              SimbleeForMobile.setVisible(wineImage2, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage2, false);
            }
            if (nextButtonState[18] == 1) {
              SimbleeForMobile.setVisible(wineImage3, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage3, false);
            }
            if (nextButtonState[19] == 1) {
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
            if (nextButtonState[20] == 1)
            {
              SimbleeForMobile.setVisible(wineImage1, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage1, false);
            }
            if (nextButtonState[21] == 1) {
              SimbleeForMobile.setVisible(wineImage2, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage2, false);
            }
            if (nextButtonState[22] == 1) {
              SimbleeForMobile.setVisible(wineImage3, true);
            }
            else {
              SimbleeForMobile.setVisible(wineImage3, false);
            }
            if (nextButtonState[23] == 1) {
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
            if (nextButtonState[24] == 1)
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
    else if (SimbleeForMobile.screen == 5)
    {
      if (switchDetected > 0)
      {
        SimbleeForMobile.updateValue(insertScreen5, addedWine);
        SimbleeForMobile.updateValue(insertScreen4, switchDetected + 1);
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
        switchDetected = 0;
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
      if (winePage > 7)
      {
        winePage = 1;
      }
    }
    else if (event.id == previousPage)
    {
      updatePage = true;
      winePage--;
      if (winePage == 0) {
        winePage = 7;
      }
    }
    else if (event.id == addButton1)
    {
      addedWine = (winePage * 4) - 3;
      showWineInsertScreen();
    }
    else if (event.id == addButton2)
    {
      addedWine = (winePage * 4) - 2;
      showWineInsertScreen();
    }
    else if (event.id == addButton3)
    {
      addedWine = (winePage * 4) - 1;
      showWineInsertScreen();
    }
    else if (event.id == addButton4)
    {
      addedWine = (winePage * 4) - 0;
      showWineInsertScreen();
    }
    else if (event.id == inventoryButton)
    {
      SimbleeForMobile.showScreen(3);
    }
    else if (event.id == scanButton)
    {
      SimbleeForMobile.showScreen(4);
    }
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
      //SimbleeForMobile.showScreen(5);
      removeBottle();
    }
    else if (event.id == wineImage2)
    {
      wineChosen = 2;
      //SimbleeForMobile.showScreen(5);
      removeBottle();
    }
    else if (event.id == wineImage3)
    {
      wineChosen = 3;
      //SimbleeForMobile.showScreen(5);
      removeBottle();
    }
    else if (event.id == wineImage4)
    {
      wineChosen = 4;
      //SimbleeForMobile.showScreen(5);
      removeBottle();
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
    }
  }

  else if (SimbleeForMobile.screen == 5)
  {
    if (event.id == addButton) {
      addedWine = 12;
      showWineInsertScreen();
    }
  }
}

void ui()
{
  switch (SimbleeForMobile.screen)
  {
    case 1:
      splashScreen();
      delay(3000);
      SimbleeForMobile.showScreen(2);
      break;

    case 2:
      catalogScreen();
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

void SimbleeForMobile_onDisconnect() 
{
  disconnected = true;
}

