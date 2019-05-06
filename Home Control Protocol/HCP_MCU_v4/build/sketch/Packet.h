#ifndef Packet_h
#define Packet_h

#include "SoftwareSerial.h"
#include "Arduino.h"

enum PacketType
{
  DataRequest,
  Push,
  Answer,
  PleaseResend
};

class Packet
{
  public:
    Packet();
    Packet(unsigned char* data, unsigned char len = 20);
    Packet(unsigned char slaveAddress, unsigned char masterAddress, unsigned char* data, unsigned char len, PacketType type, unsigned char multiPurposeByte = 0x0);
    unsigned char data[20];
    static unsigned char identifier;
    void sendViaSoftware(SoftwareSerial* ss);
    void printToSerial();
    char getIdentifier();
    unsigned char getCRC();
    unsigned char getSlave();
    unsigned char getMaster();
    unsigned char getRawType();
    PacketType getType();
    unsigned char getDataLength();
    unsigned char* getData();
    unsigned char getMultiPurposeByte();
    unsigned char getCurrentCRC();
    bool hasValidIntegrity();
    void recalculateCRC();
    bool needsResponse();
};

#endif
