# pragma once

# include <DHTesp.h>

# include "rotary_encoder.hpp"

# define BUZZER_PIN 25
# define BUZZER_FREQ 440

# define DHT_PIN 26

namespace pc_table {
    static RotaryEncoder upper_encoder = RotaryEncoder(23, 19, 18);
    static RotaryEncoder lower_encoder = RotaryEncoder(16, 27, 14);

    static DHTesp dht;

    void setup();

    float get_temp();
    float get_humid();
}