/* 
  Author: Stijn Rogiest (reddusted@gmail.com)
  Creation: 12/3/2019
*/

#include "HCP.h"
#include "Packet.h"
#include "Device.h"

/// Create a home control protocol object using the standard tx and rx pins on the controller.
HCP::HCP(byte addr, int baud) : software(SoftwareSerial(-1, -1))
{
    this->address = addr;
    this->baud = baud;
    this->useHardwareSerial = true;
    this->enableLogging = false;

    Serial.begin(baud);
}

/// Create a home control protocol object using the specified tx and px pins.
HCP::HCP(byte addr, int baud, int rxPin, int txPin) : software(SoftwareSerial(rxPin, txPin))
{
    this->address = addr;
    this->baud = baud;
    this->useHardwareSerial = false;
    this->enableLogging = false;

    this->software.begin(baud);
}

void HCP::log(String str, bool force = false)
{
    if (!this->useHardwareSerial && (this->enableLogging || force))
        Serial.print(str);
}

void HCP::logln(String str, bool force = false)
{
    if (!this->useHardwareSerial && (this->enableLogging || force))
        Serial.println(str);
}

void HCP::send(Packet p)
{
    this->sendRaw(p.getBytes());
}

void HCP::sendRaw(byte data[8])
{
    if (this->useHardwareSerial)
    {
        for(byte i = 0; i < 8; i++)
            Serial.write(data[i]);
    }
    else
    {
        for(byte i = 0; i < 8; i++)
            software.write(data[i]);
    }
}

byte HCP::readRaw()
{
    if (this->useHardwareSerial)
        return Serial.read();
    else
        return this->software.read();
}

byte HCP::peekRaw()
{
    if (this->useHardwareSerial)
        return Serial.peek();
    else
        return this->software.peek();
}

int HCP::availableRaw()
{
    if (this->useHardwareSerial)
        return Serial.available();
    else
        return this->software.available();
}

void HCP::receive()
{
    if (this->availableRaw() < 8)
        return;

    if (this->peekRaw() != Packet::identifierByte)
    {
        this->readRaw();
        
        return;
    }

    Packet p = new Packet(
    {
        this->readRaw(),
        this->readRaw(),
        this->readRaw(),
        this->readRaw(),
        this->readRaw(),
        this->readRaw(),
        this->readRaw(),
        this->readRaw()
    });

    if (p.toAddress != this->address)
    {
        this->logln("Packet is not for me, actually for " + String(p.toAddress));

        return;
    }

    if (!p.getIsValid())
    {
        this->logln("Received packet was not valid!");

        return;
    }

    for(int i = 1; i < 8; i++)
        this->packets[i] = this->packets[i - 1];

    this->packets[0] = p;
}

bool HCP::getPacket(Packet *packet)
{
    for(int i = 0; i < 8; i++)
    {
        if (this->packets[i])
        {
            if (!this->packets[i].responded)
            { 
                *packet = this->packets[i];
                return true;
            }
        }
    }

    return false;
}

void HCP::sendResponse(Packet p, byte data1, byte data2, byte data3, byte data4)
{
    p.responded = true;

    // We fill the packet with respond data.
    p.data1 = data1;
    p.data2 = data2;
    p.data3 = data3;
    p.data4 = data4;

    // Send the packet back to the sender of this packet.
    byte addr = p.toAddress;
    p.toAddress = p.fromAddress;
    p.fromAddress = addr;

    this->send(p);
}