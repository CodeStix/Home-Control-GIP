#ifndef PacketSenderReceiver_h
#define PacketSenderReceiver_h

#ifndef MAX_CONCURRENT_REQUESTS
#define MAX_CONCURRENT_REQUESTS 10
#endif

#include "Request.h"
#include "Packet.h"
#include "SoftwareSerial.h"
#include "Logger.h"
#include "Arduino.h"

class PacketSenderReceiver : public Logger
{
    public:
        PacketSenderReceiver(SoftwareSerial* serial, bool isSlave, unsigned char address);
        bool receiveAny(Packet* packet);
        bool receive(Packet* packet);
        void send(Packet packet);
        void send(unsigned char to, unsigned char* data, unsigned char len, PacketType type, unsigned char multiPurposeByte = 0x0);
        void broadcast(unsigned char* data, unsigned char len, PacketType type, unsigned char multiPurposeByte = 0x0);
        void resendLastPacket();
        SoftwareSerial* serial;
        unsigned char address;
        bool isSlave;
        Packet lastSentPacket;
        unsigned long lastSentMillis;
        
        Request requests[MAX_CONCURRENT_REQUESTS];
        unsigned char sendRequest(Request* request);
        unsigned char sendRequest(unsigned char to, ResponseHandler handler, unsigned char* data, unsigned char len);
        void resendUnansweredRequests();
        Request* getRequestWithId(unsigned char id);    
        void answer(Packet* toAnswer, unsigned char* respData, unsigned char respLen);

    private:
        bool receiving;
        unsigned char dataPosition;
        unsigned char incomingLength;
        Request* getNewRequest(unsigned char fromAddress, ResponseHandler handler, unsigned char* data, unsigned char len);
};

#endif
