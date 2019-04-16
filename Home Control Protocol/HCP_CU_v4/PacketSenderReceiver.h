#include "Packet.h"
#include "SoftwareSerial.h"
#include "Logger.h"

#ifndef PacketSenderReceiver_h
#define PacketSenderReceiver_h

class PacketSenderReceiver : public Logger
{
    public:
        PacketSenderReceiver(SoftwareSerial* serial, bool isSlave, unsigned char address);
        bool receiveAny(Packet* packet);
        bool receive(Packet* packet);
        void send(Packet packet);
        void send(unsigned char to, unsigned char* data, unsigned char len, PacketType type);
        void resendLastPacket();
        SoftwareSerial* serial;
        unsigned char address;
        bool isSlave;
        Packet lastSentPacket;

    private:
        bool receiving;
        unsigned char dataPosition;
        unsigned char incomingLength;
};

#endif
