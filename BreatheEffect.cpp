#include "BreatheEffect.h"

BreatheEffect::BreatheEffect(LEDStrip* strip, uint32_t start_time_ms, uint32_t duration_ms, uint32_t color, uint8_t min_brightness, uint8_t max_brightness)
    : Effect(strip, start_time_ms, duration_ms),
      color(color),
      min_brightness(min_brightness),
      max_brightness(max_brightness),
      current_brightness(min_brightness),
      last_brightness(min_brightness),
      increasing(true),
      last_update_time(start_time_ms)
{
    led_strip->SetAll(color); // Initialize all LEDs to the specified color
    led_strip->SetBrightness(current_brightness); // Set initial brightness
}

bool BreatheEffect::Update(uint32_t current_time)
{
    if ((current_time < start_time_ms) || !led_strip) {
        return false; // Effect not started or strip not available
    }

    uint32_t elapsed_time = current_time - last_update_time;
    if (elapsed_time == 0) {
        return false; // No time has passed since the last update
    }

    int32_t delta = (max_brightness - min_brightness) * elapsed_time / (duration_ms / 2);
    if (increasing) {
        current_brightness += delta;
        if (current_brightness >= max_brightness) {
            current_brightness = max_brightness;
            increasing = false;
        }
    }
    else {
        current_brightness -= delta;
        if (current_brightness <= min_brightness) {
            current_brightness = min_brightness;
            increasing = true;
        }
    }

    if (current_brightness == last_brightness) {
        return false;
    }

    led_strip->SetBrightness(current_brightness);
    last_brightness = current_brightness;
    last_update_time = current_time;
    return true;
}