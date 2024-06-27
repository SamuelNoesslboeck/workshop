# include "module.hpp"

# include "Arduino.h"

void Bedside::setup() {
    pinMode(BEDSIDE_RELAY_1, OUTPUT);
    pinMode(BEDSIDE_RELAY_2, OUTPUT);

    Bedside::rgb_led.setup();

    Bedside::deactive_relay();
}

void Bedside::activate_relay() {
    digitalWrite(BEDSIDE_RELAY_1, LOW);
    digitalWrite(BEDSIDE_RELAY_2, LOW);
}

void Bedside::deactive_relay() {
    digitalWrite(BEDSIDE_RELAY_1, HIGH);
    digitalWrite(BEDSIDE_RELAY_2, HIGH);
}