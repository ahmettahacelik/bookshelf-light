#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#include "SingleLED.h"
#include "config.h"

// default to pin 2 for ws2812
#ifndef WS2812_PIN
#define WS2812_PIN 2
#endif
// Check the pin is compatible with the platform
#if WS2812_PIN >= NUM_BANK0_GPIOS
#error Attempting to use a pin>=32 on a platform that does not support it
#endif

static inline void put_pixel(PIO pio, uint sm, uint32_t pixel_grb) {
    pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
}

class LEDStrip {
    private:
        // led array
        LED led_arr[NUM_LED];
        PIO pio;
        uint sm;
        uint offset;

    public:
        /**
         * @brief Construct a new LEDStrip object
         * 
         * @param pin GPIO pin to use for the LED strip
         */
        LEDStrip(uint pin = WS2812_PIN);

        /**
         * @brief Destroy the LEDStrip object
         * 
         */
        ~LEDStrip();

        /**
         * @brief Update the LED strip with the current colors
         * 
         */
        void Update();

        /**
         * @brief Set the color of a specific pixel
         * 
         * @param i Index of the pixel
         * @param rgb Color in RGB format
         */
        void SetPixel(uint8_t i, uint32_t rgb);

        /**
         * @brief Get the color of a specific pixel
         * 
         * @param i Index of the pixel
         * @return uint32_t Color in RGB format
         */
        uint32_t GetPixel(uint8_t i) const;

        /**
         * @brief Set the color of all pixels in the strip
         * 
         * @param rgb Color in RGB format
         */
        void SetAll(uint32_t rgb);

        /**
         * @brief Set the color of all pixels in the strip using separate RGB values
         * 
         * @param r Red component (0-255)
         * @param g Green component (0-255)
         * @param b Blue component (0-255)
         */
        void SetAll(uint8_t r, uint8_t g, uint8_t b);

        /**
         * @brief Set the color of the entire LED array from a predefined array
         * 
         * @param arr Array of colors in RGB format
         */
        void SetArray(uint32_t arr[NUM_LED]);

        /**
         * @brief Get the length of the LED strip
         * 
         * @return uint32_t Number of LEDs in the strip
         */
        uint32_t Length();

        /**
         * @brief Toggle the mask of all LEDs in the strip
         * 
         */
        void ToggleAllMask();

        /**
         * @brief Set the brightness of all LEDs in the strip
         * 
         * @param b Brightness level (0-7)
         */
        void SetBrightness(uint8_t b);

        /**
         * @brief Set the mask of all LEDs in the strip
         * 
         * @param m true to mask, false to unmask
         */
        void SetMask(bool m);
};

#endif // LEDSTRIP_H