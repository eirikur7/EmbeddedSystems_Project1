// Driver API for a quadrature rotary encoder, interrupt version.
#ifndef ENCODER_PART3_H
#define ENCODER_PART3_H

#include "digital_in.h"
#include "digital_out.h"

class Encoder
{
public:
    // c1 must be on D2, which is the INT0 pin. c2 is any other port D pin.
    // led is pulsed for the duration of each interrupt, for scope measurements.
    Encoder(int pin_c1, int pin_c2, int pin_led);

    void init();
    int position(); // return shaft position

    // Called from the interrupt handler.
    // Public only because an interrupt handler cannot be a class member.
    void handle_edge();

private:
    Digital_in c1;
    Digital_in c2;
    Digital_out activity;

    volatile int count; // written by the interrupt, read by position()
};

#endif
