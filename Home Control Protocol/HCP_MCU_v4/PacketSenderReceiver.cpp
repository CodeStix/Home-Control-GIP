#include "PacketSenderReceiver.h"

PacketSenderReceiver::PacketSenderReceiver(SoftwareSerial* serial)
{
    this->serial = serial;
    this->receiving = false;
    this->dataPosition = 0;
    this->incomingLength = 0;
}

bool receiveAny(Packet* p)
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

      Serial.print("Out of sync: ");
      Serial.print(this->serial->peek());
      Serial.print(" != ");
      Serial.println(Packet::identifier);

      this->serial->read();
    }

    if (this->serial->available() < 3)
      return false;

    this->receiving = true;
    for (this->dataPosition = 0; this->dataPosition < 3; this->dataPosition++)
      p->data[this->dataPosition] = this->serial->read();
    this->incomingLength = (p->data[2] & 0xF) + 1;

    return receive(p);
  }
}