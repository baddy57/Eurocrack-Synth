#pragma once
#include "Potentiometer.h"
class SelectorMulti : public Potentiometer
{
public:
    SelectorMulti(const Address& a, uint_fast8_t id, int _zones, float pullup_res = 0.f) : Potentiometer(a, id), zones(_zones) {
        Potentiometer::setRange(0, _zones - 1, LIN);
    }
    inline int read()  { return Potentiometer::read(); }
private:
    int zones;
};

