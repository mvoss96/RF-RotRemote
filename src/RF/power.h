#pragma once
#include <Arduino.h>
#include "config.h"


void updateVcc();
uint16_t getBatteryVoltage();
uint8_t getBatteryLevel();
void printPowerStatus();
