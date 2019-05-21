#ifndef Logger_h
#define Logger_h

#include "Arduino.h"

class Logger
{
    public:
        Logger();
        void log(char* s);
        void log(char s);
        void log(unsigned char i);
        void log(int i);
        void log(long i);
        void logln(char* s);
        void logln(char s);
        void logln(unsigned char i);
        void logln(int i);
        void logln(long i);
        void logln();
        bool enable;
};

#endif
