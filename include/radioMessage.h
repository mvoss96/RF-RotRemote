#pragma once
#include <Arduino.h>
#include "config.h"

enum class MessageTypes
{
    EMPTY,
    REMOTE,
};

enum class RemoteEvents
{
    EMPTY,
    ON,
    OFF,
    TOGGLE,
    UP1,
    DOWN1,
    UP2,
    DOWN2,
};

class RadioMessage
{
    protected:
        uint8_t PROTOCOL_VERSION =  0x01;
        uint8_t UUID[4] = DEVICE_UUID;
        uint8_t MSG_NUM = 0;
        uint8_t MSG_TYPE = (uint8_t)MessageTypes::EMPTY;
        uint8_t DATA[25] = {0xFF}; // Last two bytes are checksum

        bool created = false;
        size_t dataSize = 0;
        void calculateChecksum();
    public:
        RadioMessage() = default;
        void print();
        size_t getSize();
        bool getCreated();
        bool create(uint8_t *data, size_t size);
        uint8_t *getData();
};

class RemoteRadioMessage : public RadioMessage
{
    private:
        uint8_t EVENT = (uint8_t)RemoteEvents::EMPTY;
    public:
        RemoteRadioMessage(RemoteEvents event);
};
