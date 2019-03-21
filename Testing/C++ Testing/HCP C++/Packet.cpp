#include <stdio.h>
#include "Packet.h"

using namespace std;

unsigned char Packet::identifierByte = 0xf;
unsigned char Packet::currentIndex = 0;

Packet::Packet()
{
    this->isUsed = false;
}

Packet::Packet(unsigned char data[8])
{
    if (data[0] != identifierByte)
    {
        this->isValid = false;
        return;
    }

    this->index = data[1];
    this->from = data[2];
    this->too = data[3];
    this->data1 = data[4];
    this->data2 = data[5];
    this->data3 = data[6];
    this->data4 = data[7];

    this->isUsed = true;
}

Packet::Packet(unsigned char fromAddress, unsigned char tooAddress, unsigned char data1, unsigned char data2, unsigned char data3, unsigned char data4)
{
    this->isValid = true;

    this->index = Packet::currentIndex++;
    this->from = fromAddress;
    this->too = tooAddress;
    this->data1 = data1;
    this->data2 = data2;
    this->data3 = data3;
    this->data4 = data4;

    this->isUsed = true;
}

char* Packet::to_string()
{
    static char str[50];

    sprintf(str, "(Index, %d, From: %d, Too: %d, Data: %x %x %x %x)", this->index, this->from, this->too, this->data1, this->data2, this->data3, this->data4);
    
    return str;
    //printf("(From: %d, Too: %d, Data: %x %x %x %x)", this->from, this->too, this->data1, this->data2, this->data3, this->data4);
}

unsigned char* Packet::get_bytes()
{
    static unsigned char data[8];

    data[0] = identifierByte;
    data[1] = this->index;
    data[2] = this->from;
    data[3] = this->too;
    data[4] = this->data1;
    data[5] = this->data2;
    data[6] = this->data3;
    data[7] = this->data4;

    return data;
}