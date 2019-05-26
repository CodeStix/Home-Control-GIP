#ifndef Device_h
#define Device_h

class Device
{
    public:
        Device(unsigned char fromBytes[104]);
        Device(unsigned char uniqueFactoryId[7], unsigned char address, char name[25]);
        char name[25];
        unsigned char uniqueFactoryId[7];
        unsigned char liveDeviceInfo[16];
        unsigned char knownProperties[64];
        unsigned int deviceType;
        unsigned char address;
        bool working;
        bool online;
        void printToSerial();
        unsigned char* getBytes();
};

#endif