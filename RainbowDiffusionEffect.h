#ifndef RAINBOW_DIFFUSION_EFFECT_H
#define RAINBOW_DIFFUSION_EFFECT_H

#include "Effect.h"

class RainbowDiffusionEffect : public Effect {
public:
    /**
     * @brief Construct a new RainbowDiffusionEffect object
     * 
     * @param strip Pointer to the LED strip to apply the effect on
     * @param start_time_ms Start time of the effect in milliseconds
     * @param duration_ms Duration of the effect in milliseconds
     * @param diffusion_rate Rate of diffusion effect (0.0 to 1.0)
     * @param max_step_size Maximum step size for diffusion effect
     */
    RainbowDiffusionEffect(LEDStrip* strip, uint32_t start_time_ms, uint32_t duration_ms = 10000,
        float diffusion_rate = 0.25f, int16_t max_step_size = 5, uint32_t refresh_interval = 200);
    
    /**
     * @brief Update the diffusion effect based on the current time
     * 
     * @param current_time Current time in milliseconds
     * 
     * @return true if something is changed, false otherwise
     */
    bool Update(uint32_t current_time) override;

private:
    uint16_t rainbow_strip[NUM_LED]; ///< Array to hold the rainbow colors for diffusion effect
    float float_rainbow_strip[NUM_LED]; ///< Array to hold the float rainbow colors for diffusion effect calculations
    float diffusion_rate; ///< Rate of diffusion effect
    uint16_t min_diffusion_diff; ///< Minimum difference for diffusion effect
    int16_t max_step_size; ///< Maximum step size for diffusion effect
    uint32_t last_update_time; ///< Last update time in milliseconds
    uint32_t refresh_interval; ///< Refresh interval for the effect
    uint16_t random_noise_index; ///< Index for random noise application
};


#endif // RAINBOW_DIFFUSION_EFFECT_H