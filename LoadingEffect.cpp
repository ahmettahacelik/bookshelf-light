#include "LoadingEffect.h"

#include <cstdio>

LoadingEffect::LoadingEffect(LEDStrip* strip, uint32_t start_time_ms, uint32_t duration_ms, uint32_t color1, uint32_t color2)
    : Effect(strip, start_time_ms, duration_ms),
      color1(color1),
      color2(color2),
      current_color2_index(NUM_LED - 1),
      min_color2_index(0),
      last_update_time(start_time_ms)
{
    led_strip->SetAll(color1); // Initialize all LEDs to the first color
    led_strip->SetBrightness(DEFAULT_BRIGHTNESS); // Set default brightness
    led_strip->Update(); // Update the LED strip to apply the initial color

    // Calculate the total number of updates needed based on the LED strip length
    int32_t num_leds = led_strip->Length();
    total_update_count = num_leds * (num_leds + 1) / 2; // Total number of updates needed

    delta_update_time = duration_ms / total_update_count; // Time between updates in milliseconds
}

bool LoadingEffect::Update(uint32_t current_time)
{
    if ((current_time < start_time_ms) || !led_strip) {
        return false; // Effect not started or strip not available
    }

    current_time_ms = current_time;

    uint32_t elapsed_time = current_time_ms - last_update_time;

    printf("%d %d %d\n", current_time_ms, elapsed_time, delta_update_time);

    if(elapsed_time >= delta_update_time) {
        if(current_color2_index == min_color2_index) {
            min_color2_index = (min_color2_index + 1) % led_strip->Length(); // Move the min index forward
            current_color2_index = led_strip->Length() - 1; // Start from the end
        }
        else {
            led_strip->SetPixel(current_color2_index, color1); // Set the previous pixel to the first color
            current_color2_index = (current_color2_index - 1) % led_strip->Length(); // Increment index
        }

        led_strip->SetPixel(current_color2_index, color2); // Set the current pixel to the second color

        last_update_time = current_time_ms; // Update last update time

        return true; // Effect has been updated
    }

    return false; // No update needed
}