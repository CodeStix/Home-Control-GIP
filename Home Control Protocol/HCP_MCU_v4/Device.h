#ifndef Device_h
#define Device_h

#ifndef PROPERTY_COUNT
#define PROPERTY_COUNT 64
#endif

class Device
{
    public:
        Device(unsigned char fromBytes[50]);
        Device(unsigned char uniqueFactoryId[7], unsigned char address, char name[25]);
        char name[25];
        unsigned char deviceInfo[8];
        unsigned char uniqueFactoryId[7];
        unsigned char knownProperties[PROPERTY_COUNT];
        unsigned char address;
        bool working;
        bool online;
        void printToSerial();
        unsigned char* getBytes();
};

#endif