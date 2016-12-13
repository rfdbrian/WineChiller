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

#include <RBD_Timer.h>

#include <RBD_Button.h>



#define LED_PIN 5
#define NUM_LEDS 25
#define color CRGB::Purple

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

CRGB leds[NUM_LEDS];

void checkButtonState(RBD::Button& toButton, CRGB& someLED, int cellID) {
  if (toButton.onPressed()) {
    Serial.print("Button ");
    Serial.print(cellID);
    Serial.println("Pressed");
    someLED = color;
  } else if (toButton.onReleased()) {
    someLED = CRGB::Black;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
}

void loop() {
  // put your main code here, to run repeatedly:
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
  delay(10);
  FastLED.show();
}
