#ifndef SLOT_H
#define SLOT_H

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


class Slot {
	private:
        RBD::Button btn;
        CRGB& slotLed;

	public:
    Slot();
    Slot(int buttonPin, CRGB& led); 

    void updateColor(uint32_t colorCode);

    Slot& operator=(const Slot& b);
    bool isPressed();
    bool isReleased();
    bool onPressed();
    bool onReleased();
    
};
#endif
