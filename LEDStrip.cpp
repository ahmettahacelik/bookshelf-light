#include "LEDStrip.h"

LEDStrip::LEDStrip(uint pin)
{
    // Claim and load PIO program
    bool success = pio_claim_free_sm_and_add_program_for_gpio_range(
        &ws2812_program, &pio, &sm, &offset, pin, 1, true
    );
    hard_assert(success);
    ws2812_program_init(pio, sm, offset, pin, 800000, false); // IS_RGBW = false
}

LEDStrip::~LEDStrip()
{
    // Free the PIO program and state machine
    pio_remove_program_and_unclaim_sm(&ws2812_program, pio, sm, offset);
}

void LEDStrip::Update()
{
    for (int i = 0; i < NUM_LED; i++) {
        pio_sm_put_blocking(pio, sm, led_arr[i].GetColorGRB() << 8u);
    }
}

void LEDStrip::SetPixel(uint8_t i, uint32_t rgb)
{
    led_arr[i].SetColor(rgb);
}

void LEDStrip::SetAll(uint32_t rgb)
{
    for(int i = 0; i < NUM_LED; i++) {led_arr[i].SetColor(rgb);}
}

void LEDStrip::SetAll(uint8_t r, uint8_t g, uint8_t b)
{
    for(int i = 0; i < NUM_LED; i++) {led_arr[i].SetColor(r, g, b);}
}

void LEDStrip::SetArray(uint32_t arr[NUM_LED])
{
    for(int i = 0; i < NUM_LED; i++) {led_arr[i].SetColor(arr[i]);}
}

uint32_t LEDStrip::Length()
{
    return NUM_LED;
}

void LEDStrip::ToggleAllMask() {
    for(int i = 0; i < NUM_LED; i++) {
        led_arr[i].ToggleMask();
    }
}

void LEDStrip::SetBrightness(uint8_t b)
{
    for(int i = 0; i < NUM_LED; i++) {
        led_arr[i].SetBrightness(b);
    }
}

void::LEDStrip::SetMask(bool m)
{
    for(int i = 0; i < NUM_LED; i++) {
        led_arr[i].SetMask(m);
    }
}