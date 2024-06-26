#pragma once
#include <Arduino.h>

#define VREFINT                                  1113           // In mV
#define BATTERY_FULL_VOLTAGE                     3000           // In mV
#define BATTERY_EMPTY_VOLTAGE                    1800           // In mV
#define LED_BLINK_ON_MESSAGE                        1           // Blink LEDs on messages
#define RADIO_CHANNEL                             100           // Radio channel   0-125
#define DEVICE_NAME                     "RF-RotRemote"          // Device name
#define WRITE_ADDRESS   {0x46, 0x36, 0x31, 0x38, 0x00}          // Radio address (5 bytes)
#define DEVICE_UUID                 {'M', 'V', 'R', 2}          // Device UUID  (4 bytes)

// Pin definitions
#define PIN_RADIO_CE                                9       
#define PIN_RADIO_CSN                              10     
#define PIN_LED1                                   A1     
#define PIN_LED2                                   A0
#define PIN_ROTARY1                                 3
#define PIN_ROTARY2                                 2
#define PIN_BTN_ENC                                 4
#define PIN_BTN_RST                                A4
#define PIN_BTN_LISTEN                              5                             
#define DEBOUNCE_MS                                40
#define LONG_PRESS_MS                             500
#define DOUBLE_CLICK_MS                           200
#define SLEEP_AFTER_MS                           1500
#define CLICKS_PER_STEP                             4

extern unsigned long globalTimer;
