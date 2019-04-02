#include "HCPClient.h"
#include <stdio.h>

#ifndef NO_ARDUINO
#include "Arduino.h"
#endif

#ifndef MAX_CONCURRENT_REQUESTS
#define MAX_CONCURRENT_REQUESTS 16
#endif

#ifndef NO_ARDUINO
HCPClient::HCPClient(unsigned char my_address, int rxPin, int txPin) : this->software(rxPin, txPin)
{
    this->my_address = my_address;
}
#else
HCPClient::HCPClient(unsigned char my_address)
{
    this->my_address = my_address;
}
#endif

void HCPClient::run()
{
    static Packet temp = Packet();

    if (this->receive(&temp)) 
    {
        this->log("Processing received packet: ");
        this->logln(temp.to_string());

        if (temp.to != this->my_address)
        {
            this->logln("Packet is not for me! Ignoring...");

            return;
        }

        for(int i = 0; i < MAX_CONCURRENT_REQUESTS; i++)
        {
            this->log("\tIs the response for request ");
            this->log(pending[i].to_string());
            this->logln("?");

            if (pending[i].is_packet_response(temp))
            {
                //printf("\t!! Found packet for responding to request %s\n", pending[i].to_string());

                this->log("\t\tPacket got responded: ");
                this->logln(pending[i].to_string());

                //break;
            }
        }
    }
    
}

void HCPClient::send(Packet packet)
{
#ifndef NO_ARDUINO
    this->serial.write(packet.get_bytes(), 8);
#endif
    this->log("Packet was send: ");
    this->logln(packet.to_string());
}

bool HCPClient::request(Request* used, unsigned char from, unsigned char data1, unsigned char data2, unsigned char data3, unsigned char data4)
{
    for(int i = 0; i < MAX_CONCURRENT_REQUESTS; i++)
    {
        if (!pending[i].isUsed)  
        {
            used = &pending[i];
            used->use_for(from, data1, data2, data3, data4);
            
            this->log("Sending request: ");
            this->logln(used->to_string());
            this->send(used->get_request_packet());

            return true;
        } 
        else if (i == MAX_CONCURRENT_REQUESTS - 1)
        {
            this->logln("!! Cannot request, max concurrent requests reached.");

            return false;
        }
    }

    return false;
}

bool HCPClient::receive(Packet* packet)
{
#ifdef NO_ARDUINO
    return false;
#else
    if (Serial.available() < 8)
        return false;

    packet = &Packet(
    { 
        Serial.read(),
        Serial.read(),
        Serial.read(),
        Serial.read(),
        Serial.read(),
        Serial.read(),
        Serial.read(),
        Serial.read()
    });

    return true;
#endif
}

void HCPClient::log(const char* str)
{   
#ifdef NO_ARDUINO
    printf("%s", str);
#else
    Serial.print(str);
#endif
}

void HCPClient::logln(const char* str)
{
#ifdef NO_ARDUINO
    printf("%s\n", str);
#else
    Serial.println(str);
#endif
}