#include <Arduino.h>
#include <NewEncoder.h>

#include "encoder.h"
#include "config.h"
#include "radio.h"

static volatile bool btnPressed = false;
static NewEncoder encoder(2, 3, -20, 20, 0, FULL_PULSE);
static bool wasRotated = false;

ISR(PCINT2_vect)
{
    btnPressed = !digitalRead(PIN_BTN_RST);
}

void encoderSetup()
{
    NewEncoder::EncoderState state;
    if (!encoder.begin())
    {
        Serial.println(F("Encoder Failed to Start. Check pin assignments and available interrupts. Aborting."));
        while (1)
        {
            yield();
        }
    }
}

static void rotLeft()
{
    Serial.print(F("left "));
    wasRotated = true;
    globalTimer = millis();
    bool pressed = !digitalRead(PIN_BTN_ENC);
    Serial.println(pressed);
    radioSend();
}

static void rotRight()
{
    Serial.print(F("right "));
    wasRotated = true;
    globalTimer = millis();
    bool pressed = !digitalRead(PIN_BTN_ENC);
    Serial.println(pressed);
    radioSend();
}

static void rotClick()
{
    Serial.println(F("click "));
    globalTimer = millis();
    radioSend();
}

static void rotDoubleClick()
{
    Serial.println(F("doubleclick "));
    globalTimer = millis();
    radioSend();
}

static void btnRead()
{
    static bool lastPin = HIGH;
    static bool clicked = false;
    static unsigned long lastHigh = 0;
    static unsigned long lastLow = 0;
    static unsigned long lastClick = 0;

    if (clicked && wasRotated == false && millis() - lastClick >= DOUBLE_CLICK_MS)
    {
        clicked = false;
        rotClick();
    }

    if (digitalRead(PIN_BTN_ENC) == LOW && lastPin == HIGH)
    {
        if (millis() - lastHigh <= DEBOUNCE_MS)
        {
            return;
        }
        // Serial.println("pressed");
        clicked = false;
        globalTimer = millis();
        lastPin = LOW;
        lastLow = millis();
        wasRotated = false;
    }
    else if (digitalRead(PIN_BTN_ENC) == HIGH && lastPin == LOW)
    {
        if (millis() - lastLow < DEBOUNCE_MS)
        {
            return;
        }
        lastHigh = millis();
        lastPin = HIGH;
        if (millis() - lastLow > LONG_PRESS_MS && wasRotated == false)
        {
            Serial.println(F("Long press"));
            return;
        }
        if (millis() - lastClick < DOUBLE_CLICK_MS)
        {
            rotDoubleClick();
            clicked = false;
        }
        else
        {
            // Serial.print("released ");
            // Serial.println(millis() - lastLow);
            clicked = true;
            globalTimer = millis();
            lastClick = millis();
        }
    }
}

void encoderRead()
{
    NewEncoder::EncoderState state;
    if (encoder.getState(state))
    {
        Serial.print(F("Encoder: "));
        globalTimer = millis();
        switch (state.currentClick)
        {
        case NewEncoder::UpClick:
            rotRight();
            break;

        case NewEncoder::DownClick:
            rotLeft();
            break;

        default:
            break;
        }
    }
    btnRead();
}