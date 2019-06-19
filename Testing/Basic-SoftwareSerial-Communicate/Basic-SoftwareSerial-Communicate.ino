#include "SoftwareSerial.h"

// 10 11
// 12 14
SoftwareSerial ss(10, 11);

void setup() 
{
  ss.begin(2400);
  Serial.begin(19200);

  Serial.println(" ----- basic communication ----- ");
}

void loop() 
{
  if (ss.available() > 0)
  {
    Serial.print("Received: ");
    Serial.println(char(ss.read()));
  }

  if (Serial.available() > 0)
  {
    Serial.print("Sending ");
    char t = Serial.read();
    Serial.println(t);

    ss.write(t);
    ss.flush();
  }

}
