# include "module.hpp"

# include "Arduino.h"

void living::setup() {
    pinMode(RELAY_CHAIN_MAIN, OUTPUT);
    pinMode(RELAY_CHAIN_LIVING, OUTPUT);

    living::rgb_led.setup();

    living::set_light_chain_main(false);
    living::set_light_chain_living(false);
    
}

void living::set_light_chain_main(bool value) {
    digitalWrite(RELAY_CHAIN_MAIN, value);
}

void living::set_light_chain_living(bool value) {
    digitalWrite(RELAY_CHAIN_LIVING, value);
}