#include <SoftwareSerial.h>
#include "Packet.h"

#define DEBUG_PIN LED_BUILTIN
// Note: HC12 TX to RX and RX to TX
#define TX_PIN 10
#define RX_PIN 11

SoftwareSerial ss = SoftwareSerial(RX_PIN, TX_PIN);
Packet temp = Packet({}, 0);

void setup()
{
  pinMode(DEBUG_PIN, OUTPUT);

  ss.begin(2400);
  Serial.begin(19200);
}

void loop()
{
  digitalWrite(DEBUG_PIN, true);
  delay(500);
  digitalWrite(DEBUG_PIN, false);
  delay(500);

  if (receive(&temp))
  {
    Serial.print("Received packet: ");
    temp.printToSerial();
    Serial.println();
  }
}

/*bool receive()
{
  if (ss.available() < 8)
  {
    Serial.print("No data (");
    Serial.print(ss.available());
    Serial.println(')');

    return false;
  }
  else
  {
    Serial.println("Received data x8");

    for (int i = 0; i < 8; i++)
    {
      Serial.print('[');
      Serial.print(ss.read());
      Serial.print(']');
    }

    Serial.println();

    return true;
  }

}*/

bool receive(Packet* p)
{
  if (ss.available() < 8)
    return false;

  while (ss.peek() != Packet::identifier)
  {
    if (ss.available() == 0)
      return false;

    ss.read();
  }

  if (ss.available() < 8)
    return false;

  for(int i = 0; i < 8; i++)
    p->data[i] = ss.read(); 

  return true;
}
