#include <RBD_Button.h>
#include <RBD_Timer.h>

#include "FastLED.h"
#include <vector>

// How many leds in your strip?
#define NUM_LEDS 6
asm(".global _printf_float"); 

#define SW1 2
#define SW2 3
#define SW3 4
#define SW4 7
#define SW5 8
#define SW6 9

#define color CRGB::Purple

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 5

// Define the array of leds
CRGB leds[NUM_LEDS];

RBD::Button button1(SW1, true);
RBD::Button button2(SW2, true);
RBD::Button button3(SW3, true);
RBD::Button button4(SW4, true);
RBD::Button button5(SW5, true);
RBD::Button button6(SW6, true);

RBD::Timer button1Timer;
RBD::Timer button2Timer;
RBD::Timer button3Timer;
RBD::Timer button4Timer;
RBD::Timer button5Timer;
RBD::Timer button6Timer;

std::vector<RBD::Timer> timerList = {button1Timer, button2Timer, button3Timer, button4Timer, button5Timer, button6Timer};

void setup() { 
  	  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
      Serial.begin(9600);
      button1Timer.setTimeout(3000);
      button2Timer.setTimeout(3000);
      button3Timer.setTimeout(3000);
      button4Timer.setTimeout(3000);
      button5Timer.setTimeout(3000);
      button6Timer.setTimeout(3000);
}

void loop() { 
  // Turn the LED on, then pause

  if(button1.onPressed()) {
    Serial.println("Button1 Pressed");
    leds[3] = color;
    button1Timer.restart();
  } else if (button1Timer.onExpired()) {
    leds[3] = CRGB::Black;
  }
  
  if(button2.onPressed()) {
    Serial.println("Button2 Pressed");
    leds[4] = color;
    button2Timer.restart();
  } else if (button2Timer.onExpired()) {
    leds[4] = CRGB::Black;
  }
  
  if(button3.onPressed()) {
    Serial.println("Button3 Pressed");
    leds[5] = color;
    button3Timer.restart();
  } else if (button3Timer.onExpired()) {
    leds[5] = CRGB::Black;
  }
  
  if(button4.onPressed()) {
    Serial.println("Button4 Pressed");
    leds[2] = color;
    button4Timer.restart();
  } else if (button4Timer.onExpired()) {
    leds[2] = CRGB::Black;
  }
  
  if(button5.onPressed()) {
    Serial.println("Button5 Pressed");
    leds[1] = color;
    button5Timer.restart();
  } else if (button5Timer.onExpired()) {
    leds[1] = CRGB::Black;
  }
  
  if(button6.onPressed()) {
    Serial.println("Button6 Pressed");
    leds[0] = color;
    button6Timer.restart();
  } else if (button6Timer.onExpired()) {
    leds[0] = CRGB::Black;
  }  

  for (std::vector<RBD::Timer>::iterator it = timerList.begin(); it != timerList.end(); ++it) {
    if ((*it).onExpired()) {
      Serial.println("Test");
    }
  }

  int lumens = quadwave8(beat8(72));
  FastLED.setBrightness(lumens);
  FastLED.show();
}
