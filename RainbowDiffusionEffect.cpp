#include "RainbowDiffusionEffect.h"

#include "RainbowGradient.h"
#include <pico/rand.h>

/// @todo no need for circular update, the strip is linear not circular
/// @todo pixel0 and pixel359 are not neighbors, they are not connected

RainbowDiffusionEffect::RainbowDiffusionEffect(LEDStrip* strip, uint32_t start_time_ms, uint32_t duration_ms,
                                                   float mutation_rate, uint16_t min_mutation_step, uint16_t max_mutation_step,
                                                   uint32_t mutation_interval, uint16_t min_diffusion_diff)
    : Effect(strip, start_time_ms, duration_ms),
      mutation_rate(mutation_rate),
      min_mutation_step(min_mutation_step),
      max_mutation_step(max_mutation_step),
      last_mutation_time(start_time_ms),
      mutation_interval(mutation_interval),
      min_diffusion_diff(min_diffusion_diff)
{
    for(int i = 0; i < led_strip->Length(); i++) {
        led_strip->SetPixel(i, Rainbow360[0]); // Initialize all LEDs with Rainbow color 0
        rainbow_strip[i] = 0; // Initialize the rainbow strip array
        updated_rainbow_strip[i] = 0; // Initialize the updated rainbow strip array
    }
    led_strip->SetBrightness(200); // Set a reasonable brightness level
    led_strip->Update(); // Update the LED strip to apply the initial state
}

bool RainbowDiffusionEffect::Update(uint32_t current_time)
{
    if ((current_time < start_time_ms) || !led_strip) {
        return false; // Effect not started or strip not available
    }

    uint32_t elapsed_time = current_time - last_mutation_time;
    if (elapsed_time == 0) {
        return false; // No time has passed since the last update
    }

    bool return_value = false;

    uint32_t mutation_possibility = get_rand_32();
    if((mutation_possibility < mutation_rate * UINT32_MAX) && (elapsed_time >= mutation_interval)) {
        // Apply a new random color mutation
        uint8_t pixel_index = get_rand_32() % led_strip->Length(); // Random pixel index
        uint16_t mutation_step = min_mutation_step + (get_rand_32() % (max_mutation_step - min_mutation_step + 1));
        rainbow_strip[pixel_index] = (rainbow_strip[pixel_index] + mutation_step) % 360;
        led_strip->SetPixel(pixel_index, Rainbow360[rainbow_strip[pixel_index]]);
        last_mutation_time = current_time;
        return_value = true; // A mutation occurred
    }

    auto circular_distance = [](uint16_t a, uint16_t b) -> uint16_t {
        uint16_t diff = (a > b) ? (a - b) : (b - a);
        uint16_t min_diff = (diff > 180) ? (360 - diff) : diff;
        return min_diff;
    };

    for (int i = 0; i < led_strip->Length(); i++) {
        uint16_t left_color = (i == 0) ? rainbow_strip[led_strip->Length() - 1] : rainbow_strip[i - 1];
        uint16_t current_color = rainbow_strip[i];
        uint16_t right_color = (i == led_strip->Length() - 1) ? rainbow_strip[0] : rainbow_strip[i + 1];

        auto signed_ring_distance = [](int a, int b) -> int {
            int d = (a - b + 540) % 360 - 180;
            return d;
        };

        if (circular_distance(current_color, left_color) >= min_diffusion_diff ||
            circular_distance(current_color, right_color) >= min_diffusion_diff) {
            int left_distance = signed_ring_distance(left_color, current_color);
            int right_distance = signed_ring_distance(right_color, current_color);
            int avg_distance = (left_distance + right_distance) / 3;
            int new_color = (current_color + avg_distance + 360) % 360;
            updated_rainbow_strip[i] = static_cast<uint16_t>(new_color);
            return_value = true; // A change occurred
        } else {
            updated_rainbow_strip[i] = current_color;
        }
    }

    for(int i = 0; i < led_strip->Length(); i++) {
        led_strip->SetPixel(i, Rainbow360[updated_rainbow_strip[i]]);
        rainbow_strip[i] = updated_rainbow_strip[i];
    }

    return return_value;
}