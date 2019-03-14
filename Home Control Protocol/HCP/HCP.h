/* 
  Author: Stijn Rogiest (reddusted@gmail.com)
  Creation: 12/3/2019
*/

#ifndef HCP_h
#define HCP_h

#include "<Arduino.h>"
#include "SoftwareSerial.h"
#include "HCP.h"
#include "Packet.h"
#include "Device.h"

class HCP
{
    public:
        HCP(byte addr, int baud);
        HCP(byte addr, int baud, int rxPin, int txPin);

        bool useHardwareSerial;
        int baud;
        SoftwareSerial software;
        bool enableLogging = false;
        Packet packets[8];

        void send(Packet p);
        void receive();
        bool getPacket(Packet * packet);
        void sendResponse(Packet p, byte data1, byte data2, byte data3, byte data4);

      private:
        void log(String str, bool force);
        void logln(String str, bool force);

        byte address;

        void sendRaw(byte data[8]);
        byte readRaw();
        byte peekRaw();
        int availableRaw();

        
};

#endif