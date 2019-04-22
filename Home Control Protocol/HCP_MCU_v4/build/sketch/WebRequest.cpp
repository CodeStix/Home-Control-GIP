#include "WebRequest.h"
#include "WiFiClient.h"
#include "Arduino.h"

WebRequest::WebRequest(WiFiClient client)
{
    this->client = client;
    this->receivedMillis = millis();
    this->alive = true;
}

void WebRequest::println(String str)
{
    this->client.println(str);
}

void WebRequest::print(String str)
{
    this->client.print(str);
}

void WebRequest::close()
{
    this->client.stop();
    this->alive = false;
}