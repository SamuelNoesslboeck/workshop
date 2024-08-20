// ########################
// #    ROTARY-ENCODER    #
// ########################
//
// A library for dealing with rotary encoders
//
// > Version 0.1.0
//

# include "rotary_encoder.hpp"

RotaryEncoder::RotaryEncoder(uint8_t sw, uint8_t dt, uint8_t clk) {
    // Setup all the required pins
    pinMode(sw, INPUT);
    pinMode(clk, INPUT);
    pinMode(dt, INPUT);

    // Set variables
    this->counter = 0;
    this->clk_last = false;
    this->dt_last = false;

    // Set pins
    this->sw = sw;
    this->clk = clk;
    this->dt = dt;
}

bool RotaryEncoder::check_switch() {
    // Switch is normally open!
    return digitalRead(this->sw) == LOW;
}

RotaryMove RotaryEncoder::check_rotary() {
    bool clk_new = digitalRead(this->clk);
    bool dt_new = digitalRead(this->dt);

    RotaryMove move = RotaryMove::None;

    if (dt_new != dt_last) {
        if (dt_new ^ clk_new) {
            // Clockwise
            counter++;
            move = RotaryMove::CW;
        } else {
            counter--;
            move = RotaryMove::CCW;
        }
    }

    this->clk_last = clk_new;
    this->dt_last = dt_new;

    return move;
}

void RotaryEncoder::reset_counter() {
    this->counter = 0;
}