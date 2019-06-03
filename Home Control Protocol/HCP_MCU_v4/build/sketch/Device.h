#ifndef Device_h
#define Device_h

#include "Print.h"

#define DEVICE_BYTE_SIZE 120

class Device
{
    public:
        Device(unsigned char fromBytes[118]);
        Device(unsigned char uniqueFactoryId[7], unsigned char address, char name[25]);
        char name[25];
        unsigned char uniqueFactoryId[7];
        unsigned char liveDeviceInfo[16];
        unsigned char knownProperties[64];
        unsigned char deviceType[4];
        unsigned char address;
        bool working;
        bool online;
        unsigned char* getBytes();
        void printTo(Print& dest);
        void printJSONTo(Print& dest);
};

#endif
