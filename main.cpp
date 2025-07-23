#include <cstdio>

#include <pico/stdlib.h>

#include "RainbowEffect.h"

int main(void) {
    stdio_init_all();

    LEDStrip led_strip;
    led_strip.Update();

    sleep_ms(1000); // Allow some time for initialization

    RainbowEffect rainbow_effect(&led_strip, 0, 10000, true, true);

    sleep_ms(1000); // Allow some time for initialization

    uint32_t current_time = 0;
    while(true) {
        current_time = to_ms_since_boot(get_absolute_time());
        if(rainbow_effect.Update(current_time)) {
            led_strip.Update();
        }

        sleep_ms(20);
    }

    return 0;
}