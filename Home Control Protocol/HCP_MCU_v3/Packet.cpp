#include "Packet.h"

#ifndef NO_ARDUINO
#include "Arduino.h"
#else
#include <stdio.h>
using namespace std;
#endif

unsigned char Packet::identifierByte = 0xf;

Packet::Packet()
{
    this->isValid = false;
}

Packet::Packet(unsigned char data[8])
{
    if (data[0] != identifierByte)
    {
        this->isValid = false;
        return;
    }

    this->isValid = true;
    this->index = data[1];
    this->from = data[2];
    this->to = data[3];
    this->data1 = data[4];
    this->data2 = data[5];
    this->data3 = data[6];
    this->data4 = data[7];
}

Packet::Packet(unsigned char fromAddress, unsigned char toAddress, unsigned char index, unsigned char data1, unsigned char data2, unsigned char data3, unsigned char data4)
{
    this->isValid = true;

    this->index = index;
    this->from = fromAddress;
    this->to = toAddress;
    this->data1 = data1;
    this->data2 = data2;
    this->data3 = data3;
    this->data4 = data4;
}

char* Packet::to_string()
{
#ifdef NO_ARDUINO
    static char str[50];

    sprintf(str, "(Index: %d, From: %d, Too: %d, Data: %x %x %x %x)", this->index, this->from, this->to, this->data1, this->data2, this->data3, this->data4);
    
    return str;
#else
    return "";
#endif
    //printf("(From: %d, Too: %d, Data: %x %x %x %x)", this->from, this->too, this->data1, this->data2, this->data3, this->data4);
}

unsigned char* Packet::get_bytes()
{
    static unsigned char data[8];

    data[0] = identifierByte;
    data[1] = this->index;
    data[2] = this->from;
    data[3] = this->to;
    data[4] = this->data1;
    data[5] = this->data2;
    data[6] = this->data3;
    data[7] = this->data4;

    return data;
}