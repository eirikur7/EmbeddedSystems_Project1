// encoder.h
// Driver API for a quadrature rotary encoder, interrupt driven.
// The implementation lives in encoder.cpp, so nothing here exposes the hardware.
#ifndef ENCODER_H
#define ENCODER_H

#include "digital_in.h"
#include "digital_out.h"

class Encoder
{
public:
    // pin_c1 must be D2, which is the INT0 interrupt pin.
    // pin_c2 is any other port D pin.
    // pin_led is driven high for the duration of each interrupt, so the
    // response time and the handler duration can be measured on a scope.
    Encoder(int pin_c1, int pin_c2, int pin_led);

    void init();    // configure the pins, set up INT0, enable interrupts
    int position(); // current count, 1400 per revolution of the output shaft

    // Called from the INT0 interrupt handler. Public only because an interrupt
    // handler cannot be a class member. Do not call this directly.
    void handle_edge();

private:
    Digital_in c1;
    Digital_in c2;
    Digital_out activity;

    volatile int count; // written by the interrupt, read by position()
};

#endif
