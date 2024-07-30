# include "toff.hpp"

# include "esp32-hal.h"

TOff::TOff(unsigned long phase) : last(0), phase(phase) {

}

bool TOff::check(bool in) {
    if (in) {
        last = millis();
        return true;
    }

    if ((last + phase) > millis()) {
        return true;
    }
    
    return false;
}