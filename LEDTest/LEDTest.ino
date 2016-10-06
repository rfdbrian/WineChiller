#include <RBD_Button.h>

#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 6

#define SW1 2
#define SW2 3
#define SW3 4
#define SW4 9
#define SW5 8
#define SW6 7

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 6

// Define the array of leds
CRGB leds[NUM_LEDS];

RBD::Button button1(SW1, true);
RBD::Button button2(SW2, true);
RBD::Button button3(SW3, true);
RBD::Button button4(SW4, true);
RBD::Button button5(SW5, true);
RBD::Button button6(SW6, true);

unsigned long button1Timer;
unsigned long button2Timer;
unsigned long button3Timer;
unsigned long button4Timer;
unsigned long button5Timer;
unsigned long button6Timer;

vector<unsigned long> timerList = {button1Timer, button2Timer, button3Timer, button4Timer, button5Timer, button6Timer};

void setup() { 
  	  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
      Serial.begin(9600);
//    pinMode(SW6, INPUT);
}

void loop() { 
  // Turn the LED on, then pause
  

  if(button1.onPressed()) {
    Serial.println("Button1 Pressed");
    leds[0] = CRGB::PURPLE;
    button1Timer = millis();
  }
  if(button2.onPressed()) {
    Serial.println("Button2 Pressed");
    leds[1] = CRGB::PURPLE;
    button2Timer = millis();
  }
  if(button3.onPressed()) {
    Serial.println("Button3 Pressed");
    leds[2] = CRGB::PURPLE;
    button3Timer = millis();
  }
  if(button4.onPressed()) {
    Serial.println("Button4 Pressed");
    leds[3] = CRGB::PURPLE;
    button4Timer = millis();
  }
  if(button5.onPressed()) {
    Serial.println("Button5 Pressed");
    leds[4] = CRGB::PURPLE;
    button5Timer = millis();
  }
  if(button6.onPressed()) {
    Serial.println("Button6 Pressed");
    leds[5] = CRGB::PURPLE;
    button6Timer = millis();
  }

  for (vector<unsigned long>::iterator it = timerList.begin(); it != timerList.end(); ++it) {
    if ((millis() - *it) > 15000) {
      leds[it - timerList.begin()] = CRGB::BLACK;
    }
  }

  int lumens = scale16(beat16(92), 250 );
  FastLED.setBrightness(lumens);
  FastLED.show();
  
  delay(20);
}
