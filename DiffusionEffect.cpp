#include "DiffusionEffect.h"

#include "RainbowGradient.h"

#include <pico/rand.h>
#include <cmath>
#include <algorithm> // For std::clamp
#include <cstdio> // For printf

/// @note It works as intended but it is not very visually appealing.
/// @note RainbowDiffusionEffect is a more visually appealing effect.
/// @note I do not consider improving this effect further.
/// @warning It uses some external libraries like <cmath> and <algorithm> for calculations.

DiffusionEffect::DiffusionEffect(LEDStrip* strip, uint32_t start_time_ms, uint32_t duration_ms,
    float new_color_rate, uint32_t new_color_interval, float diffusion_speed,
    int32_t max_change_per_update, uint32_t update_interval)
    : Effect(strip, start_time_ms, duration_ms),
      new_color_rate(new_color_rate),
      new_color_interval(new_color_interval),
      last_new_color_time(start_time_ms),
      diffusion_speed(diffusion_speed),
      max_change_per_update(max_change_per_update),
      last_diffusion_time(start_time_ms),
      update_interval(update_interval)
{
    min_diffusion_difference = static_cast<uint32_t>(2 / diffusion_speed); 

    led_strip->SetAll(0x000000); // Initialize all LEDs to black
    led_strip->SetBrightness(200); // Set brightness to a reasonable level
    led_strip->Update();
    // Initialize the LED strip copy
    for (int i = 0; i < NUM_LED; i++) {
        led_strip_red[i] = 0x00;
        led_strip_green[i] = 0x00;
        led_strip_blue[i] = 0x00;
    }
}

bool DiffusionEffect::Update(uint32_t current_time)
{
    if ((current_time < start_time_ms) || !led_strip) {
        return false; // Effect not started or strip not available
    }

    uint32_t elapsed_time = current_time - last_diffusion_time;
    if (elapsed_time < update_interval) {
        return false; // Not enough time has passed to update the diffusion effect
    }

    bool return_value = false;

    uint32_t new_color_possibility = get_rand_32();
    if((new_color_possibility < new_color_rate * UINT32_MAX) && ((current_time - last_new_color_time) >= new_color_interval)) {
        // Apply a new random color mutation
        uint8_t pixel_index = get_rand_32() % led_strip->Length(); // Random pixel index
        uint32_t new_color_index = get_rand_32() % 360; // Assuming 360 colors in the rainbow gradient
        led_strip->SetPixel(pixel_index, Rainbow360[new_color_index]);
        last_new_color_time = current_time;
        return_value = true; // A mutation occurred
    }

    for(int i = 0; i < led_strip->Length(); i++) {
        uint32_t color = led_strip->GetPixel(i);
        led_strip_red[i] = (color >> 16) & 0xFF; // Extract red component
        led_strip_green[i] = (color >> 8) & 0xFF; // Extract green component
        led_strip_blue[i] = color & 0xFF; // Extract blue component
    }
    for (int i = 0; i < led_strip->Length() - 1; i++) {
        uint32_t current_color = led_strip->GetPixel(i);
        uint32_t right_color = led_strip->GetPixel(i + 1);

        if(current_color == right_color) {
            // If the current pixel is the same as the right neighbor, skip diffusion
            continue;
        }

        uint8_t r_current = (current_color >> 16) & 0xFF;
        uint8_t g_current = (current_color >> 8) & 0xFF;
        uint8_t b_current = current_color & 0xFF;

        uint8_t r_right = (right_color >> 16) & 0xFF;
        uint8_t g_right = (right_color >> 8) & 0xFF;
        uint8_t b_right = right_color & 0xFF;

        // Calculate the deltas for each color component
        int16_t r_delta = CalculateDeltaComponent(r_current, r_right);
        int16_t g_delta = CalculateDeltaComponent(g_current, g_right);
        int16_t b_delta = CalculateDeltaComponent(b_current, b_right);

        printf("Diffusion between pixels %d and %d: R(%d - %d = %d), G(%d - %d = %d), B(%d - %d = %d)\n",
               i, i + 1, r_current, r_right, r_delta,
               g_current, g_right, g_delta,
               b_current, b_right, b_delta);

        if(r_delta == 0 && g_delta == 0 && b_delta == 0) {
            // If no change is needed, skip this pixel
            continue;
        }

        led_strip_red[i] -= r_delta;
        led_strip_green[i] -= g_delta;
        led_strip_blue[i] -= b_delta;

        led_strip_red[i + 1] += r_delta;
        led_strip_green[i + 1] += g_delta;
        led_strip_blue[i + 1] += b_delta;

        return_value = true; // A change occurred
    }

    if(return_value) {
        // If any changes were made, update the LED strip
        for (int i = 0; i < led_strip->Length(); i++) {

            led_strip->SetPixel(i, (led_strip_red[i] << 16) | (led_strip_green[i] << 8) | led_strip_blue[i]);
        }
        last_diffusion_time = current_time; // Update the last diffusion time
    }

    return return_value;
};

int16_t DiffusionEffect::CalculateDeltaComponent(uint8_t current_color_component, uint8_t right_color_component) const
{
    int16_t diff = static_cast<int16_t>(current_color_component) - static_cast<int16_t>(right_color_component);
    if(abs(diff) < min_diffusion_difference) return 0; // If the difference is less than the minimum, return 0
    int16_t delta = static_cast<int16_t>(diffusion_speed * diff / 2); // Apply diffusion speed
    if(delta > max_change_per_update) delta = max_change_per_update; // Limit the change to max_change_per_update
    if(delta < -max_change_per_update) delta = -max_change_per_update; // Limit the change to -max_change_per_update
    return delta;
}