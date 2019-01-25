#include "SR_HCP.h"
#define MY_ADDRESS 1


// Valid address ranges for each device type.
#define MCU_ADDRESS_RANGE_MIN 0
#define MCU_ADDRESS_RANGE_MAX 15
#define CU_ADDRESS_RANGE_MIN 16
#define CU_ADDRESS_RANGE_MAX 255
// Identifiers(error, info, commands, requests, ...) that will be recognized by the main unit.
#define ID_MCU_OKEY 255
#define ID_MCU_UNKNOWN 254
#define ID_MCU_ERROR 253
// Identifiers(error, info, commands, requests, ...) that will be recognized  by the controlled units (this).
#define ID_CU_PING 255

/* Main control unit */

SR_HCP hcp = SR_HCP(MY_ADDRESS, 2400, 12, 14);
long waitingTime = 0;

String inString = "";
String argument = "";
int value = 0;
int prop = 0;

void flash(int mil, int cycles = 1)
{
  for (int i = 0; i < cycles; i++)
  {
    digitalWrite(LED_BUILTIN, true);
    delay(mil / 2);
    digitalWrite(LED_BUILTIN, false);
    delay(mil / 2);
  }
}

void received(int fromAddress, int data)
{
  if (data == ID_MCU_OKEY) // 255 = OK
  {
    flash(100);
  }
  else if (data == ID_MCU_UNKNOWN) // 254 = UNKNOWN
  {
    flash(100, 5);
  }
  else if (data == ID_MCU_ERROR) // 253 = ERROR
  {
    flash(100, 3);
  }
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);

  Serial.print("H-MCU Startup Address: ");
  Serial.println(MY_ADDRESS);

  delay(100);

  flash(50, 10);
}

void connect()
{


}

void loop()
{
  int fromAddress, data;

  if (hcp.hcpReceive(&fromAddress, &data, false))
    received(fromAddress, data);

  hcp.hcpResendIfNeeded();

  while (Serial.available() > 0)
  {
    int inChar = Serial.read();
    if (isAlphaNumeric(inChar))
      inString += (char)inChar;
    
    if (inChar == '\n')
    {
      Serial.print(">>> ");
      Serial.println(inString);

      if (inString == "rndto")
      {
        Serial.println("<<< Sending random data to " + argument.toInt());
        hcp.hcpSend(argument.toInt(), random(0, 4));
      }
      else if (inString.startsWith("d"))
      {
        int data = inString.substring(1).toInt();
        
        Serial.println("<<< Sending " + String(data)  + " to " + argument.toInt());

        hcp.hcpSend(argument.toInt(), data);
      }
      else if (inString.startsWith("v"))
      {
        value = inString.substring(1).toInt();
        
        Serial.println("<<< Value is set to " + String(value));
      }
      else if (inString.startsWith("p"))
      {
        prop = inString.substring(1).toInt();
        
        Serial.println("<<< Selected property is now " + String(prop));
      }
      else if (inString == "set")
      {
        Serial.println("<<< Setting...");
        hcp.hcpSendSet(argument.toInt(), prop, value);
      }
      else if (inString == "ok")
      {
        Serial.println("<<< OK");
      }
      else
      {
        argument = inString;
                
        Serial.print("<<< Argument = ");
        Serial.println(argument);
      }

      inString = "";
    }
  }

  delay(10);
}


