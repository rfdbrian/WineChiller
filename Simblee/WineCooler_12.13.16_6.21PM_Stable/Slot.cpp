#include "Slot.h"

Slot::Slot(int buttonPin, CRGB& led) : 
  btn(buttonPin, true), slotLed(led) {
//    btn = Button(buttonPin, true);
};

      

void Slot::updateColor(uint32_t colorCode) {
  slotLed = colorCode;
}

bool Slot::isPressed() {
    btn.isPressed();
}

bool Slot::isReleased() {
    btn.isReleased();
}

bool Slot::onPressed() {
    btn.isPressed();
}

bool Slot::onReleased() {
    btn.isPressed();
}
