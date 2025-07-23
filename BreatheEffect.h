#ifndef BREATHE_EFFECT_H
#define BREATHE_EFFECT_H

#include "Effect.h"

class BreatheEffect : public Effect {
public:
    /**
     * @brief Construct a new BreatheEffect object
     * 
     * @param led_strip Pointer to the LED strip to apply the effect on
     * @param start_time_ms Start time of the effect in milliseconds
     * @param duration_ms Duration of the effect in milliseconds
     * @param color Color of the breathing effect in RGB format (0xRRGGBB)
     * @param min_brightness Minimum brightness level (0-255)
     * @param max_brightness Maximum brightness level (0-255)
     */
    BreatheEffect(LEDStrip* strip, uint32_t start_time_ms, uint32_t duration_ms = 10000, uint32_t color = 0xFFFFFF, uint8_t min_breathness = 0, uint8_t max_brightness = 255);

    /**
     * @brief Update the breathe effect based on the current time
     * 
     * @param current_time Current time in milliseconds
     * 
     * @return true if something is changed, false otherwise
     */
    bool Update(uint32_t current_time) override;

private:
    uint32_t color; ///< Color of the breathing effect in RGB format (0xRRGGBB)
    uint8_t min_brightness; ///< Minimum brightness level (0-255)
    uint8_t max_brightness; ///< Maximum brightness level (0-255)
    int16_t current_brightness; ///< Current brightness level (0-255)
    int16_t last_brightness; ///< Last brightness level (0-255)

    bool increasing; ///< Flag to indicate if brightness is increasing or decreasing
    uint32_t last_update_time; ///< Last time the effect was updated
};

#endif // BREATHE_EFFECT_H
