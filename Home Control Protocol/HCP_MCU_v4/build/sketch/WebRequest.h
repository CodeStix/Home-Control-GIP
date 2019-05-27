#ifndef WebRequest_h
#define WebRequest_h

#include "WiFiClient.h"
#include "Arduino.h"

class WebRequest
{
    public:
        WebRequest(WiFiClient client);
        WiFiClient client;
        unsigned long receivedMillis;
        unsigned char requestId;
        String requested;
        void println(String str);
        void print(String str);
        void println(int i);
        void print(int i);
        void close();
        void update(bool(*funcRequester)(WebRequest*, String));
        bool shouldBeDisposed();

    private:
        String clientData;
        bool closed;
};

#endif