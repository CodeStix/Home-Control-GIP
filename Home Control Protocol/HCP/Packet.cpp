#include "Packet.h"
#include "Arduino.h"

Packet::Packet(byte fromAddress, byte toAddress, byte data)
{
    this->creationMillis = millis();
    this->fromAddress = fromAddress;
    this->toAddress = toAddress;
    this->data = data;
    this->index = currentIndex++;
}