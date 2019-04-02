#include "Packet.cpp"
#include "Request.cpp"

#ifndef NO_ARDUINO
#include "Arduino.h"
#endif

#ifndef MAX_CONCURRENT_REQUESTS
#define MAX_CONCURRENT_REQUESTS 16
#endif

class HCPClient
{
    public:
        #ifndef NO_ARDUINO
        HCPClient(unsigned char my_address, int rxPin, int txPin);
        #else
        HCPClient(unsigned char my_address);  
        #endif

        Request pending[MAX_CONCURRENT_REQUESTS];
        #ifndef NO_ARDUINO
        SoftwareSerial serial;
        #endif
        unsigned char my_address;

        void run();
        bool request(Request* used, unsigned char from, unsigned char data1, unsigned char data2, unsigned char data3, unsigned char data4);

    private:
        void send(Packet packet);
        bool receive(Packet* packet);
        void log(const char * str);
        void logln(const char* str);
};