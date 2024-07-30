# include "rtrig.hpp"

RTrig::RTrig() : last(false) {
    
}

bool RTrig::check(bool in) {
    bool res = (!last) & in;
    last = in;
    return res;
}