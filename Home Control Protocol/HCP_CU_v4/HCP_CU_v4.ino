#include <SoftwareSerial.h>
#include "Packet.h"
#include "PacketSenderReceiver.h"

#define DEBUG_PIN LED_BUILTIN
// Note: HC12 TX to RX and RX to TX
#define TX_PIN 10
#define RX_PIN 11

SoftwareSerial ss = SoftwareSerial(RX_PIN, TX_PIN);
PacketSenderReceiver sr = PacketSenderReceiver(&ss, true, 20);
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
  if (millis() % 50 == 0)
  {
    digitalWrite(DEBUG_PIN, debugLed);

    debugLed = !debugLed;
  }

  if (sr.receive(&temp))
  {
    Serial.print("Received packet: ");
    temp.printToSerial();
    Serial.println();

    if (temp.needsResponse())
    {
      Serial.println("Packet needs response");

      if (temp.getDataLength() == 1)
      {
        if (temp.getData()[0] == 0x40)
        {
          Serial.print("It wants my identity...");

          delay(random(0, 2000));
          Serial.println("Sent");

          unsigned char data[] = "identity";
          sr.answer(&temp, data, sizeof(data));
        }
      }
      else
      {
        // 0x0 = failed
        unsigned char data[1] = { 0x0 };
        sr.answer(&temp, data, sizeof(data));
      }
    }
  }
}

/*bool receiving = false;
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
*/
