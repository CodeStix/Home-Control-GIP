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
  delay(50);
  digitalWrite(DEBUG_PIN, false);
  delay(50);

  if (receive(&temp))
  {
    if (!temp.hasValidIntegrity())
    {
      Serial.println("Received faulty packet, asking for resend...");
    }

    Serial.print("Received packet: ");
    temp.printToSerial();
    Serial.println();
  }
}

bool receiving = false;
unsigned char dataPosition = 0;
unsigned char incomingLength = 0;

bool receive(Packet* p)
{
  if (receiving)
  {
    while (ss.available() > 0 && dataPosition < (incomingLength + 3))
      p->data[dataPosition++] = ss.read();

    if (dataPosition >= (incomingLength + 3))
    {
      receiving = false;
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    if (ss.available() < 3)
      return false;

    while (ss.peek() != Packet::identifier)
    {
      if (ss.available() == 0)
        return false;

      Serial.print("Out of sync: ");
      Serial.print(ss.peek());
      Serial.print(" != ");
      Serial.println(Packet::identifier);

      ss.read();
    }

    if (ss.available() < 3)
      return false;

    receiving = true;
    for (dataPosition = 0; dataPosition < 3; dataPosition++)
      p->data[dataPosition] = ss.read();
    incomingLength = (p->data[2] & 0xF) + 1;

    return receive(p);
  }
}
