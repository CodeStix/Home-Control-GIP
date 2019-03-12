/* 
  Author: Stijn Rogiest (reddusted@gmail.com)
  Creation: 12/3/2019
*/

#ifndef HCP_h
#define HCP_h

#include "SoftwareSerial.h"

class HCP
{
    public:
        HCP(byte addr, int baud);
        HCP(byte addr, int baud, int rxPin, int txPin);
        bool useHardwareSerial;
        int baud;
        SoftwareSerial software;
        bool enableLogging;
        byte properties[255];

      private:
        void log(String str, bool force);
        void logln(String str, bool force);

        byte address;
};


#endif