// encoder.cpp
// Quadrature encoder driver, polled version. Counts state changes on channel
// C1 and reads the level of C2 at each C1 edge to decide which way the shaft
// is turning. All pin access goes through Digital_in, so this file needs no
// register definitions of its own and the hardware stays hidden from the API.

#include "encoder.h"

Encoder::Encoder(int pin_c1, int pin_c2) : c1(pin_c1), c2(pin_c2)
{
    count = 0;
    last_c1 = false; // placeholder, the real value is seeded in init()
}

void Encoder::init()
{
    c1.init();
    c2.init();

    // Seed the reference level from the pin's current state, so the first
    // call to update() does not report a state change that never happened.
    last_c1 = c1.is_hi();
}

// Samples the encoder once. Must be called at least every 238 us to keep the
// direction correct, since that is how long after a C1 edge C2 flips.
// Returns true when a C1 state change was found and handled, false otherwise.
bool Encoder::update()
{
    bool current_c1 = c1.is_hi();

    // Any change on C1, rising or falling, is one counted pulse.
    if (current_c1 != last_c1)
    {
        // C2 is a quarter cycle out of phase with C1, so its level at this
        // instant gives the direction: differing levels mean one way round,
        // matching levels mean the other.
        if (c2.is_hi() != current_c1)
            count++;
        else
            count--;

        last_c1 = current_c1;
        return true;
    }

    return false;
}

// Current position in encoder counts, 1400 per revolution of the output shaft.
// Note that int is 16 bit on AVR, so this wraps after about 23 revolutions.
int Encoder::position()
{
    return count;
}
