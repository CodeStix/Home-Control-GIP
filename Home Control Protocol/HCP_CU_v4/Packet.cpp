#include "Packet.h"
#include "SoftwareSerial.h"
#include "Arduino.h"

unsigned char Packet::identifier = 0x69;

Packet::Packet(unsigned char* data, unsigned char len)
{
  memset(this->data, 0, sizeof(this->data));

  for (int i = 0; i < len; i++)
    this->data[i] = data[i];
}

Packet::Packet(unsigned char slaveAddress, unsigned char masterAddress, unsigned char type, unsigned char* data, unsigned char len)
{
  if (len <= 0)
  {
    len = 1;
  }
  else
  {
    memcpy(this->data + 3, data, len);
  }

  this->data[0] = Packet::identifier;
  this->data[1] = slaveAddress & 0x3F;
  this->data[2] = ((masterAddress & 0x3) << 6) | ((type & 0x3) << 4) | ((len - 1) & 0xF);
  
  this->data[1] |= getCurrentCRC() << 6;
}

unsigned char Packet::getCurrentCRC()
{
  unsigned char crc = 0x32;

  for (int i = 2; i < 20; i++)
    crc ^= this->data[i];
  crc ^= this->data[1] & 0x3F;

  return (crc ^ (crc >> 2) ^ (crc >> 4) ^ (crc >> 6)) & 0x3;
}

void Packet::sendViaSoftware(SoftwareSerial* ss)
{
  ss->write(this->data, 8);
}

void Packet::printToSerial()
{
  Serial.print('[');
  Serial.print(this->getIdentifier());
  Serial.print(", CRC: ");
  Serial.print(this->getCRC());
  Serial.print(", current CRC: ");
  Serial.print(this->getCurrentCRC());
  Serial.print(", slave: ");
  Serial.print(this->getSlave());
  Serial.print(", master: ");
  Serial.print(this->getMaster());
  Serial.print(", data(");
  Serial.print(this->getLength());
  Serial.print("): ");
  for (int i = 3, ii = this->getLength(); i < 20 && i < (ii + 4); i++)
  {
    Serial.print(this->data[i], DEC);
    Serial.print(' ');
  }
  Serial.print(']');
}

char Packet::getIdentifier()
{
  return this->data[0];
}

unsigned char Packet::getCRC()
{
  return this->data[1] >> 6;
}

unsigned char Packet::getSlave()
{
  return this->data[1] & 0x3F;
}

unsigned char Packet::getMaster()
{
  return this->data[2] >> 6;
}

unsigned char Packet::getType()
{
  return (this->data[2] >> 4) & 0x3;
}

unsigned char Packet::getLength()
{
  return this->data[2] & 0xF;
}
