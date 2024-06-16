# pragma once

# include "rotary_encoder.hpp"

# define BUZZER_PIN D2
# define BUZZER_FREQ 440

namespace PcTable {
    static RotaryEncoder upper_encoder = RotaryEncoder(5, 6, 7);
    static RotaryEncoder lower_encoder = RotaryEncoder(8, 9, 10);
}