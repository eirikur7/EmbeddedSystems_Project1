#include <Arduino.h>
#include "encoder.h"

int main()
{
  // Pins
  // D2: encoder channel C1, wired to INT0 so its edges trigger an interrupt
  // D3: encoder channel C2, read inside the handler to resolve direction
  // D4: external LED, high for the duration of each interrupt

  Encoder motor(2, 3, 4);

  Serial.begin(9600);
  motor.init();

  // No sampling loop and no delay. The encoder is driven entirely by the
  // interrupt, so the main loop only has to report the count.
  //
  // Printing flat out at 9600 baud is the heaviest serial load we can apply.
  // This is the same condition that destroyed the count in Part 2, where the
  // blocking UART write set the sample period to 7.3 ms.
  while (1)
  {
    Serial.println(motor.position());
  }

  return 0;
}
