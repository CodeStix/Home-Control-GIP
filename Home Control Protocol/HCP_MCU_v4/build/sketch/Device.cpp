#include "Device.h"
#include "Arduino.h"

Device::Device(unsigned char fromBytes[118])
{
    memcpy(this->name, &fromBytes[0], 25);
    memcpy(this->liveDeviceInfo, &fromBytes[25], 16);
    memcpy(this->uniqueFactoryId, &fromBytes[41], 7);
    memcpy(this->knownProperties, &fromBytes[48], 64);
    memcpy(&this->deviceType, &fromBytes[112], 4);
    this->address = fromBytes[116];
    this->working = (fromBytes[117] & 0x1) == 0x1;
    this->online = (fromBytes[117] & 0x2) == 0x2;   
}

Device::Device(unsigned char uniqueFactoryId[7], unsigned char address, char name[25])
{
    memcpy(this->name, name, sizeof(this->name));
    memset(this->liveDeviceInfo, 0, sizeof(this->liveDeviceInfo));
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
    for(unsigned char i = 0; i < 4; i++)
    {
        Serial.print(this->deviceType[i]);
        Serial.print(' ');
    }
    Serial.print(this->working ? "" : ", NOT WORKING");
    Serial.print(this->online ? ", ONLINE" : ", OFFLINE");
    Serial.print(')');
}

unsigned char* Device::getBytes()
{
    static unsigned char bytes[118];

    memset(bytes, 0x0, sizeof(bytes));
    memcpy(&bytes[0], this->name, 25);
    memcpy(&bytes[25], this->liveDeviceInfo, 16);
    memcpy(&bytes[41], this->uniqueFactoryId, 7);
    memcpy(&bytes[48], this->knownProperties, 64);
    memcpy(&bytes[112], &this->deviceType, 4);
    bytes[116] = this->address;
    bytes[117] |= this->working ? 0x1 : 0x0;
    bytes[117] |= this->online ? 0x2 : 0x0;

    return bytes;
}