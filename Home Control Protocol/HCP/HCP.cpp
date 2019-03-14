/* 
  Author: Stijn Rogiest (reddusted@gmail.com)
  Creation: 12/3/2019
*/

#include "<Arduino.h>"
#include "SoftwareSerial.h"
#include "HCP.h"
#include "Packet.h"
#include "Device.h"

HCP::HCP(byte addr, int baud) : software(SoftwareSerial(-1, -1))
{
    this->address = addr;
    this->baud = baud;
    this->useHardwareSerial = true;
    this->enableLogging = false;

    Serial.begin(baud);
}

HCP::HCP(byte addr, int baud, int rxPin, int txPin) : software(SoftwareSerial(rxPin, txPin))
{
    this->address = addr;
    this->baud = baud;
    this->useHardwareSerial = false;
    this->enableLogging = true;

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
        return Serial.avaliable();
    else
        return this->software.avaliable();
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
        this.readRaw(),
        this.readRaw(),
        this.readRaw(),
        this.readRaw(),
        this.readRaw(),
        this.readRaw()
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

bool HCP::getPacket(Packet * packet)
{
    for(int i = 0; i < 8; i++)
    {
        if (!this->packets[i].responded)
        { 
            packet = this->packets[i];
            return true;
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

// Returns true if data needs to be interpret, false if it is a command (interpret here) abd false if data isn't yours.
/*bool SR_HCP::hcpReceive(byte *fromAddress, byte *data, bool sync = false)
{
    if (sync)
        while (this->hcpRawAvailable() < 4)
            delay(10);
    else if (this->hcpRawAvailable() < 4)
        return false;
    
    if (this->hcpRawPeek() != this->firstByte) // Out of sync
    {
        if (!this->useHardwareSerial)
        {
            this->log("Out of sync (");
            this->log(String(hcpRawPeek()));
            this->log(" != ");
            this->log(String(this->firstByte));
            this->log(")... ");
        }

        int r = 0;

        while (r !=  this->firstByte)
        {
            r = this->hcpRawRead();

            while (!this->hcpRawAvailable());

            r = this->hcpRawPeek();
        }

        if (!this->useHardwareSerial)
            this->logln("Fixed");

        return this->hcpReceive(fromAddress, data);
    }

    hcpRawRead();

    *fromAddress = this->hcpRawRead();
    byte toAddress = this->hcpRawRead();
    *data = this->hcpRawRead();

    if (toAddress != this->address)
    {
        return this->hcpReceive(fromAddress, data, sync);
    }

    lastDidReceiveFrom = *fromAddress;

    if (!this->useHardwareSerial)
    {
        this->log("Start receiving: From=");
        this->log(String(*fromAddress));
        this->log(", To=");
        this->log(String(toAddress));
        this->log(", Data=");
        this->logln(String(*data));
    }

    if (!this->responded && *fromAddress == this->lastReceiver)
    {
        this->logln("Did respond with value: " + String(*data));
        this->response = *data;
        this->responded = true;
        this->sendTries = 0;

        switch(*data)
        {
            case CMD_OKEY:
                this->logln("Did respond with OKEY!");
                return false;

            case CMD_FAILED:
                this->logln("Did respond with FAILED. :(");
                return false;

            case CMD_UNKNOWN:
                this->logln("Did respond with UNKNOWN.");
                return false;
        }

        return true; // True so the user can interpret the responded data itself.
    }

    if (*data > VALUE_RANGE_MAX)
    {
        switch(*data)
        {
            case CMD_SET:
                this->respondOkey();
                this->logln("Waiting for property...");
                this->currentSetProperty = hcpReceiveFrom(*fromAddress);
                this->respondOkey();
                this->logln("Waiting for value...");
                this->currentSetValue = hcpReceiveFrom(*fromAddress);
                this->respondOkey();
                this->logln("Received set values.");
                this->properties[this->currentSetProperty] = this->currentSetValue;
                this->logln("Property " + String(this->currentSetProperty) + " is set to " + String(this->currentSetValue));
                this->propertyChange = true;
                return false;

            case CMD_FETCH:
                // WIP
                return false;

            default:
                return true; // True so the user can interpret the data itself.
        }
    }

    return true;
}
*/