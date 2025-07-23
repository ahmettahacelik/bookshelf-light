#ifndef LOADING_EFFECT_H
#define LOADING_EFFECT_H

#include "Effect.h"

class LoadingEffect : public Effect {
public:
    /**
     * @brief Construct a new LoadingEffect object
     * 
     * @param led_strip Pointer to the LED strip to apply the effect on
     * @param start_time_ms Start time of the effect in milliseconds
     * @param duration_ms Duration of the effect in milliseconds  
     * @param color1 First color of the loading effect in RGB format (0xRRGGBB)
     * @param color2 Second color of the loading effect in RGB format (0xR
     */
    LoadingEffect(LEDStrip* strip, uint32_t start_time_ms, uint32_t duration_ms = 10000, uint32_t color1 = 0xFF0000, uint32_t color2 = 0x00FF00);

    /**
     * @brief Update the loading effect based on the current time
     * 
     * @param current_time Current time in milliseconds
     * 
     * @return true if something is changed, false otherwise
     */
    bool Update(uint32_t current_time) override;

private:
    /// @todo Naming of these variables are horrible, consider renaming them
    uint32_t color1; // First color of the loading effect in RGB format (0xRRGGBB)
    uint32_t color2; // Second color of the loading effect in RGB format (0xRRGGBB)
    uint8_t current_color2_index; // Current index for the second color
    uint8_t min_color2_index; // min index for the second color (border)
    uint32_t last_update_time; ///< Last time the effect was updated
    uint32_t total_update_count; ///< Total number of updates needed
    uint32_t delta_update_time; ///< Time between updates in milliseconds
};

#endif // LOADING_EFFECT_H
