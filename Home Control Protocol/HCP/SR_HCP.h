/* 
  Author: Stijn Rogiest (reddusted@gmail.com)
  Creation: 10/2018 
*/

#ifndef SR_HCP_h
#define SR_HCP_h

#include "SoftwareSerial.h"

class SR_HCP
{
    public:
        SR_HCP(int addr, int baud);
        SR_HCP(int addr, int baud, int rxPin, int txPin);
        bool useHardwareSerial;
        int address;
        int baud;
        byte firstByte;
        SoftwareSerial software;
        void hcpSend(int toAddress, int data);
        bool hcpReceive(int *fromAddress, int *data, bool sync);
        void hcpRawSend(int data);
        int hcpRawRead();
        int hcpRawAvailable();
        int hcpRawPeek();
        void hcpResendIfNeeded();
        void hcpResend();
        int resendMillis;
        bool didReceive();

      private:
        int lastSendMillis;
        int lastSendData;
        int lastReceiver;
        bool didReceive;
};


#endif