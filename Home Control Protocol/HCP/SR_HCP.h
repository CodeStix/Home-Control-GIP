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
        SR_HCP(byte addr, int baud);
        SR_HCP(byte addr, int baud, int rxPin, int txPin);
        bool useHardwareSerial;
        byte address;
        int baud;
        byte firstByte;
        SoftwareSerial software;
        void hcpSend(byte toAddress, byte data);
        bool hcpReceive(byte *fromAddress, byte *data, bool sync);
        void hcpRawSend(byte data);
        byte hcpRawRead();
        byte hcpRawAvailable();
        byte hcpRawPeek();
        void hcpResendIfNeeded();
        void hcpResend();
        int resendMillis;
        bool didRespond();
        bool enableLogging;
        int maxSendTries;
        void respondOkey();
        void respondFailed();
        void respondUnknown();
        void respond(byte b);
        byte response;
        byte hcpReceiveFrom(byte address);
        bool enableResend;
        byte properties[255];
        void hcpSendSet(byte address, byte property, byte value);
        bool didPropertyChange();

      private:
        int lastSendMillis;
        int lastSendData;
        int lastReceiver;
        bool responded;
        int sendTries;
        void log(String str, bool force);
        void logln(String str, bool force);
        byte lastDidReceiveFrom;
        byte currentSetValue;
        byte currentSetProperty;
        bool propertyChange;
};


#endif