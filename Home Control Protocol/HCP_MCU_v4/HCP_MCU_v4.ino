#include <SoftwareSerial.h>
#include "Packet.h"
#include "PacketSenderReceiver.h"

#define DEBUG_PIN LED_BUILTIN
// Note: HC12 TX to RX and RX to TX
#define TX_PIN 14
#define RX_PIN 12

SoftwareSerial ss = SoftwareSerial(RX_PIN, TX_PIN);
PacketSenderReceiver sr = PacketSenderReceiver(&ss, false, 2);

void setup()
{
  pinMode(DEBUG_PIN, OUTPUT);

  ss.begin(2400);
  Serial.begin(19200);
}

void loop()
{
  digitalWrite(DEBUG_PIN, true);
  delay(1000);
  digitalWrite(DEBUG_PIN, false);
  delay(1000);

  send();

  if (random(0,5) == 0)
  {
    delay(random(0, 20));
    
    Serial.println("Send an extra one :)");
    
    send();  
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
