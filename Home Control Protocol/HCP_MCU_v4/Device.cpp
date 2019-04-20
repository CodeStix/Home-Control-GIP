#include "Device.h"
#include "Arduino.h"

Device::Device(unsigned char fromBytes[50])
{
    memcpy(this->name, &fromBytes[0], 25);
    memcpy(this->deviceInfo, &fromBytes[25], 8);
    memcpy(this->uniqueFactoryId, &fromBytes[33], 7);
    this->address = fromBytes[40];
    this->working = (fromBytes[41] & 0x1) == 0x1;
    this->online = (fromBytes[41] & 0x2) == 0x2;
}

Device::Device(unsigned char uniqueFactoryId[7], unsigned char address, char name[25])
{
    memcpy(this->name, name, sizeof(this->name));
    memset(this->deviceInfo, 0, sizeof(this->deviceInfo));
    memcpy(this->uniqueFactoryId, uniqueFactoryId, 7);
    this->address = address;
    this->working = false;
    this->online = false;
}

void Device::printToSerial()
{
    Serial.print("(address: ");
    Serial.print(this->address);
    Serial.print(", name: ");
    Serial.print(this->name);
    Serial.print(", ufid: ");
    for(unsigned char i = 0; i < 7; i++)
    {
        Serial.print(this->uniqueFactoryId[i], DEC);
        Serial.print(' ');
    }
    Serial.print(", info: ");
    for(unsigned char i = 0; i < 8; i++)
    {
        Serial.print(this->deviceInfo[i]);
        Serial.print(' ');
    }
    Serial.print(this->working ? "" : ", NOT WORKING");
    Serial.print(this->online ? ", ONLINE" : ", OFFLINE");
    Serial.print(')');
}

unsigned char* Device::getBytes()
{
    static unsigned char bytes[50];

    memset(bytes, 0x0, sizeof(bytes));
    memcpy(&bytes[0], this->name, 25);
    memcpy(&bytes[25], this->deviceInfo, 8);
    memcpy(&bytes[33], this->uniqueFactoryId, 7);
    bytes[40] = this->address;
    bytes[41] |= this->working ? 0x1 : 0x0;
    bytes[41] |= this->online ? 0x2 : 0x0;

    return bytes;
}