#ifndef Request_h
#define Request_h

#ifndef REQUEST_MAX_RESENDS
#define REQUEST_MAX_RESENDS 10
#endif

#ifndef REQUEST_TRY_INTERVAL
#define REQUEST_TRY_INTERVAL 350
#endif

#ifndef REQUEST_MAX_LIFETIME
#define REQUEST_MAX_LIFETIME 30000
#endif

#include "Packet.h"
#include "SoftwareSerial.h"
#include "Logger.h"

enum ResponseStatus
{
  NoResponse,
  Failed,
  Okay
};

class Request;

typedef void (*ResponseHandler)(ResponseStatus status, Request* requested);

class Request
{
    public:
        Request();
        Request(unsigned char fromAddress, ResponseHandler handler, unsigned char* data, unsigned char len, void* state = nullptr);
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
        void use(unsigned char fromAddress, ResponseHandler handler, unsigned char* data, unsigned char len, void* state = nullptr);
        void answered(unsigned char* respData, unsigned char respLen);
        void noAnswer();
        unsigned char sentData[20];
        unsigned char sentDataLength;
        unsigned char response[16];
        unsigned char responseLength;
        unsigned char resendTries;
        void* state;
};



#endif
