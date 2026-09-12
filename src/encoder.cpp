// Quadrature encoder driver, interrupt version.
// C1 sits on D2 (INT0) and triggers on both edges, so the resolution stays at 1400 counts per revolution.
// C2 is read inside the handler to decide which way the shaft is turning.
// The interrupt registers are only touched in this file, so the API stays free of hardware detail.

#include <avr/io.h>
#include <avr/interrupt.h>
#include "encoder.h"

static Encoder *isr_target = 0;

ISR(INT0_vect)
{
    if (isr_target)
        isr_target->handle_edge();
}

Encoder::Encoder(int pin_c1, int pin_c2, int pin_led)
    : c1(pin_c1), c2(pin_c2), activity(pin_led)
{
    count = 0;
}

void Encoder::init()
{
    c1.init();
    c2.init();
    activity.init();

    isr_target = this;

    // Trigger on any change, so both edges of C1 are counted.
    EICRA |= (1 << ISC00);
    EICRA &= ~(1 << ISC01);

    // enable INT0
    EIMSK |= (1 << INT0);

    // enable interrupts, main() skips the Arduino init()
    sei();
}

// Runs on every edge of C1.
void Encoder::handle_edge()
{
    activity.set_hi(); // LED rising edge marks the response time

    // Check direction of shaft and change count respectively
    if (c2.is_hi() != c1.is_hi())
        count++;
    else
        count--;

    activity.set_lo(); // LED pulse width is how long the handler took
}

// Return current position of shaft.
// count is 16 bit, so reading it takes two instructions. Interrupts are turned
// off across the read so an interrupt cannot land between them and return half
// an old value and half a new one.
int Encoder::position()
{
    uint8_t saved = SREG;
    cli();
    int value = count;
    SREG = saved;
    return value;
}
