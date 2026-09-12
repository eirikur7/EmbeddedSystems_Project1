#include <Arduino.h>
#include "encoder.h"

int main()
{
  // Pins
  // D2: encoder channel C1, on INT0 so its edges trigger the interrupt
  // D3: encoder channel C2, read in the handler to resolve direction
  // D4: external LED, high for the duration of each interrupt

  Encoder motor(2, 3, 4); // encoder_C1 on D2, encoder_C2 on D3, LED on D4

  Serial.begin(9600);
  motor.init();

  // No sampling loop and no delay, the interrupt does the counting.
  // Printing on every iteration is the same serial load that broke Part 2.
  while (1)
  {
    Serial.println(motor.position());
  }

  return 0;
}
