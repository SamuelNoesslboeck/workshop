# include "rgb_led.hpp"

# include <Arduino.h>
# include <esp32-hal-ledc.h>

RGBLed::RGBLed(uint8_t pin_r, uint8_t pin_g, uint8_t pin_b, uint8_t ch_r, uint8_t ch_g, uint8_t ch_b) {
    this->pin_r = pin_r;
    this->pin_g = pin_g;
    this->pin_b = pin_b;

    this->ch_r = ch_r;
    this->ch_g = ch_g;
    this->ch_b = ch_b;
}

void RGBLed::setup() {
    // Attach channels
    ledcAttachPin(pin_r, ch_r);
    ledcAttachPin(pin_g, ch_g);
    ledcAttachPin(pin_b, ch_b);

    // Setup channels
    ledcSetup(ch_r, RGBLED_FREQ, RGBLED_ACCR);
    ledcSetup(ch_g, RGBLED_FREQ, RGBLED_ACCR);
    ledcSetup(ch_b, RGBLED_FREQ, RGBLED_ACCR);
}

void RGBLed::set_red(uint8_t duty) {
    ledcWrite(ch_r, duty);
}

void RGBLed::set_green(uint8_t duty) {
    ledcWrite(ch_g, duty);
}

void RGBLed::set_blue(uint8_t duty) {
    ledcWrite(ch_b, duty);
}