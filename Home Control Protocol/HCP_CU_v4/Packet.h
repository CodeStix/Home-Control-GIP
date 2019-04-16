#include "SoftwareSerial.h"

#ifndef Packet_h
#define Packet_h

class Packet
{
  public:
    Packet(unsigned char* data, unsigned char len);
    Packet(unsigned char slaveAddress, unsigned char masterAddress, unsigned char type, unsigned char* data, unsigned char len);
    unsigned char data[20];
    static unsigned char identifier;
    void sendViaSoftware(SoftwareSerial* ss);
    void printToSerial();
    char getIdentifier();
    unsigned char getCRC();
    unsigned char getSlave();
    unsigned char getMaster();
    unsigned char getType();
    unsigned char getLength();
    unsigned char* getData();
    unsigned char getCurrentCRC();
    bool hasValidIntegrity();
    
};

#endif
