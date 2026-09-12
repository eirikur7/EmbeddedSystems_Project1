// Quadrature encoder driver, polled version. 
// Counts state changes on chanel C1 and reads the level of C2 at each C1 edge to decide which way the shaft is turning. 
// All pin access goes through Digital_in, so this file needs no register definitions of its own and the hardware stays hidden from the API.

#include "encoder_part1_2.h"

Encoder::Encoder(int pin_c1, int pin_c2) : c1(pin_c1), c2(pin_c2)
{
    count = 0;
    last_c1 = false; // placeholder, real value from init()
}

void Encoder::init()
{
    c1.init();
    c2.init();

    // We need this so update() can call back to last state to track changes
    last_c1 = c1.is_hi();
}

// Checks state of encoder, compares to last state.
// On state change it checks the direction and alters 
// the count correspondingly.
// Return true on state change and false if no change
bool Encoder::update()
{
    // Get current state
    bool current_c1 = c1.is_hi();

    // Any change on C1, rising or falling, is one counted pulse.
    if (current_c1 != last_c1)
    {
        // Check direction of shaft and change count respectively
        if (c2.is_hi() != current_c1)
            count++;
        else
            count--;

        last_c1 = current_c1;
        return true;
    }

    return false;
}

// Return current position of shaft
int Encoder::position()
{
    return count;
}
