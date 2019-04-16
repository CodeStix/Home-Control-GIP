#ifndef Logger_h
#define Logger_h

class Logger
{
    public:
        Logger();
        void log(char* s);
        void log(char s);
        void logln(char* s);
        void logln();
        bool enable;
};

#endif
