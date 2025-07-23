#ifndef SINGLELED_H
#define SINGLELED_H

#define DEFAULT_BRIGHTNESS 5

#include <stdint.h>

/**
 * @brief Class representing a single LED in the strip.
 * 
 * @todo Consider using a color struct or class for better color management.
 * @todo Consider dynamic size allocation for the LED array if needed instead of fixed size.
 */
class LED
{
private:
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t brightness; ///< in the range [0, 7]
    bool mask;

public:
    /**
     * @brief Construct a new LED object with default color and brightness.
     * 
     */
    LED();

    /**
     * @brief Get the Color G R B object
     * 
     * @return uint32_t color code of the LED in GRB format
     */
    uint32_t GetColorGRB(void);
    
    /**
     * @brief Set the Color of the LED
     * 
     * @param r Red component (0-255)
     * @param g Green component (0-255)
     * @param b Blue component (0-255)
     */
    void SetColor(uint8_t r, uint8_t g, uint8_t b);

    /**
     * @brief Set the Color of the LED using a single RGB value
     * 
     * @param rgb RGB value in the format 0xRRGGBB
     */
    void SetColor(uint32_t rgb);

    /**
     * @brief Set the Mask of the LED
     * 
     * @param m true to mask, false to unmask
     */
    void SetMask(bool m);

    /**
     * @brief Toggle the Mask of the LED
     * 
     */
    void ToggleMask();

    /**
     * @brief Set the Brightness of the LED
     * 
     * @param b Brightness level (0-7)
     */
    void SetBrightness(uint8_t b);

    /**
     * @brief Increase the Brightness of the LED by 1
     * 
     */
    void IncreaseBrightness();

    /**
     * @brief Decrease the Brightness of the LED by 1
     * 
     */
    void DecreaseBrightness();
};

#endif // SINGLELED_H