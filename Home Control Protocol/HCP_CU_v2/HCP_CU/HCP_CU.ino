#include "SR_HCP.h"

// Replace number with current salve module
#define MOD_47

// COM13
#ifdef MOD_45
#define MY_ADDRESS 45
#endif

// COM12
#ifdef MOD_46
#define MY_ADDRESS 46
#endif

// COM9
#ifdef MOD_47
#define MY_ADDRESS 47
bool state[4] = { false, false, false, false };
#endif

// COM14
#ifdef MOD_48
#define MY_ADDRESS 48
#endif

// We will accept values until this point, higher values will get recognized as a command.
#define VALUE_RANGE_MAX 240
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
#define CMD_PING 255
#define CMD_SET 250
/* Controlled unit */

SR_HCP hcp = SR_HCP(MY_ADDRESS, 2400, 10, 11);

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


void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);

  delay(500);

  Serial.print("H-CU Startup Address: ");
  Serial.println(MY_ADDRESS);

  delay(100);

  flash(100, 2);

#ifdef MOD_45
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
#endif
#ifdef MOD_47
  for(int i = 4; i <= 7; i++)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, true);
  }
#endif

}

void loop()
{
  byte fromAddress, data;

  if (hcp.hcpReceive(&fromAddress, &data, true)) //-> returns true if data needs to be interpret.
    received(fromAddress, data);

  if (hcp.didPropertyChange())
  {
    Serial.println("Property did change");
    
    update();
  }

  flash(50);

  delay(10);
}

void update()
{
#ifdef MOD_45
    analogWrite(6, hcp.properties[0]);
    analogWrite(5, hcp.properties[1]);
    analogWrite(9, hcp.properties[2]);
#endif
}

void received(int fromAddress, int data)
{
  if (data <= VALUE_RANGE_MAX)
  {
    // Interpret data

    hcp.respond(interpret(data));

    //hcp.hcpSend(fromAddress, interpret(data)); // 255 = OK
  }
  else if (data == CMD_PING)
  {
    hcp.respondOkey();
    //hcp.hcpSend(fromAddress, ID_MCU_OKEY);
  }
  else
  {
    hcp.respondUnknown();
    //hcp.hcpSend(fromAddress, ID_MCU_UNKNOWN); // 254 = UNKNOWN
  }

  flash(50);
}

// Interpret data between 0 and VALUE_RANGE_MAX, commands are not included.
byte interpret(int val)
{
#ifdef MOD_47
  if (val >= 0 && val < 4)
  {
    digitalWrite(val + 4, state[val]);
    Serial.println("Output " + String(val + 4) + " is " + String(state[val]));
  }
  state[val] = !state[val];
  return 255;
#endif

  return 254;
}



