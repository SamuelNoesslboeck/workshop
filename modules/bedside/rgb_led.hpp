# pragma once

# include "inttypes.h"

# define RGBLED_FREQ 1000.0
# define RGBLED_ACCR 8

struct RGBLed {
private:

public:
    uint8_t pin_r, pin_g, pin_b;
    uint8_t ch_r, ch_g, ch_b;

    RGBLed(uint8_t pin_r, uint8_t pin_g, uint8_t pin_b, uint8_t ch_r, uint8_t ch_g, uint8_t ch_b);

    void setup();

    void set_red(uint8_t duty);

    void set_green(uint8_t duty);

    void set_blue(uint8_t duty);
};