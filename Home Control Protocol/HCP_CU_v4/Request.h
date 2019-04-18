#ifndef Request_h
#define Request_h

#ifndef REQUEST_MAX_RESENDS
#define REQUEST_MAX_RESENDS 10
#endif

#ifndef REQUEST_TRY_INTERVAL
#define REQUEST_TRY_INTERVAL 200
#endif

#ifndef REQUEST_MAX_LIFETIME
#define REQUEST_MAX_LIFETIME 30000
#endif

#include "Packet.h"
#include "SoftwareSerial.h"
#include "Logger.h"

typedef void (*ResponseHandler)(unsigned char* respData, unsigned char respLen);

class Request
{
    public:
        Request();
        Request(unsigned char fromAddress, ResponseHandler handler, unsigned char* data, unsigned char len);
        ResponseHandler handler;
        unsigned char fromAddress;
        unsigned char id;
        unsigned long sentMillis;
        unsigned long createdMillis;
        bool gotAnswered;
        static unsigned char currentId;
        bool shouldGetResend();
        bool used;
        bool mayGetDisposed();
        void use(unsigned char fromAddress, ResponseHandler handler, unsigned char* data, unsigned char len);
        void answered(unsigned char* respData, unsigned char respLen);
        unsigned char sentData[20];
        unsigned char sentDataLength;
        unsigned char resendTries;
};

#endif
