#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include "Packet.h"
#include "PacketSenderReceiver.h"

#define DEBUG_PIN LED_BUILTIN
// Note: HC12 TX to RX and RX to TX
#define TX_PIN 14
#define RX_PIN 12
// This masters address, can be 1, 2 or 3.
#define MASTER_ADDRESS 2

const char* ssid = "PollenPatatten";
const char* password = "Ziektes123";

WiFiServer server(80);
SoftwareSerial ss = SoftwareSerial(RX_PIN, TX_PIN);
PacketSenderReceiver sr = PacketSenderReceiver(&ss, false, MASTER_ADDRESS);
Packet temp;
bool debugLed = true;

void setup()
{
  pinMode(DEBUG_PIN, OUTPUT);

  Serial.begin(19200);
  Serial.println();
  Serial.println("  _   _      ____    ____    ");
  Serial.println(" |'| |'|  U /\"___| U|  _\"\\ u ");
  Serial.println("/| |_| |\\ \\| | u   \\| |_) |/ ");
  Serial.println("U|  _  |u  | |/__   |  __/   ");
  Serial.println(" |_| |_|    \\____|  |_|      ");
  Serial.println(" //   \\\\   _// \\ \\  ||>>_    ");
  Serial.println("(_\") (\"_) (__)(__) (__)__)  v0.4.0 (c)");
  Serial.println("\tby Stijn Rogiest 2019");
  Serial.println();
  Serial.print("Master address: ");
  Serial.println(MASTER_ADDRESS);
  Serial.println("Starting...");

  delay(500);

  ss.begin(2400);
}

void loop()
{
  if (millis() % 500 == 0)
  {
    digitalWrite(DEBUG_PIN, debugLed);

    debugLed = !debugLed;
  }


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
    else if (c == 'b')
    {
      sendBroadcast();
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

void sendBroadcast()
{
  unsigned char data[1] = { 0x40 };
  sr.broadcast(data, sizeof(data), DataRequest, 0x40);
}

void answer(ResponseStatus status, unsigned char* respData, unsigned char respLen)
{
  if (status == NoResponse)
    Serial.print("Packet did not get answered: ");
  else if (status == Failed)
    Serial.print("Packet got answered (failed): ");
  else
    Serial.print("Packet got answered (okey): ");

  for (int i = 0; i < respLen; i++)
  {
    Serial.print(respData[i]);
    Serial.print(' ');
  }
  Serial.println();
}
