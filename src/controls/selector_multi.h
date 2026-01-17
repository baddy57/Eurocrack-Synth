#pragma once

#include "potentiometer.h"

class SelectorMulti : public Potentiometer
{
public:
    SelectorMulti(const Address& a, uint_fast8_t modulePin, int _zones, float pullup_res = 0.f) 
        : Potentiometer(a, modulePin), zones(_zones) {
        Potentiometer::setRange(0, _zones - 1, LIN);
    }
    inline int read()  { return Potentiometer::read(); }
private:
    int zones;
};

