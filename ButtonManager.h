#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H

#include "pico/stdlib.h"

/// @brief Button class to manage button state and interactions
/// @todo Consider adding hold time functionality
/// @todo Add support for long press detection
class Button {
    public:
        uint16_t _reg16; ///< register to hold past 16 readings
        uint16_t _pin; ///< pin of the button
        

    public:
        /**
         * @brief Construct a new Button object
         * 
         * @param p pin number for the button
         */ 
        Button(uint16_t p): _pin(p), _reg16(0) {
            gpio_init(p);
            gpio_set_dir(p, GPIO_IN);
        }

        /**
         * @brief Update the button state by reading the GPIO pin
         * 
         * This method shifts the previous readings and adds the current state of the button.
         */
        void Update() {
            _reg16 = (_reg16 << 1) | gpio_get(_pin);
        }

        /**
         * @brief Check if the button is currently pressed
         * 
         * @return true if the button is pressed, false otherwise
         */
        bool IsPressing() {
            return (_reg16 & 0x1 == 0x1);
        }

        /**
         * @brief Check for a positive edge (button pressed)
         * 
         * @return true if a positive edge is detected, false otherwise
         */
        bool PosEdge() {
            return ((_reg16 & 0x3) == 0x1);
        }

        /**
         * @brief Check for a negative edge (button released)
         * 
         * @return true if a negative edge is detected, false otherwise
         */
        bool NegEdge() {
            return ((_reg16 & 0x3) == 0x2);
        }
};

#endif // BUTTON_MANAGER_H