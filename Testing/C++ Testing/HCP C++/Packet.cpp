#include "Packet.h"

unsigned char Packet::identifierByte = 0xf;
unsigned char Packet::currentIndex = 0;

/// Create a new packet from raw data.
Packet::Packet(unsigned char fromAddress, unsigned char toAddress, unsigned char data1, unsigned char data2, unsigned char data3, unsigned char data4)
{
    this->creationMillis = 0;//millis();

    this->index = currentIndex++;
    this->fromAddress = fromAddress;
    this->toAddress = toAddress;
    this->data1 = data1;
    this->data2 = data2;
    this->data3 = data3;
    this->data4 = data4;

    this->isValid = true;
    this->responded = false;
}

/// Create a packet from received bytes. 
Packet::Packet(unsigned char data[8])
{
    this->creationMillis = 0;//millis();

    if (data[0] != Packet::identifierByte)
    {
        this->isValid = false;
        return;
    }

    this->index = data[1];
    this->fromAddress = data[2];
    this->toAddress = data[3];
    this->data1 = data[4];
    this->data2 = data[5];
    this->data3 = data[6];
    this->data4 = data[7];

    this->isValid = true;
    this->responded = false;
}

/// Converts this packet to its appropriate bytes representation.
unsigned char * Packet::getBytes()
{
    unsigned char data[8];

    data[0] = Packet::identifierByte;
    data[1] = this->index;
    data[2] = this->fromAddress;
    data[3] = this->toAddress;
    data[4] = this->data1;
    data[5] = this->data2;
    data[6] = this->data3;
    data[7] = this->data4;

    return data;
}

int Packet::getCreationMillis()
{
    return this->creationMillis;
}

bool Packet::getIsValid()
{
    return this->isValid;
}