#ifndef DIFFUSION_EFFECT_H
#define DIFFUSION_EFFECT_H

#include "Effect.h"

class DiffusionEffect : public Effect {
public:
    /**
     * @brief Construct a new DiffusionEffect object
     * 
     * @param strip Pointer to the LED strip to apply the effect on
     * @param start_time_ms Start time of the effect in milliseconds
     * @param duration_ms Duration of the effect in milliseconds
     * @param mutation_rate Rate at which new random colors appear (0.0 - 1.0)
     * @param min_mutation_interval Minimum interval between mutations in milliseconds
     * @param diffusion_speed Speed of the diffusion effect (0.0 - 1.0)
     */
    DiffusionEffect(LEDStrip* strip, uint32_t start_time_ms, uint32_t duration_ms = 10000,
        float mutation_rate = 0.1f, uint32_t min_mutation_interval = 10000,
        float diffusion_speed = 0.5f);

    /**
     * @brief Update the diffusion effect based on the current time
     * 
     * @param current_time Current time in milliseconds
     * 
     * @return true if something is changed, false otherwise
     */
    bool Update(uint32_t current_time) override;

private:
    float mutation_rate; ///< Mutation rate for the new random color appears
    uint32_t last_mutation_time; ///< Last time a mutation occurred
    uint32_t min_mutation_interval; ///< Minimum interval between mutations in milliseconds
    float diffusion_speed; ///< Speed of diffusion effect
    uint32_t last_update_time; ///< Last time the effect was updated

    /**
     * @brief Calculate the weighted average of three colors for diffusion effect
     * 
     * @param left_color left color in RGB format (0xRRGGBB)
     * @param current_color current color in RGB format (0xRRGGBB)
     * @param right_color right color in RGB format (0xRRGGBB)
     * @return uint32_t new color in RGB format (0xRRGGBB)
     */
    uint32_t WeightedSmoothPixel(uint32_t left_color, uint32_t current_color, uint32_t right_color) const;

};

#endif // DIFFUSION_EFFECT_H