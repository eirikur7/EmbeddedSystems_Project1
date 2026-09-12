// Driver API for a quadrature rotary encoder, polled version.
#ifndef ENCODER_H
#define ENCODER_H

#include "digital_in.h"

class Encoder
{
public:
    // c1 and c2 are the encoder outputs
    Encoder(int pin_c1, int pin_c2);

    void init();
    bool update();  // Update motor state
    int position(); // return shaft position

private:
    Digital_in c1;
    Digital_in c2;
    bool last_c1;
    int count;
};

#endif