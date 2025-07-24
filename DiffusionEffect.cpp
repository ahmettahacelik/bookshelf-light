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
    led_strip->SetAll(0xFFFFFF); // Initialize all LEDs to white
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

    /// @todo This algorithm implements pixel by pixel not whole strip at once.
    for (int i = 0; i < led_strip->Length(); i++) {
        uint32_t left_color = (i == 0) ? led_strip->GetPixel(led_strip->Length() - 1) : led_strip->GetPixel(i - 1);
        uint32_t current_color = led_strip->GetPixel(i);
        uint32_t right_color = (i == led_strip->Length() - 1) ? led_strip->GetPixel(0) : led_strip->GetPixel(i + 1);

        if(left_color != current_color || current_color != right_color) {
            // If the current pixel is different from its neighbors, apply diffusion
            led_strip->SetPixel(i, WeightedSmoothPixel(left_color, current_color, right_color));
            return_value = true; // A change occurred
        }
    }

    return return_value;
};

uint32_t DiffusionEffect::WeightedSmoothPixel(uint32_t left_color, uint32_t current_color, uint32_t right_color) const
{
    // Calculate the weighted average of the three colors with diffusion_speed
    float left_weight = diffusion_speed;
    float right_weight = diffusion_speed;
    float center_weight = 1.0f;

    uint8_t left_r = (left_color >> 16) & 0xFF;
    uint8_t left_g = (left_color >> 8) & 0xFF;
    uint8_t left_b = left_color & 0xFF;

    uint8_t current_r = (current_color >> 16) & 0xFF;
    uint8_t current_g = (current_color >> 8) & 0xFF;
    uint8_t current_b = current_color & 0xFF;

    uint8_t right_r = (right_color >> 16) & 0xFF;
    uint8_t right_g = (right_color >> 8) & 0xFF;
    uint8_t right_b = right_color & 0xFF;

    float total_weight = left_weight + center_weight + right_weight;

    uint8_t new_r = static_cast<uint8_t>(
        (left_r * left_weight + current_r * center_weight + right_r * right_weight) / total_weight
    );
    uint8_t new_g = static_cast<uint8_t>(
        (left_g * left_weight + current_g * center_weight + right_g * right_weight) / total_weight
    );
    uint8_t new_b = static_cast<uint8_t>(
        (left_b * left_weight + current_b * center_weight + right_b * right_weight) / total_weight
    );

    return (new_r << 16) | (new_g << 8) | new_b; // Return the new color in RGB format
}