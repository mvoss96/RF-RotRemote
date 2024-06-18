#include <Arduino.h>
#include <RF24.h>

#include "config.h"
#include "radio.h"

static RF24 radio(PIN_RADIO_CE, PIN_RADIO_CSN);

static bool radioInitialized = false;
// static bool _radioInterruptReceived = false;
static const auto RADIO_DATARATE = RF24_250KBPS;
const uint8_t radioAddress[5] = WRITE_ADDRESS;

void radioInit()
{
    if (!radio.begin())
    {
        Serial.println("RF24Radio Connection Error!\n");
        return;
    }

    // pinMode(PIN_RADIO_IRQ, INPUT);
    //  let IRQ pin only trigger on "data_ready" event
    // radio.maskIRQ(true, true, false); // args = "data_sent", "data_fail", "data_ready"
    // attachInterrupt(digitalPinToInterrupt(PIN_RADIO_IRQ), radioInterrupt, FALLING);

    // Set the radio address
    // radioAddress[4] = address;

    radio.setChannel(RADIO_CHANNEL);     // Set the channel
    radio.setPALevel(RF24_PA_LOW);       // Adjust power level
    radio.setAddressWidth(5);            // Set address width
    radio.setCRCLength(RF24_CRC_16);     // Set CRC length
    radio.setRetries(5, 15);             // Set the number of retries and delay between retries
    radio.enableDynamicPayloads();       // Enable dynamic payloads
    radio.setDataRate(RADIO_DATARATE);   // Set data rate
    radio.openWritingPipe(radioAddress); // Set the writing pipe address
    radio.startListening();              // Start listening

    Serial.println("RF24Radio initialized!\n");
    radioInitialized = true;
}

void radioSleep()
{
    radio.powerDown();
}

void radioWake()
{
    radio.powerUp();
}

bool radioSendMessage(RadioMessage msg)
{
    Serial.print("Sending RadioMessage: ");
    if (!msg.getCreated())
    {
        Serial.println("RadioMessage not created!\n");
        return false;
    }
    msg.print();
    return radioSend(msg.getData(), msg.getSize());
}

bool radioSend(uint8_t *dataToSend, size_t size)
{
    if (!radioInitialized)
    {
        Serial.println("RF24Radio not initialized!\n");
        return false;
    }

    if (LED_BLINK_ON_MESSAGE)
    {
        digitalWrite(PIN_LED1, LOW);
    }
    Serial.print("Sending: ");
    for (size_t i = 0; i < size; i++)
    {
        Serial.print(dataToSend[i], HEX);
        Serial.print(" ");
    }

    radio.stopListening();
    bool result = radio.write(dataToSend, size);
    if (result)
    {
        Serial.println("-> Success");
    }
    else
    {
        Serial.println("-> Failed");
        if (LED_BLINK_ON_MESSAGE)
        {
            digitalWrite(PIN_LED2, LOW);
            delay(100);
            digitalWrite(PIN_LED2, HIGH);
        
        }
    }
    if (LED_BLINK_ON_MESSAGE)
    {
        digitalWrite(PIN_LED1, HIGH);
    }
    return result;
}