#ifndef RAINBOW_EFFECT_H
#define RAINBOW_EFFECT_H

#include "Effect.h"
#include "RainbowGradient.h" ///< Get array of rainbow colors 120 elements

class RainbowEffect : public Effect {
public:
    RainbowEffect(LEDStrip* strip, uint32_t start_time_ms, uint32_t duration_ms = 1000, bool is_periodic = true, bool is_shifting = false);

    bool Update(uint32_t current_time) override;

private:
    bool is_shifting; ///< Flag to indicate if the hue is shifting
    uint8_t shift_index; ///< Index for shifting the hue
    uint32_t last_shift_time; ///< Last time the hue was shifted
};

#endif // RAINBOW_EFFECT_H
