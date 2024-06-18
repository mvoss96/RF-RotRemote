#include "power.h"

static uint16_t batteryVoltage = 0;
static uint8_t batteryLevel = 0;

inline static uint16_t readVcc()
{
    uint16_t result; // Variable to hold the result

    // Configure the ADC to do a conversion with AVcc as the reference, and the internal 1.1V voltage reference as the input.
    // _BV(REFS0) sets the reference voltage to AVcc.
    // _BV(MUX3) | _BV(MUX2) | _BV(MUX1) sets the input to the internal 1.1V reference.
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);

    delay(2); // Wait for Vref to settle after changing the reference voltage or input

    // Start an ADC conversion. _BV(ADSC) sets the ADSC bit in the ADCSRA register, which starts a conversion.
    ADCSRA |= _BV(ADSC);

    // Wait for the conversion to complete. The ADSC bit in the ADCSRA register remains high while a conversion is in progress,
    // and goes low when the conversion is complete.
    while (bit_is_set(ADCSRA, ADSC))
        ;

    // Read the low 8 bits of the conversion result.
    result = ADCL;

    // Read the high 2 bits of the conversion result and add them to the result.
    // The bit shift operation (ADCH<<8) moves the high bits to the correct position in the result.
    result |= ADCH << 8;

    // Back-calculate AVcc in mV. The constant  is calculated as follows: 1.1V (internal reference voltage) * 1023 (max ADC reading) * 1000 (conversion from V to mV).
    // The result of the ADC conversion is divided into this constant to calculate AVcc.
    result = (VREFINT * 1023L) / result;

    // Return the supply voltage in mV.
    return result;
}

void updateVcc()
{
    batteryVoltage = readVcc();
    // Ensure that the voltage is within the range from emptyVoltage to fullVoltage
    // Map the constrained voltage to the range from 1 to 255
    batteryLevel = map(constrain(batteryVoltage, BATTERY_EMPTY_VOLTAGE, BATTERY_FULL_VOLTAGE), BATTERY_EMPTY_VOLTAGE, BATTERY_FULL_VOLTAGE, 1, 255);
}

void printPowerStatus()
{
  Serial.print("VCC: ");
  Serial.print(batteryVoltage);
  Serial.print("mV ");
  Serial.print((batteryLevel*100)/255);
  Serial.println("%");
}

uint16_t getBatteryVoltage()
{
    return batteryVoltage;
}

uint8_t getBatteryLevel()
{
    return batteryLevel;
}

