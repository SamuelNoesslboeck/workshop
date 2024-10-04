# pragma once

# include "rgb_led.hpp"

// Constants
# define RELAY_CHAIN_MAIN 14
# define RELAY_CHAIN_LIVING 27

namespace living {
    // Structures
    static RGBLed rgb_led = RGBLed(26, 25, 17, 10, 11, 12);

    void setup();

    // Relays
        void set_light_chain_main(bool value);

        void set_light_chain_living(bool value);
    //
}