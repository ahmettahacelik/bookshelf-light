#include "DiffusionEffect.h"

#include "RainbowGradient.h"

#include <pico/rand.h>

DiffusionEffect::DiffusionEffect(LEDStrip* strip, uint32_t start_time_ms, uint32_t duration_ms,
                                   float mutation_rate, uint32_t min_mutation_interval, float diffusion_speed)
    : Effect(strip, start_time_ms, duration_ms),
      mutation_rate(mutation_rate),
      last_mutation_time(start_time_ms),
      min_mutation_interval(min_mutation_interval),
      diffusion_speed(diffusion_speed),
      last_update_time(start_time_ms)
{
    led_strip->SetAll(0x000000); // Initialize all LEDs to black
    led_strip->SetBrightness(200); // Set a reasonable brightness level
    led_strip->Update(); // Update the LED strip to apply the initial state
}

bool DiffusionEffect::Update(uint32_t current_time)
{
    if ((current_time < start_time_ms) || !led_strip) {
        return false; // Effect not started or strip not available
    }

    uint32_t elapsed_time = current_time - last_update_time;
    if (elapsed_time == 0) {
        return false; // No time has passed since the last update
    }

    bool return_value = false;

    uint32_t mutation_possibility = get_rand_32();
    if((mutation_possibility < mutation_rate * UINT32_MAX) && ((current_time - last_mutation_time) >= min_mutation_interval)) {
        // Apply a new random color mutation
        uint8_t pixel_index = get_rand_32() % led_strip->Length(); // Random pixel index
        uint32_t new_color_index = get_rand_32() % 360; // Assuming 360 colors in the rainbow gradient
        led_strip->SetPixel(pixel_index, Rainbow360[new_color_index]);
        last_mutation_time = current_time;
        return_value = true; // A mutation occurred
    }

    uint32_t updated_led_strip[NUM_LED];
    for (int i = 0; i < led_strip->Length(); i++) {
        uint32_t left_color = (i == 0) ? led_strip->GetPixel(led_strip->Length() - 1) : led_strip->GetPixel(i - 1);
        uint32_t current_color = led_strip->GetPixel(i);
        uint32_t right_color = (i == led_strip->Length() - 1) ? led_strip->GetPixel(0) : led_strip->GetPixel(i + 1);

        if(left_color != current_color || current_color != right_color) {
            updated_led_strip[i] = WeightedSmoothPixel(left_color, current_color, right_color);
            return_value = true; // A change occurred
        }
        else {
            // If the current pixel is the same as its neighbors, keep it unchanged
            updated_led_strip[i] = current_color;
        }
    }

    for(int i = 0; i < led_strip->Length(); i++) {
        led_strip->SetPixel(i, updated_led_strip[i]);
    }

    return return_value;
};

/// @warning This function does not work properly, I think that I need to think about the math behind color mixing
/// @todo Fix the WeightedSmoothPixel function to properly calculate the diffusion effect
uint32_t DiffusionEffect::WeightedSmoothPixel(uint32_t left_color, uint32_t current_color, uint32_t right_color) const
{
    // Extract color components
    uint8_t left_r = (left_color >> 16) & 0xFF;
    uint8_t left_g = (left_color >> 8) & 0xFF;
    uint8_t left_b = left_color & 0xFF;

    uint8_t current_r = (current_color >> 16) & 0xFF;
    uint8_t current_g = (current_color >> 8) & 0xFF;
    uint8_t current_b = current_color & 0xFF;

    uint8_t right_r = (right_color >> 16) & 0xFF;
    uint8_t right_g = (right_color >> 8) & 0xFF;
    uint8_t right_b = right_color & 0xFF;

    // Weights for each channel
    float left_r_weight   = (left_color == 0x0) ? 0.0f : diffusion_speed;
    float left_g_weight   = (left_color == 0x0) ? 0.0f : diffusion_speed;
    float left_b_weight   = (left_color == 0x0) ? 0.0f : diffusion_speed;

    float center_r_weight = (current_color == 0x0) ? 0.0f : 0.9999f;
    float center_g_weight = (current_color == 0x0) ? 0.0f : 0.9999f;
    float center_b_weight = (current_color == 0x0) ? 0.0f : 0.9999f;

    float right_r_weight  = (right_color == 0x0) ? 0.0f : diffusion_speed;
    float right_g_weight  = (right_color == 0x0) ? 0.0f : diffusion_speed;
    float right_b_weight  = (right_color == 0x0) ? 0.0f : diffusion_speed;

    // Integer total weights for each channel
    int total_r_weight = (left_r_weight   > 0.0f ? 1 : 0) +
                         (center_r_weight > 0.0f ? 1 : 0) +
                         (right_r_weight  > 0.0f ? 1 : 0);

    int total_g_weight = (left_g_weight   > 0.0f ? 1 : 0) +
                         (center_g_weight > 0.0f ? 1 : 0) +
                         (right_g_weight  > 0.0f ? 1 : 0);

    int total_b_weight = (left_b_weight   > 0.0f ? 1 : 0) +
                         (center_b_weight > 0.0f ? 1 : 0) +
                         (right_b_weight  > 0.0f ? 1 : 0);

    // Avoid division by zero
    if (total_r_weight == 0) total_r_weight = 1;
    if (total_g_weight == 0) total_g_weight = 1;
    if (total_b_weight == 0) total_b_weight = 1;

    // Weighted average for each component
    uint8_t new_r = static_cast<uint8_t>(
        (left_r * left_r_weight + current_r * center_r_weight + right_r * right_r_weight) / total_r_weight
    );
    uint8_t new_g = static_cast<uint8_t>(
        (left_g * left_g_weight + current_g * center_g_weight + right_g * right_g_weight) / total_g_weight
    );
    uint8_t new_b = static_cast<uint8_t>(
        (left_b * left_b_weight + current_b * center_b_weight + right_b * right_b_weight) / total_b_weight
    );

    return (new_r << 16) | (new_g << 8) | new_b;
}