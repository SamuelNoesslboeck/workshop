# pragma once

class TOff {
public:
    unsigned long last, phase;

    TOff(unsigned long phase);

    bool check(bool in);
}; 