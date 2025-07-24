#include "RainbowEffect.h"

#include "RainbowGradient.h"

RainbowEffect::RainbowEffect(LEDStrip* led_strip, uint32_t start_time_ms, uint32_t duration_ms, bool is_periodic, bool is_shifting)
    : Effect(led_strip, start_time_ms, duration_ms, is_periodic),
      is_shifting(is_shifting),
      last_shift_time(start_time_ms),
      shift_index(0)
{}

bool RainbowEffect::Update(uint32_t current_time)
{
    // Call base Effect update to handle time checks and periodic logic
    if ((current_time < start_time_ms) || !led_strip) {
        return false;
    }

    current_time_ms = current_time;

    if (current_time_ms >= start_time_ms + duration_ms) {
        if (is_periodic) {
            Reset();
            return true;
        }
        return false;
    }

    const int num_leds = led_strip->Length();

    bool return_value = false;

    static uint8_t last_index = 255; // Force update on first run
    if (shift_index != last_index) {
        for (int i = 0; i < num_leds; ++i) {
            uint8_t index = (i + shift_index) % 120;
            uint32_t color = Rainbow120[index];
            led_strip->SetPixel(i, color);
        }
        last_index = shift_index;
        return_value = true; // Something changed
    }

    if (is_shifting) {
        uint32_t interval = duration_ms / 120;

        if (current_time - last_shift_time >= interval) {
            shift_index = (shift_index + 1) % 120;
            last_shift_time = current_time;
        }
    }

    return return_value;
}
