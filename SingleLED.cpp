#include "SingleLED.h"

LED::LED(): red(255), green(255), blue(255), brightness(DEFAULT_BRIGHTNESS), mask(false) {}

uint32_t LED::GetColorGRB(void)
{
    if(mask) {
        return 0; // Return 0 if masked
    }
    else {
        // Apply brightness scaling
        uint8_t scaled_red = (red * brightness) / 7;
        uint8_t scaled_green = (green * brightness) / 7;
        uint8_t scaled_blue = (blue * brightness) / 7;

        return ((uint32_t)scaled_green << 16) | ((uint32_t)scaled_red << 8) | ((uint32_t)scaled_blue << 0);
    }
}
    
void LED::SetColor(uint8_t r, uint8_t g, uint8_t b) 
{
    red = r;
    green = g;
    blue = b;
}

void LED::SetColor(uint32_t rgb)
{
    red = ((rgb >> 16) & 0xFF);
    green = ((rgb >> 8) & 0xFF);
    blue = ((rgb >> 0) & 0xFF);
}

void LED::SetMask(bool m)
{
    mask = m;
}

void LED::ToggleMask(void)
{
    mask = !mask;
}

void LED::SetBrightness(uint8_t b)
{
    brightness = (b > 7) ? 7 : b; // Ensure brightness is within [0, 7]
}

void LED::IncreaseBrightness()
{
    if(brightness < 7) {
        brightness += 1;
    }
}

void LED::DecreaseBrightness()
{
    if(brightness > 0) {
        brightness -= 1;
    }
}