#include <Arduino.h>
#include <util/delay.h>
#include "digital_out.h"
#include "digital_in.h"
#include "encoder_part1_2.h"

int main()
{
  // Pins
  // D4: external LED, pulsed once per handled encoder state change
  // D2: encoder channel C1, the channel we count
  // D3: encoder channel C2, read at each C1 edge to resolve direction

  Digital_out led(4);  // External LED on D4 (PD4)
  Encoder motor(2, 3); // encoder_C1 on D2, encoder_C2 on D3

  Serial.begin(9600);
  led.init();
  motor.init();

  // Print every iteration, this is the Part 2 case
  const int print_interval = 1;

  int samples_since_print = 0;

  while (1)
  {
    // Sampling constraint 138us, 100us used for margin
    _delay_us(100);

    // Sample the encoder
    bool state_change = motor.update();

    // LED set HIGH on successfull state change
    // LED set LOW on no state change
    if (state_change == true)
    {
      led.set_hi();
    }
    else
    {
      led.set_lo();
    }

    // Print current position
    if (++samples_since_print >= print_interval)
    {
      samples_since_print = 0;
      Serial.println(motor.position());
    }
  }

  return 0;
}
