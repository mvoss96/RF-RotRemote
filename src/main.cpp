#include <Arduino.h>

void setup()
{
  Serial.begin(115200);

  // Enable interrupt on PIN_BTN1
  PCICR |= (1 << PCIE1);   // Aktivate Interrupts on Port C (analog)
  PCMSK1 |= (1 << PCINT4); // Aktivate Pin Change ISR for A4
}

void loop()
{
}

// put function definitions here:
int myFunction(int x, int y)
{
}