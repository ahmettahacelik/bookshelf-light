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
     * @param new_color_rate Rate at which new colors are applied (0.0 to 1.0)
     * @param new_color_interval Interval for applying new colors in milliseconds
     * @param diffusion_speed Speed of the diffusion effect (0.0 to 1.0)
     * @param max_change_per_update Maximum change per update
     */
    DiffusionEffect(LEDStrip* strip, uint32_t start_time_ms, uint32_t duration_ms = 10000,
        float new_color_rate = 0.1f, uint32_t new_color_interval = 1000, float diffusion_speed = 0.2f,
        int32_t max_change_per_update = 32, uint32_t update_interval = 100);

    /**
     * @brief Update the diffusion effect based on the current time
     * 
     * @param current_time Current time in milliseconds
     * 
     * @return true if something is changed, false otherwise
     */
    bool Update(uint32_t current_time) override;

private:
    uint8_t led_strip_red[NUM_LED]; ///< Array to hold the red component of the LED strip
    uint8_t led_strip_green[NUM_LED]; ///< Array to hold the green component of the LED strip
    uint8_t led_strip_blue[NUM_LED]; ///< Array to hold the blue component of
    float new_color_rate; ///< Rate at which new colors are applied
    uint32_t new_color_interval; ///< Interval for applying new colors in milliseconds
    uint32_t last_new_color_time; ///< Last time a new color was applied
    uint32_t min_diffusion_difference; ///< Minimum difference for diffusion effect
    int16_t max_change_per_update; ///< Maximum change per update
    uint32_t last_diffusion_time; ///< Last time diffusion was applied
    uint32_t update_interval; ///< Interval for updating the diffusion effect
    float diffusion_speed; ///< Speed of diffusion effect


    /**
     * @brief Calculate the delta component for diffusion effect
     * 
     * @param current_color_component Current color component (R, G, or B)
     * @param right_color_component Right neighbor color component (R, G, or B)
     * 
     * @return int16_t Calculated delta component
     */
    int16_t CalculateDeltaComponent(uint8_t current_color_component, uint8_t right_color_component) const;

};

#endif // DIFFUSION_EFFECT_H