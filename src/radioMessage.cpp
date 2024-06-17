#include "radioMessage.h"

static uint8_t msgNum = 0;

void RadioMessage::print()
{
    Serial.print("RadioMessage: PV: ");
    Serial.print(PROTOCOL_VERSION, HEX);
    Serial.print(" UUID: ");
    for (size_t i = 0; i < sizeof(UUID); i++)
    {
        Serial.print(UUID[i], HEX);
        Serial.print(" ");
    }
    Serial.print(" MSG_NUM: ");
    Serial.print(MSG_NUM, HEX);
    Serial.print(" MSG_TYPE: ");
    Serial.print(MSG_TYPE, HEX);
    Serial.print(" DATA: ");
    for (size_t i = 0; i < dataSize; i++)
    {
        Serial.print(DATA[i], HEX);
        Serial.print(" ");
    }
    Serial.print("Checksum: ");
    Serial.print(DATA[dataSize], HEX);
    Serial.print(" ");
    Serial.println(DATA[dataSize + 1], HEX);
}

size_t RadioMessage::getSize()
{
    return sizeof(PROTOCOL_VERSION) + sizeof(UUID) + sizeof(MSG_NUM) + sizeof(MSG_TYPE) + dataSize;
}

bool RadioMessage::getCreated()
{
    return created;
}

bool RadioMessage::create(uint8_t *data, size_t size)
{
    if (size > sizeof(DATA) - 3)
    {
        Serial.println("Data too large for radioMessage");
        return false;
    }
    MSG_NUM = msgNum++;
    dataSize = size;
    memcpy(DATA, data, dataSize);
    calculateChecksum();
    created = true;
    return created;
}

uint8_t *RadioMessage::getData()
{
    return (uint8_t *)this;
}

void RadioMessage::calculateChecksum()
{
    // Calculate checksum and store it in the last two bytes of DATA
    uint16_t checksum = 0;
    checksum += PROTOCOL_VERSION;
    for (size_t i = 0; i < sizeof(UUID); i++)
    {
        checksum += UUID[i];
    }
    checksum += MSG_NUM;
    for (size_t i = 0; i < dataSize; i++)
    {
        checksum += DATA[i];
    }
    DATA[dataSize] = checksum & 0xFF;
    DATA[dataSize + 1] = (checksum >> 8) & 0xFF;
}

RemoteRadioMessage::RemoteRadioMessage(RemoteEvents event)
{
    MSG_TYPE = (uint8_t)MessageTypes::REMOTE;
    create((uint8_t *)&event, 1);
}
