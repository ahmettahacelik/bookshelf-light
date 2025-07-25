#include <cstdio>

#include <pico/stdlib.h>

#include "RainbowEffect.h"
#include "BreatheEffect.h"
#include "LoadingEffect.h"
#include "DiffusionEffect.h"
#include "RainbowDiffusionEffect.h"

int main(void) {
    stdio_init_all();
    printf("Starting bookshelf-light...\n");

    LEDStrip led_strip;
    led_strip.Update();

    sleep_ms(1000); // Allow some time for initialization

    //RainbowEffect rainbow_effect(&led_strip, 0, 10000, true, true);

    //BreatheEffect breathe_effect(&led_strip, 0, 10000, 0x55ff00, 0, 255);

    //LoadingEffect loading_effect(&led_strip, 0, 600000, 0x000000, 0x00FFFF);

    //DiffusionEffect diffusion_effect(&led_strip, 0, 10000, 0.1f, 2000, 0.2f, 32, 100);

    RainbowDiffusionEffect rainbow_diffusion_effect(&led_strip, 0, 10000, 0.25f, 10, 20);

    sleep_ms(5000); // Allow some time for initialization

    uint32_t current_time = 0;
    while(true) {
        current_time = to_ms_since_boot(get_absolute_time());
        if(rainbow_diffusion_effect.Update(current_time)) {
            led_strip.Update();
        }

        sleep_ms(10);
    }

    return 0;
}