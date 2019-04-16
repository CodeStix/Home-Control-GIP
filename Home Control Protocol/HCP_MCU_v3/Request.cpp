#include "Request.h"
#include "Packet.h"

#ifndef NO_ARDUINO
#include "Arduino.h"
#endif

unsigned char Request::currentIndex = 10;

Request::Request()
{
    this->gotResponded = false;
    this->isUsed = false;

    this->from = 0;
    this->data1 = 0;
    this->data2 = 0;
    this->data3 = 0;
    this->data4 = 0;
    this->forIndex = 0;
}

/*Request::Request(unsigned char from, unsigned char data1, unsigned char data2, unsigned char data3, unsigned char data4)
{
    this->gotResponded = false;
    this->isUsed = true;

    this->from = from;
    this->data1 = data1;
    this->data2 = data2;
    this->data3 = data3;
    this->data4 = data4;
    this->forIndex = Request::currentIndex++;
}*/

bool Request::is_packet_response(Packet packet)
{
    //printf("is_packet_response(%s) isUsed: %s, isValid: %s\n", packet.to_string(), this->isUsed ? "yes" : "no", packet.isValid ? "yes" : "no");

    if (!this->isUsed || !packet.isValid || (packet.index != this->forIndex))
    {
        return false;
    }
    else
    {
        this->data1 = packet.data1;
        this->data2 = packet.data2;
        this->data3 = packet.data3;
        this->data4 = packet.data4;
        this->gotResponded = true;
        this->isUsed = false;

        return true;
    }
    
}

Packet Request::get_request_packet(unsigned char my_address)
{
    return Packet(my_address, this->from, this->forIndex, this->data1, this->data2, this->data3, this->data4);
}

char* Request::to_string()
{
#ifdef NO_ARDUINO

    static char str[65];

    sprintf(str, "(Used: %s, Responded: %s, Index: %d, From: %d, Response data: (%d %d %d %d))", this->isUsed ? "yes" : "no", this->gotResponded ? "yes" : "no", this->forIndex, this->from,this->data1, this->data2, this->data3, this->data4);
    
    return str;
#else

    return "";

#endif
}

 void Request::use_for(unsigned char from, unsigned char data1, unsigned char data2, unsigned char data3, unsigned char data4)
 {
    this->gotResponded = false;
    this->isUsed = true;

    this->from = from;
    this->data1 = data1;
    this->data2 = data2;
    this->data3 = data3;
    this->data4 = data4;
    this->forIndex = Request::currentIndex++;
 }