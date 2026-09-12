#include <Arduino.h>
#include <util/delay.h>
#include "digital_out.h"
#include "digital_in.h"
#include "encoder.h"

int main()
{
  // Pins
  // D4: external LED, pulsed once per handled encoder state change
  // D2: encoder channel C1, the channel we count
  // D3: encoder channel C2, read at each C1 edge to resolve direction

  Digital_out led(4);  // External LED on D4 (PD4)
  Encoder motor(2, 3); // C1 on D2, C2 on D3

  Serial.begin(9600);
  led.init();
  motor.init();

  // Part 2. Printing on every single sample. At 9600 baud one line takes
  // several milliseconds to transmit and the write blocks, so the UART, not
  // the encoder, now sets the sample period. This is the case the assignment
  // asks us to examine.
  const int print_interval = 1;

  int samples_since_print = 0;

  while (1)
  {
    // C1 carries at most 2100 edges/sec by the datasheet calculation, so C1
    // edges are 476 us apart and counting alone would only need a sample every
    // 476 us. Direction is the tighter constraint: the C1/C2 pair advances
    // through four states per cycle, changing every 238 us, and reading C2
    // later than that inverts the direction. 238 us is a failure boundary
    // rather than a target, so we sample well inside it.
    // 100 us is about 10 kHz, comfortably faster than required.
    _delay_us(100);

    // Sample the encoder. True when a C1 state change was found and handled.
    bool state_change = motor.update();

    // Pulse the LED to signal the state change has been handled.
    // This pin is the scope probe point for measuring response time.
    if (state_change == true)
    {
      led.set_hi();
    }
    else
    {
      led.set_lo();
    }

    // Report the current position count.
    if (++samples_since_print >= print_interval)
    {
      samples_since_print = 0;
      Serial.println(motor.position());
    }
  }

  return 0;
}
