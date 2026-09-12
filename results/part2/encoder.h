// encoder.h
// Driver API for a quadrature rotary encoder, polled version.
// The implementation lives in encoder.cpp, so nothing here exposes the
// underlying hardware.
#ifndef ENCODER_H
#define ENCODER_H

#include "digital_in.h"

class Encoder
{
public:
    // pin_c1, pin_c2: port D pin numbers of the two encoder channels
    Encoder(int pin_c1, int pin_c2);

    void init();    // configure both channel pins and seed the reference state
    bool update();  // sample once, true if a state change was handled
    int position(); // current count, 1400 per revolution of the output shaft

private:
    Digital_in c1;
    Digital_in c2;
    bool last_c1; // C1 level at the previous sample
    int count;    // signed position count
};

#endif
