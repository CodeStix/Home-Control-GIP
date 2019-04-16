#include "Packet.h"
#include "SoftwareSerial.h"

#ifndef PacketSenderReceiver_h
#define PacketSenderReceiver_h

class PacketSenderReceiver
{
    public:
        PacketSenderReceiver(SoftwareSerial* serial);
        bool receiveAny(&Packet packet);
        SoftwareSerial* serial;

    private:
        bool receiving;
        unsigned char dataPosition;
        unsigned char incomingLength;
};

#endif