# include "rtrig.hpp"

RTrig::RTrig() : last(false) {

}

bool RTrig::check(bool in) {
    bool res = (in != last);
    last = in;
    return res;
}