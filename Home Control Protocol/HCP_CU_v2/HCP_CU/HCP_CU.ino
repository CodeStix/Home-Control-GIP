#include "SR_HCP.h"

#define MEASURE_PIN 9

//#define MOD_45
//#define MOD_46
#define MOD_47
//#define MOD_48

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
#define ID_CU_PING 255
#define ID_CU_MEASUREMENT 254
/* Controlled unit */

#define LED_GREEN 3
#define LED_RED 2

SR_HCP hcp = SR_HCP(MY_ADDRESS, 2400, 10, 11);
byte measure = 0;

void flash(int mil, int cycles = 1)
{
  for (int i = 0; i < cycles; i++)
  {
    digitalWrite(LED_GREEN, true);
    digitalWrite(LED_RED, true);
    delay(mil / 2);
    digitalWrite(LED_GREEN, false);
    digitalWrite(LED_RED, false);
    delay(mil / 2);
  }
}


void setup()
{
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  //pinMode(BUTTON, INPUT_PULLUP);

  Serial.begin(9600);

  delay(500);

  Serial.print("H-CU Startup Address: ");
  Serial.println(MY_ADDRESS);

  delay(100);

  flash(100, 5);

#ifdef MOD_45

  pinMode(CURRENT_PIN, OUTPUT);

#endif
}

void loop()
{
  int fromAddress, data;

  if (hcp.hcpReceive(&fromAddress, &data, true)) //-> returns true if data needs to be interpret.
    received(fromAddress, data);

  flash(50);

  delay(10);
}

void received(int fromAddress, int data)
{
  flash(50);

  if (data < VALUE_RANGE_MAX)
  {
    // Interpret data
    
    hcp.respond(interpret(data));
    
    //hcp.hcpSend(fromAddress, interpret(data)); // 255 = OK
  }
  else if (data == ID_CU_PING)
  {
    hcp.respondOkey();
    //hcp.hcpSend(fromAddress, ID_MCU_OKEY);
  }
  else if (data == ID_CU_MEASUREMENT)
  {
    hcp.respondFailed();
    //hcp.hcpSend(fromAddress, measure);
  }
  else
  {
     hcp.respondUnknown();
    //hcp.hcpSend(fromAddress, ID_MCU_UNKNOWN); // 254 = UNKNOWN
  }
}

// Interpret data between 0 and VALUE_RANGE_MAX, commands are not included.
byte interpret(int val)
{

#ifdef MOD_47

  if (val >= 0 && val < 4)
  {
    pinMode(val + 4, OUTPUT);
    digitalWrite(val + 4, state[val]);

    state[val] = !state[val];
  }
  return ;

#endif

  return 255;
}



