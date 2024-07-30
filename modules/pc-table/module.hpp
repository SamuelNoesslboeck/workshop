# pragma once

# include "rotary_encoder.hpp"

# define BUZZER_PIN 25
# define BUZZER_FREQ 440

namespace PcTable {
    static RotaryEncoder upper_encoder = RotaryEncoder(23, 19, 18);
    static RotaryEncoder lower_encoder = RotaryEncoder(16, 27, 14);
}

namespace WS {
    namespace Light {
        static bool chain_main = false;
    }
}