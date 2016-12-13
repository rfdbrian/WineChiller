#include <SimbleeForMobile.h>
#include "camera_png.h"
#include "winePhoto_jpg.h"
#include "CoppolaCab2_jpg.h"


#define GE_RED rgb(158,11,15)
#define GE_GRAY rgb(159,159,159)
#define GE_DARKGRAY rgb(123,123,123)
#define GE_LIGHTGRAY rgb(194,193,193)
#define wineLabelImage 1
#define cameraImage 2
#define coppolaCabImage 3


uint8_t wineLabel, text1, text2, text3, backButton, cameraButton, acceptButton,
        addScreen1,addScreen2,addScreen3,addScreen4,addScreen5,addScreen6,addScreen7,addScreen8,addScreen9,addScreen10,addScreen11;

void setup() {
  SimbleeForMobile.deviceName = "Scan Wine";
  SimbleeForMobile.begin();
}

void loop() {
  SimbleeForMobile.process();
}

void ui(){
  SimbleeForMobile.beginScreen();
  SimbleeForMobile.drawRect(0, 0, 320, 480, GE_RED);
   SimbleeForMobile.imageSource(cameraImage, PNG, camera_png, camera_png_len);
  cameraButton = SimbleeForMobile.drawImage(cameraImage, 125, 415);
  SimbleeForMobile.setEvents(cameraButton, EVENT_PRESS);
  SimbleeForMobile.drawRect(0, 60, 320,360, GE_LIGHTGRAY);
  SimbleeForMobile.drawRect(30,90, 260, 300,GE_DARKGRAY);
  SimbleeForMobile.drawRect(0,480,320,90,WHITE);

  //WINE
  SimbleeForMobile.imageSource(wineLabelImage, JPG, winePhoto_jpg, winePhoto_jpg_len);
  wineLabel = SimbleeForMobile.drawImage(wineLabelImage, 30, 90);
  SimbleeForMobile.setVisible(wineLabel, false);

  //Label
  text1 = SimbleeForMobile.drawText(118, 190, "POSITION THE", WHITE, 14);
  text2 = SimbleeForMobile.drawText(90, 200, "WINE LABEL", WHITE, 28);
  text3 = SimbleeForMobile.drawText(100, 230, "WITHIN THIS FRAME", WHITE, 14);


  //BOTTOM MENU
  backButton = SimbleeForMobile.drawButton(40, 390, 40, "BACK", WHITE, 1);
  SimbleeForMobile.setEvents(backButton, EVENT_PRESS);
 
  acceptButton = SimbleeForMobile.drawButton(220, 390, 60, "ACCEPT", WHITE, 1);
  SimbleeForMobile.setEvents(acceptButton, EVENT_PRESS);
  SimbleeForMobile.setVisible(acceptButton, false);


  addScreen1 = SimbleeForMobile.drawRect(0, 0, 320, 480, GE_RED);
  addScreen2 = SimbleeForMobile.imageSource(coppolaCabImage,JPG,CoppolaCab2_jpg,CoppolaCab2_jpg_len);
  addScreen3 = SimbleeForMobile.drawImage(coppolaCabImage,0,60);
  addScreen4 = SimbleeForMobile.drawRect(0,240,320,180,WHITE);
  addScreen5 = SimbleeForMobile.drawText(30,245, "Diamond Collection Ivory", GE_LIGHTGRAY,16);
  addScreen6 = SimbleeForMobile.drawText(30,262,"Francis Ford Coppola", BLACK, 18);
  addScreen7 = SimbleeForMobile.drawText(30,280,"2013", BLACK, 18);
  addScreen8 =  SimbleeForMobile.drawText(30,300,"Cabernet Sauvignon", GE_LIGHTGRAY,16);
  addScreen9 = SimbleeForMobile.drawText(200,300,"$14.98",GE_LIGHTGRAY,16);
  addScreen10 = SimbleeForMobile.drawButton(80,380,160,"ADD TO INVENTORY",BLACK,1);
  addScreen11 =  SimbleeForMobile.drawText(80,385,"ADDED TO INVENTORY",GREEN);
  SimbleeForMobile.setVisible(addScreen1,false);
   SimbleeForMobile.setVisible(addScreen2,false);
   SimbleeForMobile.setVisible(addScreen3,false);
   SimbleeForMobile.setVisible(addScreen4,false);
   SimbleeForMobile.setVisible(addScreen5,false);
   SimbleeForMobile.setVisible(addScreen6,false);
   SimbleeForMobile.setVisible(addScreen7,false);
   SimbleeForMobile.setVisible(addScreen8,false);
   SimbleeForMobile.setVisible(addScreen9,false);
   SimbleeForMobile.setVisible(addScreen10,false);
   SimbleeForMobile.setVisible(addScreen11,false);
  
  SimbleeForMobile.endScreen();
  
}

void ui_event(event_t &event){
  if(event.id == cameraButton){
    SimbleeForMobile.setVisible(text1, false);
    SimbleeForMobile.setVisible(text2, false);
    SimbleeForMobile.setVisible(text3, false);
    SimbleeForMobile.setVisible(wineLabel, true);
    SimbleeForMobile.setVisible(acceptButton,true);
  }
  else if(event.id == acceptButton) {
   SimbleeForMobile.setVisible(addScreen1,true);
   SimbleeForMobile.setVisible(addScreen2,true);
   SimbleeForMobile.setVisible(addScreen3,true);
   SimbleeForMobile.setVisible(addScreen4,true);
   SimbleeForMobile.setVisible(addScreen5,true);
   SimbleeForMobile.setVisible(addScreen6,true);
   SimbleeForMobile.setVisible(addScreen7,true);
   SimbleeForMobile.setVisible(addScreen8,true);
   SimbleeForMobile.setVisible(addScreen9,true);
   SimbleeForMobile.setVisible(addScreen10,true);
   //SimbleeForMobile.setVisible(addScreen11,true);
   
  }
  else if(event.id == addScreen10){
    SimbleeForMobile.setVisible(addScreen10,false);
    SimbleeForMobile.setVisible(addScreen11,true);
  }
}



