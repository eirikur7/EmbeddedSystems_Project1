// encoder.cpp
// Interrupt driven quadrature encoder driver.
//
// C1 is wired to D2, which is the ATmega328's INT0 pin. INT0 is configured to
// fire on any logical change, so the handler runs on both the rising and the
// falling edge of C1. That is where the factor of 2 in the 1400 counts per
// revolution comes from. C2 is read inside the handler to resolve direction.
//
// This file is the only place that knows about interrupt registers, so the API
// in encoder.h stays free of hardware detail.

#include <avr/io.h>
#include <avr/interrupt.h>
#include "encoder.h"

// An interrupt handler is a plain function with a fixed name, so it cannot be a
// class member. init() records the active instance here and the handler calls
// through it.
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

    // INT0 sense control. Setting ISC00 on its own selects "any logical
    // change", so both edges of C1 trigger the handler. Setting ISC01 as well
    // would select a single edge and halve the resolution.
    EICRA |= (1 << ISC00);
    EICRA &= ~(1 << ISC01);

    // Enable INT0, then enable interrupts globally. The global enable is
    // needed because main() does not call the Arduino core's init().
    EIMSK |= (1 << INT0);
    sei();
}

// Runs on every edge of C1.
//
// The hardware has already detected the edge, so unlike the polled version
// there is no stored previous level to compare against. C1's level now is its
// level after the edge, and C2's level at this instant gives the direction.
void Encoder::handle_edge()
{
    activity.set_hi(); // rising edge marks the response time

    if (c2.is_hi() != c1.is_hi())
        count++;
    else
        count--;

    activity.set_lo(); // pulse width is the handler's execution time
}

// count is 16 bit, so reading it takes two instructions. An interrupt landing
// between them would return a torn value, half old and half new. Interrupts
// are disabled across the read and the previous state is restored afterwards,
// rather than calling sei() blindly, so this is still safe if it is ever
// called with interrupts already disabled.
int Encoder::position()
{
    uint8_t saved = SREG;
    cli();
    int value = count;
    SREG = saved;
    return value;
}
