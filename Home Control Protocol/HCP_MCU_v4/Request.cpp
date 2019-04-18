#include "Request.h"
#include "Packet.h"
#include "SoftwareSerial.h"
#include "Logger.h"

unsigned char Request::currentId = 0;

Request::Request()
{
    this->used = false;
}

Request::Request(unsigned char fromAddress, ResponseHandler handler, unsigned char* data, unsigned char len)
{
    this->use(fromAddress, handler, data, len);
}

void Request::use(unsigned char fromAddress, ResponseHandler handler, unsigned char* data, unsigned char len)
{
    this->handler = handler;
    this->fromAddress = fromAddress;
    this->id = Request::currentId++;
    this->gotAnswered = false;
    this->sentMillis = millis();
    this->createdMillis = this->sentMillis;
    this->used = true;
    this->resendTries = 0;
    memset(this->sentData, 0, 20);
    memcpy(this->sentData, data, len);
    this->sentDataLength = len;
}

bool Request::shouldGetResend()
{
    if (this->gotAnswered || !this->used || this->resendTries > REQUEST_MAX_RESENDS)
        return false;

    return (millis() - this->sentMillis) > REQUEST_TRY_INTERVAL;
}

bool Request::mayGetDisposed()
{
    return !used || this->resendTries > REQUEST_MAX_RESENDS || (millis() - this->createdMillis) > REQUEST_MAX_LIFETIME;
}

void Request::answered(unsigned char* respData, unsigned char respLen)
{
  this->handler(respData, respLen);
  
  this->used = false;
}
