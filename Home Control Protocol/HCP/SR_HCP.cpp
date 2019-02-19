/* 
  Author: Stijn Rogiest (reddusted@gmail.com)
  Creation: 10/2018 
*/

#include <Arduino.h>
#include <SR_HCP.h>
#include "SoftwareSerial.h"

// We will accept values until this point, higher values will get recognized as a command.
#define VALUE_RANGE_MAX 240
// Commands that the master receives.
#define CMD_OKEY 255
#define CMD_FAILED 254
#define CMD_UNKNOWN 253
// Command that the slave receives.
#define CMD_FETCH 252
#define CMD_SET 250

SR_HCP::SR_HCP(byte addr, int baud) : software(SoftwareSerial(-1, -1))
{
    this->address = addr;
    this->baud = baud;
    this->useHardwareSerial = true;
    this->firstByte = 0xf;
    this->resendMillis = 675;
    this->enableLogging = true;
    this->responded = true;
    this->maxSendTries = 20;
    this->enableResend = true;

    Serial.begin(baud);
}

SR_HCP::SR_HCP(byte addr, int baud, int rxPin, int txPin) : software(SoftwareSerial(rxPin, txPin))
{
    this->address = addr;
    this->baud = baud;
    this->useHardwareSerial = false;
    this->firstByte = 0xf;
    this->resendMillis = 600;
    this->enableLogging = true;
    this->responded = true;
    this->maxSendTries = 20;
    this->enableResend = true;

    this->software.begin(baud);
}

void SR_HCP::log(String str, bool force = false)
{
    if (!this->useHardwareSerial && (this->enableLogging || force))
    {
        Serial.print(str);
    }
}

void SR_HCP::logln(String str, bool force = false)
{
    if (!this->useHardwareSerial && (this->enableLogging || force))
    {
        Serial.println(str);
    }
}

// Returns true if data needs to be interpret, false if it is a command (interpret here) abd false if data isn't yours.
bool SR_HCP::hcpReceive(byte *fromAddress, byte *data, bool sync = false)
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

void SR_HCP::hcpSendSet(byte address, byte property, byte value)
{
    logln("Sending set");
    this->hcpSend(address, CMD_SET);
    this->hcpReceiveFrom(address);
    logln("Sending property value");
    this->hcpSend(address, property);
    this->hcpReceiveFrom(address);
    logln("Sending value");
    this->hcpSend(address, value);
    this->hcpReceiveFrom(address);

    this->responded = true;
}

byte SR_HCP::hcpReceiveFrom(byte address)
{
    byte fromAddress, data;
    bool d = true;

    do
    {
        d = this->hcpReceive(&fromAddress, &data, true);
    }
    while (fromAddress != address && d);

    return data;
}

void SR_HCP::hcpSend(byte toAddress, byte data)
{
    this->responded = false;
    this->lastSendMillis = millis();
    this->lastReceiver = toAddress;
    this->lastSendData = data; 

	this->hcpRawSend(this->firstByte);
	this->hcpRawSend(this->address);
	this->hcpRawSend(toAddress);
	this->hcpRawSend(data);
}

void SR_HCP::hcpRawSend(byte data)
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

byte SR_HCP::hcpRawRead()
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

byte SR_HCP::hcpRawAvailable()
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

byte SR_HCP::hcpRawPeek()
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

void SR_HCP::hcpResendIfNeeded()
{
    if (this->enableResend && !this->responded && millis() - this->lastSendMillis >= this->resendMillis && this->sendTries <= this->maxSendTries)
    {
        SR_HCP::hcpResend();
    }
}

void SR_HCP::hcpResend()
{
    this->logln("Resending last...");
    this->sendTries++;
    hcpSend(this->lastReceiver, this->lastSendData);
}

bool SR_HCP::didRespond()
{
    return this->responded;
}

void SR_HCP::respondOkey()
{
    hcpSend(lastDidReceiveFrom, CMD_OKEY);

    this->responded = true;
}

void SR_HCP::respondFailed()
{
    hcpSend(lastDidReceiveFrom, CMD_FAILED);

    this->responded = true;
}

void SR_HCP::respondUnknown()
{
    hcpSend(lastDidReceiveFrom, CMD_UNKNOWN);

    this->responded = true;
}

void SR_HCP::respond(byte b)
{
    hcpSend(lastDidReceiveFrom, b);

    this->responded = true;
}

bool SR_HCP::didPropertyChange()
{
    if (this->propertyChange)
    {
        this->propertyChange = false;
        return true;
    }

    return false;
}