#include <SoftwareSerial.h>
#include "Packet.h"
#include "PacketSenderReceiver.h"

#define DEBUG_PIN LED_BUILTIN
// Note: HC12 TX to RX and RX to TX
#define TX_PIN 14
#define RX_PIN 12

SoftwareSerial ss = SoftwareSerial(RX_PIN, TX_PIN);
PacketSenderReceiver sr = PacketSenderReceiver(&ss, false, 2);
Packet temp;

bool debugLed = true;

void setup()
{
  pinMode(DEBUG_PIN, OUTPUT);

  ss.begin(2400);
  Serial.begin(19200);
}

void loop()
{
  if (millis() % 500 == 0)
  {
    digitalWrite(DEBUG_PIN, debugLed);

    debugLed = !debugLed;
  }

  /*if (millis() % 1000 == 0)
  {
    send();
  }*/

  sr.resendUnansweredRequests();

  if (Serial.available() > 0)
  {
    char c = Serial.read();

    if (c == 's')
    {
      send();
    }
    else if (c == 'r')
    {
      sendRequest();
    }
  }

  /*if (random(0,5) == 0)
    {
    delay(random(0, 20));

    Serial.println("Send an extra one :)");

    send();
    }*/

  if (sr.receive(&temp))
  {
    Serial.print("Received packet: ");
    temp.printToSerial();
    Serial.println();

    if (temp.needsResponse())
    {
      Serial.println("Packet needs response.");
    }
  }
}

void send()
{
  unsigned char data[] = {20, 6};

  sr.send(20, data, sizeof(data), Push);

  /*Packet p = Packet(20, 3, data, sizeof(data), Push);
    p.sendViaSoftware(&ss);*/

  Serial.print("Send packet: ");
  sr.lastSentPacket.printToSerial();
  Serial.println();

  /*for (int i = 0; i < 8; i++)
    ss.write(i);
    Serial.println("Send data x2");*/
}

void sendRequest()
{
  sr.sendRequest(20, answer, {}, 0);
}

void answer(unsigned char* respData, unsigned char respLen)
{
  Serial.print("Packet got answered: ");
  for(int i = 0; i < respLen; i++)
  {
    Serial.print(respData[i]);
    Serial.print(' ');
  }
  Serial.println();
}
