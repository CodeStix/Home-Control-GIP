#ifndef WebRequest_h
#define WebRequest_h

#include "WiFiClient.h"
#include "Arduino.h"

class WebRequest
{
    public:
        WebRequest(WiFiClient client);
        bool alive;
        WiFiClient client;
        unsigned long receivedMillis;
        String requested;
        void println(String str);
        void print(String str);
        void close();
};

#endif