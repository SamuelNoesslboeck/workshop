# pragma once

# include "rgb_led.hpp"

// Constants
# define BEDSIDE_RELAY_1 14
# define BEDSIDE_RELAY_2 27

namespace Bedside {
    // Structures
    static RGBLed rgb_led = RGBLed(26, 25, 17, 10, 11, 12);

    void setup();

    // Relay
        void activate_relay();

        void deactive_relay();
    //
}