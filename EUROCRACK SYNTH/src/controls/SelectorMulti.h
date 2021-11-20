#pragma once
#include "Potentiometer.h"
class SelectorMulti : public Potentiometer
{
public:
    SelectorMulti(const Address& a, uint_fast8_t id, int _zones, float pullup_res = 0.f) : Potentiometer(a, id), zones(_zones) { }
    inline int read()  { return Potentiometer::read(0, zones - 1); }
private:
    int zones;
};

