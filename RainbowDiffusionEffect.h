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
     * @param mutation_rate Rate at which new random colors appear (0.0 - 1.0)
     * @param min_mutation_step Minimum step size for color mutation
     * @param max_mutation_step Maximum step size for color mutation
     * @param mutation_interval Minimum interval between mutations in milliseconds
     * @param min_diffusion_diff Minimum difference for diffusion effect
     */
    RainbowDiffusionEffect(LEDStrip* strip, uint32_t start_time_ms, uint32_t duration_ms = 10000,
        float mutation_rate = 0.1f, uint16_t min_mutation_step = 10, uint16_t max_mutation_step = 30,
        uint32_t mutation_interval = 1000, uint16_t min_diffusion_diff = 4);
    
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
    uint16_t updated_rainbow_strip[NUM_LED]; ///< Array to hold the updated rainbow colors after diffusion
    float mutation_rate; ///< Rate at which colors jumped with defined step size
    uint16_t min_mutation_step; ///< Minimum step size for color mutation
    uint16_t max_mutation_step; ///< Maximum step size for color mutation
    uint32_t last_mutation_time; ///< Last time a mutation occurred
    uint32_t mutation_interval; ///< Minimum interval between mutations in milliseconds
    uint16_t min_diffusion_diff; ///< Minimum difference for diffusion effect
    uint8_t pixel_index; ///< Index of the pixel that was mutated last
};


#endif // RAINBOW_DIFFUSION_EFFECT_H