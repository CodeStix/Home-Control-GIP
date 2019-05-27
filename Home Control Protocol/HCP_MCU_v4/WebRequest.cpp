#include "WebRequest.h"
#include "WiFiClient.h"
#include "Arduino.h"

WebRequest::WebRequest(WiFiClient client)
{
    this->client = client;
    this->receivedMillis = millis();
    this->clientData = "";
    this->requestId = 255;
    this->closed = false;
}

void WebRequest::println(String str)
{
    this->client.println(str);
}

void WebRequest::print(String str)
{
    this->client.print(str);
}

void WebRequest::println(int i)
{
    this->client.println(i);
}

void WebRequest::print(int i)
{
    this->client.println(i);
}

void WebRequest::close()
{
    this->closed = true;

    if (this->client)
        this->client.stop();

    Serial.println("WebRequest kermitted suicide (1)");
}

// funcRequester returns the fact to leave open connection or not. 
void WebRequest::update(bool(*funcRequester)(WebRequest*, String))
{
    if (shouldBeDisposed())
    {
        Serial.println("WebRequest should kermit suicide (2)");
        return;
    }

    while (client && client.available())
    {
        char c = client.read();

        if (c == '\r')
            continue;

        clientData += c;

        if (clientData.length() > 2 && c == '\n' && clientData[clientData.length() - 2] == '\n')
        {
            int i = clientData.indexOf("GET "), j = clientData.indexOf(" HTTP/");
            bool open = false;
            if (i >= 0 && j >= 0)
            {
                String request = clientData.substring(i + 4, j);
                request.trim();
                if (request.startsWith("/favicon"))
                {
                    open = false;
                }
                else
                {
                    open = funcRequester(this, request);
                }
            }
            if (!open)
                close();
        }
    }
}

bool WebRequest::shouldBeDisposed()
{
    return closed || !client || !client.connected() || (millis() - receivedMillis) > 20000;
}