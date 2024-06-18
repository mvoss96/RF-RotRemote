#include <Arduino.h>

#include "encoder.h"
#include "sleep.h"
#include "config.h"
#include "RF/power.h"
#include "RF/radio.h"

unsigned long globalTimer;

void setPinModes()
{
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);

  pinMode(PIN_BTN_RST, INPUT_PULLUP);
  pinMode(PIN_BTN_ENC, INPUT_PULLUP);
  pinMode(PIN_ROTARY1, INPUT_PULLUP);
  pinMode(PIN_ROTARY2, INPUT_PULLUP);
  pinMode(PIN_BTN_LISTEN, INPUT_PULLUP);

  // Enable interrupt on PIN_BTN1 (4)
  PCICR |= 0b00000100;  // turn on port D
  PCMSK2 |= 0b00010000; // turn on pin 4
}

void printGreeting()
{
  uint8_t uuid[] = DEVICE_UUID;
  Serial.printf("%s %02X:%02X:%02X:%02X\n", DEVICE_NAME, uuid[0], uuid[1], uuid[2], uuid[3]);
  updateVcc();
  printPowerStatus();
}

void setup()
{
  setPinModes();
  Serial.begin(115200);
  delay(100);
  printGreeting();
  encoderSetup();
  radioInit();

  // Blink LEDs
  digitalWrite(PIN_LED1, LOW);
  digitalWrite(PIN_LED2, LOW);
  delay(500);
  digitalWrite(PIN_LED1, HIGH);
  digitalWrite(PIN_LED2, HIGH);

  globalTimer = millis();
}

void loop()
{
  checkForSleep();
  encoderRead();
}
