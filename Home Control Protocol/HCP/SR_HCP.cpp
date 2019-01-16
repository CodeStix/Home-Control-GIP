/* 
  Author: Stijn Rogiest (reddusted@gmail.com)
  Creation: 10/2018 
*/

#include <Arduino.h>
#include <SR_HCP.h>
#include "SoftwareSerial.h"




SR_HCP::SR_HCP(int addr, int baud) : software(SoftwareSerial(-1, -1))
{
    this->address = addr;
    this->baud = baud;
    this->useHardwareSerial = true;
    this->firstByte = 0xf;

    Serial.begin(baud);
}

SR_HCP::SR_HCP(int addr, int baud, int rxPin, int txPin) : software(SoftwareSerial(rxPin, txPin))
{
    this->address = addr;
    this->baud = baud;
    this->useHardwareSerial = false;
    this->firstByte = 0xf;

    this->software.begin(baud);
}

bool SR_HCP::hcpReceive(int *fromAddress, int *data, bool sync = false)
{
    if (sync)
        while (this->hcpRawAvailable() < 4);
    else if (this->hcpRawAvailable() < 4)
        return false;
    
    if (this->hcpRawPeek() != this->firstByte) // Out of sync
    {
        if (!this->useHardwareSerial)
        {
            Serial.print("Out of sync (");
            Serial.print(hcpRawPeek(), HEX);
            Serial.print(" != ");
            Serial.print(this->firstByte);
            Serial.print(")... ");
        }

        int r = 0;

        while (r !=  this->firstByte)
        {
            r = this->hcpRawRead();

            while (!this->hcpRawAvailable());

            r = this->hcpRawPeek();
        }

        if (!this->useHardwareSerial)
            Serial.println("Fixed");

        return this->hcpReceive(fromAddress, data);
    }

    hcpRawRead();

    *fromAddress = this->hcpRawRead();
    int toAddress = this->hcpRawRead();
    *data = this->hcpRawRead();

    if (toAddress != this->address)
    {
        return this->hcpReceive(fromAddress, data, sync);
    }

    if (!this->useHardwareSerial)
    {
        Serial.print("Start receiving: From=");
        Serial.print(*fromAddress);
        Serial.print(", To=");
        Serial.print(toAddress);
        Serial.print(", Data=");
        Serial.println(*data);
    }

    return true;
}

void SR_HCP::hcpSend(int toAddress, int data)
{
	this->hcpRawSend(this->firstByte);
	this->hcpRawSend(this->address);
	this->hcpRawSend(toAddress);
	this->hcpRawSend(data);
}

void SR_HCP::hcpRawSend(int data)
{
	if (this->useHardwareSerial)
	{
		Serial.write(data);
	}
	else
	{
		this->software.write(data);
	}
}

int SR_HCP::hcpRawRead()
{
	if (this->useHardwareSerial)
	{
		return Serial.read();
	}
	else
	{
		return this->software.read();
	}
}

int SR_HCP::hcpRawAvailable()
{
	if (this->useHardwareSerial)
	{
		return Serial.available();
	}
	else
	{
		return this->software.available();
	}
}

int SR_HCP::hcpRawPeek()
{
	if (this->useHardwareSerial)
	{
		return Serial.peek();
	}
	else
	{
		return this->software.peek();
	}
}