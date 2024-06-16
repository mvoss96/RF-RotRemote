#include <Arduino.h>
#include <RF24.h>
#include "config.h"
#include "radio.h"

static RF24 radio(PIN_RADIO_CE, PIN_RADIO_CSN);

static bool radioInitialized = false;
static bool _radioInterruptReceived = false;
static const auto RADIO_DATARATE = RF24_250KBPS;
uint8_t radioAddress[5] = {0x99, 0x6C, 0xCA, 0x80, 0x01};
char dataToSend[10] = "Hello";

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

    radio.setChannel(100);               // Set the channel
    radio.setPALevel(RF24_PA_LOW);       // Adjust power level
    radio.setAddressWidth(5);            // Set address width
    radio.setCRCLength(RF24_CRC_16);     // Set CRC length
    radio.setRetries(5, 15);             // Set the number of retries and delay between retries
    radio.enableDynamicPayloads();       // Enable dynamic payloads
    radio.enableAckPayload();            // Enable ack payloads
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

void radioSend()
{
    radio.stopListening();
    bool result = radio.write(&dataToSend, sizeof(dataToSend));
    Serial.print("Sending data: ");
    Serial.print(dataToSend);
    if (result)
    {
        Serial.print(" -> Acknowledgment received");
    }
    else
    {
        Serial.print(" -> Acknowledgment not received");
    }
    Serial.println();
}

void radioWake()
{
    radio.powerUp();
}