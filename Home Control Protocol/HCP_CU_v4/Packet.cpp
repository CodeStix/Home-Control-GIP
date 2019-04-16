#include "Packet.h"
#include "SoftwareSerial.h"
#include "Arduino.h"

unsigned char Packet::identifier = 0x69;

Packet::Packet()
{
  memset(this->data, 0, 20);
}

Packet::Packet(unsigned char* data, unsigned char len)
{
  memset(this->data, 0, 20);
  memcpy(this->data, data, len);
}

Packet::Packet(unsigned char slaveAddress, unsigned char masterAddress, unsigned char* data, unsigned char len, PacketType type)
{
  memset(this->data, 0, 20);

  if (len <= 0)
  {
    len = 1;
  }
  else
  {
    memcpy(&this->data[3], &data[0], len);
  }

  this->data[0] = Packet::identifier;
  this->data[1] = slaveAddress & 0x3F;
  this->data[2] = ((masterAddress & 0x3) << 6) | ((type & 0x3) << 4) | ((len - 1) & 0xF);

  this->data[1] |= getCurrentCRC() << 6;
}

bool Packet::hasValidIntegrity()
{
  return this->getCurrentCRC() == this->getCRC();
}

unsigned char Packet::getCurrentCRC()
{
  unsigned char crc = ~Packet::identifier;

  for (int i = 2; i < 20; i++)
    crc ^= this->data[i];
  crc ^= this->data[1] & 0x3F;

  return (crc ^ (crc >> 2) ^ (crc >> 4) ^ (crc >> 6)) & 0x3;
}

void Packet::sendViaSoftware(SoftwareSerial* ss)
{
  ss->write(this->data, getLength() + 4);
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

PacketType Packet::getType()
{
  return static_cast<PacketType>((this->data[2] >> 4) & 0x3);
}

unsigned char Packet::getLength()
{
  return this->data[2] & 0xF;
}

void Packet::updateIntegrity()
{
  this->data[1] |= this->getCurrentCRC() << 6;
}
