#include "HCPClient.h"

HCPClient client;

void setup()
{
  Serial.begin(9600);
  Serial.println("Test");

  client = HCPClient(10, 2, 3);
}

void loop()
{

}
