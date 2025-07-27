#include "RainbowDiffusionEffect.h"

#include "RainbowGradient.h"
#include <pico/rand.h>

/// @note Consider using a more sophisticated random number generator or noise function for better diffusion effects
/// @todo Consider using perlin noise or similar algorithm for more natural diffusion effects

RainbowDiffusionEffect::RainbowDiffusionEffect(LEDStrip* strip, uint32_t start_time_ms, uint32_t duration_ms, float diffusion_rate, int16_t max_step_size, uint32_t refresh_interval)
    : Effect(strip, start_time_ms, duration_ms),
      diffusion_rate(diffusion_rate),
      max_step_size(max_step_size),
      refresh_interval(refresh_interval),
      last_update_time(start_time_ms)
{
    min_diffusion_diff = (int)(1.0 / diffusion_rate); // Calculate minimum difference based on diffusion rate

    int16_t hue = (get_rand_32() % 360); // Randomly initialize the hue for the rainbow effect
    for(int i = 0; i < led_strip->Length(); i++) {
        int16_t hue_delta = ((get_rand_32() % 20) - 10); // Randomly adjust the hue for each pixel [-10, 10]
        hue += hue_delta; // Adjust the hue for the current pixel
        while(hue < 0) hue += 360; // Ensure the hue is non-negative
        while(hue >= 360.0f) hue -= 360; // Ensure the hue is within 0-359
        uint16_t color_index = (uint16_t) hue; // Convert hue to an index
        color_index %= 360; // Wrap around to keep the index within 0-359
        rainbow_strip[i] = color_index; // Initialize the rainbow strip with random colors
        float_rainbow_strip[i] = (float) rainbow_strip[i]; // Initialize the float rainbow strip
        led_strip->SetPixel(i, Rainbow360[rainbow_strip[i]]); // Set the initial color of the pixel
    }
    // Brightness is controlled by the ButtonManager, so we don't set it here
    //led_strip->SetBrightness(200); // Set a reasonable brightness level
    led_strip->Update(); // Update the LED strip to apply the initial state

    random_noise_index = get_rand_32() % led_strip->Length(); // Initialize the random noise index
}

bool RainbowDiffusionEffect::Update(uint32_t current_time)
{
    if ((current_time < start_time_ms) || !led_strip) {
        return false; // Effect not started or strip not available
    }

    uint32_t elapsed_time = current_time - last_update_time;
    if (elapsed_time < refresh_interval) {
        return false; // Not enough time has passed since the last update
    }

    bool return_value = false;
    
    auto circular_distance = [](uint16_t a, uint16_t b) -> uint16_t {
        uint16_t diff = (a > b) ? (a - b) : (b - a);
        uint16_t min_diff = (diff > 180) ? (360 - diff) : diff;
        return min_diff;
    };

    auto signed_ring_distance = [](int a, int b) -> int {
        int d = (a - b + 540) % 360 - 180;
        return d;
    };

    /// @todo instead of uniform randomization, consider using perlin noise or similar algorithm
    /// @todo randomly decrease the color index of a random pixel to create noise
    float random_noise_change_probability = 0.001f; // 0.1% chance to change the random noise pixel
    if(get_rand_32() < (random_noise_change_probability * UINT32_MAX)) {
        random_noise_index = get_rand_32() % led_strip->Length(); // Randomly select a new pixel index
    }

    uint16_t random_increase = get_rand_32() % (2 * min_diffusion_diff); // Randomly increase the color index by a small amount
    float_rainbow_strip[random_noise_index] += random_increase;
    while(float_rainbow_strip[random_noise_index] >= 360.0f) float_rainbow_strip[random_noise_index] -= 360.0f; // Ensure the value is within 0-359
    rainbow_strip[random_noise_index] = (int) float_rainbow_strip[random_noise_index];
    rainbow_strip[random_noise_index] %= 360; // Wrap around to keep the index within 0-359
    return_value = true; // A change occurred due to noise

    // iterate through the entire strip, except the last pixel to form a linear not ring
    /// @note check every pair of current and right neighbor will cover the entire strip, no need to check left neighbor
    /// @todo consider using a helper function to reduce code duplication
    for (int i = 0; i < led_strip->Length(); i++) {
        uint16_t current_index = i;
        uint16_t right_index = (i == led_strip->Length() - 1) ? 0 : (i + 1);

        uint16_t current_color_index = rainbow_strip[current_index];
        uint16_t right_color_index = rainbow_strip[right_index];

        /// @todo make this algorithm as a diffusion helper function
        if(circular_distance(current_color_index, right_color_index) >= min_diffusion_diff) {
            int16_t diff = signed_ring_distance(current_color_index, right_color_index);
            float delta = (diffusion_rate * (float) diff);
            if(delta > max_step_size) delta = (float) max_step_size; // Limit the step size to the maximum allowed
            if(delta < -max_step_size) delta = (float) -max_step_size; // Limit the
            float_rainbow_strip[current_index] -= delta;
            float_rainbow_strip[right_index] += delta;
            return_value = true; // A change occurred
        }
    }

    for(int i = 0; i < led_strip->Length(); i++) {
        while(float_rainbow_strip[i] < 0) float_rainbow_strip[i] += 360.0f; // Ensure the value is non-negative
        while(float_rainbow_strip[i] >= 360.0f) float_rainbow_strip[i] -= 360.0f; // Ensure the value is within 0-359
        rainbow_strip[i] = (int) float_rainbow_strip[i];
        rainbow_strip[i] %= 360; // Wrap around to keep the index within 0-359
        // Update the LED strip with the new color
        led_strip->SetPixel(i, Rainbow360[rainbow_strip[i]]);
    }

    last_update_time = current_time; // Update the last update time

    return return_value;
}