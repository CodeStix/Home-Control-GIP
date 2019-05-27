#include "Request.h"
#include "Packet.h"
#include "SoftwareSerial.h"
#include "Logger.h"

unsigned char Request::currentId = 0;

Request::Request()
{
  this->used = false;
}

Request::Request(unsigned char fromAddress, ResponseHandler handler, unsigned char* data, unsigned char len, void* state)
{
  this->use(fromAddress, handler, data, len, state);
}

void Request::use(unsigned char fromAddress, ResponseHandler handler, unsigned char* data, unsigned char len, void* state)
{
  this->handler = handler;
  this->fromAddress = fromAddress;
  this->id = Request::currentId++;
  if (Request::currentId >= 128)
    Request::currentId = 0;
  this->gotAnswered = false;
  this->sentMillis = millis();
  this->createdMillis = this->sentMillis;
  this->used = true;
  this->resendTries = 0;
  this->state = state;
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
  memset(this->response, 0 , sizeof(this->response));
  memcpy(this->response, respData, respLen);
  this->responseLength = respLen;
  
  if (respLen == 0)
  {
    this->handler(Failed, this);
  }
  else if (respLen == 1)
  {
    this->handler(respData[0] == 0xff ? Okay : Failed, this);
  }
  else
  {
    this->handler(Okay, this);
  }

  this->used = false;
}

void Request::noAnswer()
{
  memset(this->response, 0 , sizeof(this->response));
  this->responseLength = 0;
  this->handler(NoResponse, this);
  this->used = false;
}
