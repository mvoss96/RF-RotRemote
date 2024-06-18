#include <Arduino.h>
#include <LowPower.h>
#include "sleep.h"
#include "config.h"
#include "RF/radio.h"
#include "RF/power.h"

bool sleeping = false;

void checkForSleep()
{
    if (millis() - globalTimer > SLEEP_AFTER_MS)
    {
        sleeping = true;
        Serial.println(F("sleep"));
        //sendStatus();
        delay(10);
        radioSleep();
        Serial.flush();
        LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);

        Serial.println(F("woken up!"));
        updateVcc();
        radioWake();
        globalTimer = millis();
        sleeping = false;
    }
}