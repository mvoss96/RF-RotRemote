#pragma once
#include "radioMessage.h"

void radioInit();
void radioSleep();
bool radioSendMessage(RadioMessage msg);
bool radioSend(uint8_t* dataToSend, size_t size);
void radioWake();

