#include "PacketSenderReceiver.h"

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
    while (this->serial->available() > 0 && this->dataPosition < (this->incomingLength + 3))
      p->data[this->dataPosition++] = this->serial->read();

    if (this->dataPosition >= (this->incomingLength + 3))
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
    if (this->serial->available() < 3)
      return false;

    while (this->serial->peek() != Packet::identifier)
    {
      if (this->serial->available() == 0)
        return false;

      // TODO: make logger
      //Serial.print("Out of sync: ");
      //Serial.print(this->serial->peek());
      //Serial.print(" != ");
      //Serial.println(Packet::identifier);

      this->serial->read();
    }

    if (this->serial->available() < 3)
      return false;

    this->receiving = true;
    memset(p->data, 0, 20);
    for (this->dataPosition = 0; this->dataPosition < 3; this->dataPosition++)
      p->data[this->dataPosition] = this->serial->read();
    this->incomingLength = (p->data[2] & 0xF) + 1;

    return receiveAny(p);
  }
}

bool PacketSenderReceiver::receive(Packet* packet)
{
  if (receiveAny(packet))
  {
    if (!packet->hasValidIntegrity() && packet->getType() != PleaseResend)
    {
      this->send(!this->isSlave ? packet->getMaster() : packet->getSlave(), {}, 0, PleaseResend);

      this->log("Faulty integrity: ");
      packet->printToSerial();
      this->logln();
      
      return false;
    }

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

    if (packet->getType() == PleaseResend)
    {
      this->resendLastPacket();

      this->logln("Resent last packet.");
      
      return false;
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

  packet.sendViaSoftware(this->serial);
}

void PacketSenderReceiver::send(unsigned char to, unsigned char* data, unsigned char len, PacketType type)
{
  if (this->isSlave)
  {
    Packet p = Packet(this->address, to, data, len, type);
    this->send(p);
  }
  else
  {
    Packet p = Packet(to, this->address, data, len, type);
    this->send(p);
  }
}

void PacketSenderReceiver::resendLastPacket()
{
  this->send(this->lastSentPacket);
}
