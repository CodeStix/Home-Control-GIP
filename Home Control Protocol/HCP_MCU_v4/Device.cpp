#include "Device.h"
#include "Arduino.h"
#include "Print.h"

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

void Device::printTo(Print& dest)
{
    dest.print("(address: ");
    dest.print(this->address);
    dest.print(", name: ");
    dest.print(this->name);
    dest.print(" (");
    dest.print(this->working ? "" : "NOT WORKING, ");
    dest.print(this->online ? "ONLINE" : "OFFLINE");
    dest.print("), ufid: ");
    for(unsigned char i = 0; i < 7; i++)
    {
        dest.print(this->uniqueFactoryId[i], DEC);
        dest.print(' ');
    }
    dest.print(", type: ");
    for(unsigned char i = 0; i < 4; i++)
    {
        dest.print(this->deviceType[i]);
        dest.print(' ');
    }
    dest.print(", live: ");
    for(unsigned char i = 0; i < 16; i++)
    {
        dest.print(this->liveDeviceInfo[i]);
        dest.print(' ');
    }
    dest.print(", prop: ");
    for(unsigned char i = 0; i < 64; i++)
    {
        if (this->knownProperties[i] != 0x0)
        {
            dest.print(i);
            dest.print('=');
            dest.print(this->knownProperties[i]);
            dest.print(' ');
        }
    }
    dest.print(')');
}

void Device::printJSONTo(Print& dest)
{
    // [0]: name
    dest.print("{\n\"name\":\"");
    dest.print(this->name);
    dest.print("\",\n\"address\":");
    // [1]: address
    dest.print(this->address);
    dest.print(",\n\"ufid\": [");
    // [2]: uniqueFactoryId
    for(unsigned char j = 0; j < 7; j++)
    {
        if (j != 0)
            dest.print(',');
        dest.print(this->uniqueFactoryId[j]);
    }
    dest.print("],\n\"type\": [");
    // [3]: deviceType
    for(unsigned char j = 0; j < 4; j++)
    {
        if (j != 0)
            dest.print(',');
        dest.print(this->deviceType[j]);
    }
    dest.print("],\n\"knownProperties\": [");
    // [4]: knownProperties
    for(unsigned char j = 0; j < 64; j++)
    {
        if (j != 0)
            dest.print(',');
        dest.print(this->knownProperties[j]);
    }
    dest.print("],\n\"liveData\": [");
    // [5]: liveDeviceInfo
    for(unsigned char j = 0; j < 16; j++)
    {
        if (j != 0)
            dest.print(',');
        dest.print(this->liveDeviceInfo[j]);
    }
    dest.print("],\n\"online\": ");
    // [6]: online
    dest.print(this->online ? "true" : "false");
    dest.print(",\n\"working\": ");
    // [7]: working
    dest.print(this->working ? "true" : "false");
    dest.print("\n}");
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