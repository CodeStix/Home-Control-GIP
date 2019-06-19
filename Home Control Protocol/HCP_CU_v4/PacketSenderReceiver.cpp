#include "PacketSenderReceiver.h"
#include "Request.h"
#include "Packet.h"
#include "SoftwareSerial.h"
#include "Logger.h"
#include "Arduino.h"

//Request PacketSenderReceiver::nullRequest;

PacketSenderReceiver::PacketSenderReceiver(SoftwareSerial* serial, bool isSlave, unsigned char address)
{
  this->serial = serial;
  this->address = address;
  this->isSlave = isSlave;
  this->receiving = false;
  this->dataPosition = 0;
  this->incomingLength = 0;
}

bool PacketSenderReceiver::receiveAny(Packet* p)
{
  if (this->receiving)
  {
    while (this->serial->available() > 0 && this->dataPosition < this->incomingLength)
      p->data[this->dataPosition++] = this->serial->read();

    if (this->dataPosition >= this->incomingLength)
    {
      this->receiving = false;

      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    if (this->serial->available() < 4)
      return false;

    while (this->serial->peek() != Packet::identifier)
    {
      if (this->serial->available() == 0)
        return false;

      this->log("Out of sync: ");
      this->log(this->serial->peek());
      this->log(" != ");
      this->logln(Packet::identifier);

      this->serial->read();
    }

    if (this->serial->available() < 4)
      return false;

    this->receiving = true;
    memset(p->data, 0, 20);
    for (this->dataPosition = 0; this->dataPosition < 4; this->dataPosition++)
      p->data[this->dataPosition] = this->serial->read();
    this->incomingLength = (p->data[2] & 0xF) + 4;

    if (this->incomingLength == 4)
    {
      this->receiving = false;

      return true;
    }

    return false;//receiveAny(p);
  }
}

bool PacketSenderReceiver::receive(Packet* packet)
{
  if (receiveAny(packet))
  {
    // Check if the received packet is not a broadcast.
    if (!((this->isSlave && packet->getSlave() == 0x0) || (!this->isSlave && packet->getMaster() == 0x0)))
    {
      // Check if this packet is for me or not.
      if ((this->isSlave && this->address != packet->getSlave()) || (!this->isSlave && this->address != packet->getMaster()))
      {
        // This packet is not for me.
        this->log("Not for me. ");
        this->log(this->address);
        this->log(" != (master: ");
        this->log(packet->getMaster());
        this->log(", slave: ");
        this->log(packet->getSlave());
        this->logln(")");

        return false;
      }
    }

    // Ask for resend if the packet has a false integrity.
    if (!packet->hasValidIntegrity() && packet->getType() != PleaseResend)
    {
      this->send(this->isSlave ? packet->getMaster() : packet->getSlave(), {}, 0, PleaseResend);

      this->log("Faulty integrity: ");
      packet->printToSerial();
      this->logln();

      return false;
    }

    // Resend packet if it is a PleaseResend packet.
    if (packet->getType() == PleaseResend)
    {
      this->resendLastPacket();

      this->log("_");

      return false;
    }

    if (packet->getType() == Answer)
    {
      /*this->log("Received request answer: ");
      packet->printToSerial();
      this->logln();*/

      Request* r = this->getRequestWithId(packet->getMultiPurposeByte());

      if (r)// != &PacketSenderReceiver::nullRequest
      {
        r->answered(packet->getData(), packet->getDataLength());

        return false;
      }
    }

    return true;
  }
  else
  {
    return false;
  }
}

void PacketSenderReceiver::send(Packet packet)
{
  this->lastSentPacket = packet;
  this->lastSentMillis = millis();

  packet.sendViaSoftware(this->serial);
}

void PacketSenderReceiver::send(unsigned char to, unsigned char* data, unsigned char len, PacketType type, unsigned char multiPurposeByte)
{
  if (this->isSlave)
  {
    Packet p = Packet(this->address, to, data, len, type, multiPurposeByte);
    this->send(p);
  }
  else
  {
    Packet p = Packet(to, this->address, data, len, type, multiPurposeByte);
    this->send(p);
  }
}

void PacketSenderReceiver::broadcast(unsigned char* data, unsigned char len, PacketType type, unsigned char multiPurposeByte)
{
  if (this->isSlave)
  {
    Packet p = Packet(this->address, 0x0, data, len, type, multiPurposeByte);
    this->send(p);
  }
  else
  {
    Packet p = Packet(0x0, this->address, data, len, type, multiPurposeByte);
    this->send(p);
  }
}

void PacketSenderReceiver::resendLastPacket()
{
  this->send(this->lastSentPacket);
}

Request* PacketSenderReceiver::getNewRequest(unsigned char fromAddress, ResponseHandler handler, unsigned char* data, unsigned char len)
{
  for (int i = 0; i < MAX_CONCURRENT_REQUESTS; i++)
  {
    if (this->requests[i].mayGetDisposed())
    {
      this->requests[i].use(fromAddress, handler, data, len);

      return &this->requests[i];
    }
  }

  this->logln("Fatal!! Ran out of requests! Increase MAX_CONCURRENT_REQUESTS!");

  return nullptr;//&PacketSenderReceiver::nullRequest;
}

void PacketSenderReceiver::resendUnansweredRequests()
{
  for (int i = 0; i < MAX_CONCURRENT_REQUESTS; i++)
  {
    if (this->requests[i].shouldGetResend())
    {
      this->requests[i].resendTries++;

      if (this->requests[i].resendTries == REQUEST_MAX_RESENDS + 1)
      {
        this->logln("!");//this->logln("Request disposed.");

        this->requests[i].noAnswer();
      }
      else
      {
        this->log(".");

        this->sendRequest(&this->requests[i]);
      }
    }
  }
}

unsigned char PacketSenderReceiver::sendRequest(Request* request)
{
  if (this->isSlave)
  {
    Packet p = Packet(this->address, request->fromAddress, request->sentData, request->sentDataLength, DataRequest, request->id);
    this->send(p);
  }
  else
  {
    Packet p = Packet(request->fromAddress, this->address, request->sentData, request->sentDataLength, DataRequest, request->id);
    this->send(p);
  }

  request->sentMillis = millis();

  return request->id;
}

unsigned char PacketSenderReceiver::sendRequest(unsigned char to, ResponseHandler handler, unsigned char* data, unsigned char len)
{
  Request* request = this->getNewRequest(to, handler, data, len);

  if (!request)//== &PacketSenderReceiver::nullRequest
    return 0;

  return this->sendRequest(request);
}

Request* PacketSenderReceiver::getRequestWithId(unsigned char id)
{
  for (int i = 0; i < MAX_CONCURRENT_REQUESTS; i++)
  {
    if (this->requests[i].used && this->requests[i].id == id)
    {
      return &this->requests[i];
    }
  }

  return nullptr;//&PacketSenderReceiver::nullRequest;
}

void PacketSenderReceiver::answer(Packet* toAnswer, unsigned char* respData, unsigned char respLen)
{
  if (!toAnswer->needsResponse())
    return;

  this->send(this->isSlave ? toAnswer->getMaster() : toAnswer->getSlave(), respData, respLen, Answer, toAnswer->getMultiPurposeByte());
}
