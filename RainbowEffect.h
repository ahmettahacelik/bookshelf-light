#ifndef RAINBOW_EFFECT_H
#define RAINBOW_EFFECT_H

#include "Effect.h"
#include "RainbowGradient.h" ///< Get array of rainbow colors 120 elements

class RainbowEffect : public Effect {
public:
    /**
     * @brief Construct a new RainbowEffect object
     * 
     * @param led_strip Pointer to the LED strip to apply the effect on
     * @param start_time_ms Start time of the effect in milliseconds
     * @param duration_ms Duration of the effect in milliseconds
     * @param is_periodic Flag to indicate if the effect is periodic
     * @param is_shifting Flag to indicate if the hue should shift over time
     */
    RainbowEffect(LEDStrip* strip, uint32_t start_time_ms, uint32_t duration_ms = 1000, bool is_periodic = true, bool is_shifting = false);

    /**
     * @brief Update the rainbow effect based on the current time
     * 
     * @param current_time Current time in milliseconds
     * 
     * @return true if something is changed, false otherwise
     */
    bool Update(uint32_t current_time) override;

private:
    bool is_shifting; ///< Flag to indicate if the hue is shifting
    uint8_t shift_index; ///< Index for shifting the hue
    uint32_t last_shift_time; ///< Last time the hue was shifted
};

#endif // RAINBOW_EFFECT_H
