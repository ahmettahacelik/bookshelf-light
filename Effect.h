#ifndef EFFECT_H
#define EFFECT_H

#include "LEDStrip.h"

/// @todo Remove is_periodic flag and related logic
/// @todo Remove duration_ms and related logic
/// @todo Add refresh_interval_ms or refresh_speed to control how often the effect updates

class Effect
{
protected:
    uint32_t start_time_ms; ///< Start time in milliseconds
    uint32_t current_time_ms; ///< Current time in milliseconds
    uint32_t duration_ms;   ///< Duration of the effect in milliseconds
    LEDStrip* led_strip; ///< Pointer to the LED strip being affected
    bool is_periodic; ///< Flag to indicate if the effect is periodic

public:
    /**
     * @brief Construct a new Effect object
     * 
     * @param strip Pointer to the LED strip to apply the effect on
     * @param start_time_ms Start time of the effect in milliseconds
     * @param duration_ms Duration of the effect in milliseconds
     */
    Effect(LEDStrip* strip, uint32_t start_time_ms, uint32_t duration_ms = 1000, bool is_periodic = true)
        : led_strip(strip), start_time_ms(start_time_ms), duration_ms(duration_ms), current_time_ms(0), is_periodic(is_periodic)
        {
            led_strip->SetAll(0xFFFFFF); // Initialize all LEDs to white
            led_strip->SetBrightness(DEFAULT_BRIGHTNESS); // Set default brightness
        }

    /**
     * @brief Update the effect based on the current time
     * 
     * @param current_time Current time in milliseconds
     * 
     * @return true if something is changed, false otherwise
     */
    virtual bool Update(uint32_t current_time)
    {
        current_time_ms = current_time;
        if((current_time_ms < start_time_ms) || !led_strip) {
            return false; // Effect not started or strip not available
        }

        if(current_time_ms >= start_time_ms + duration_ms) {
            if(is_periodic) {
                Reset(); // Reset the effect if periodic
                return true; // Effect has been reset
            } else {
                return false; // Effect has ended
            }
        }

        // example base behvaior: blink
        uint32_t elapsed_time = current_time_ms - start_time_ms;
        if(elapsed_time <= (duration_ms / 2)) {
            led_strip->SetMask(false); // Unmask all LEDs after half duration
        }
        else {
            led_strip->SetMask(true); // Mask all LEDs before half duration
        }

        return true;
    }

    /**
     * @brief Reset the effect to its initial state
     */
    void Reset() {
        start_time_ms = current_time_ms;
        current_time_ms = 0;
    }
};

#endif // EFFECT_H