#include <avr/io.h>
#include "digital_in.h"

Digital_in::Digital_in(int pin)
{
    pinMask = (1 << pin);
}

void Digital_in::init()
{
    DDRD &= ~pinMask;   // input
    PORTD |= pinMask;   // enable internal pullup
}

bool Digital_in::is_hi()
{
    return (PIND & pinMask) != 0;
}

bool Digital_in::is_lo()
{
    return !is_hi();
}
