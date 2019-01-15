#include <SoftwareSerial.h>

#define USE_HARDWARE_SERIAL true

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

/* Main control unit */

#define LED_GREEN 13
#define LED_RED 15
#define BUTTON 16
#define SELECT_BUTTON 14
#define SELECT_BUTTON2 12

SoftwareSerial HC12(10, 11);

int myAddress = 0;
int toAddress = 0;

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

bool hcpReceive(int *fromAddress, int *data, bool sync = true)
{
  if (sync)
    while (hcpRawAvailable() < 4);
  else if (hcpRawAvailable() < 4)
    return false;
  
  if (hcpRawPeek() != 0xf) // Out of sync
  {
    Serial.print("Out of sync (");
    Serial.print(hcpRawPeek(), HEX);
    Serial.print(" != 0xf)... ");

    int r = 0;

    while (r != 0xf)
    {
      r = hcpRawRead();

      while (!hcpRawAvailable());

      r = hcpRawPeek();
    }

    Serial.println("Fixed");

    return hcpReceive(fromAddress, data);
  }

  hcpRawRead();

  *fromAddress = hcpRawRead();
  int toAddress = hcpRawRead();
  *data = hcpRawRead();

  if (toAddress != myAddress)
  {
     return hcpReceive(fromAddress, data);
  }

  flash(25, 1);

  Serial.print("Start receiving: From=");
  Serial.print(*fromAddress);
  Serial.print(", To=");
  Serial.print(toAddress);
  Serial.print(", Data=");
  Serial.println(*data);

  return true;
}

void hcpSend(int toAddress, int data)
{
  hcpRawSend(0xf);
  hcpRawSend(myAddress);
  hcpRawSend(toAddress);
  hcpRawSend(data);
}

void hcpRawSend(int data)
{
  if (USE_HARDWARE_SERIAL)
  {
    Serial.write(data);
  }
  else
  {
    HC12.write(data);
  }
}

int hcpRawRead()
{
  if (USE_HARDWARE_SERIAL)
  {
    return Serial.read();
  }
  else
  {
    return HC12.read();
  }
}

int hcpRawAvailable()
{
  if (USE_HARDWARE_SERIAL)
  {
    return Serial.available();
  }
  else
  {
    return HC12.available();
  }
}

int hcpRawPeek()
{
  if (USE_HARDWARE_SERIAL)
  {
    return Serial.peek();
  }
  else
  {
    return HC12.peek();
  }
}




void setup()
{
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUTTON, INPUT);
  pinMode(SELECT_BUTTON, INPUT);
  pinMode(SELECT_BUTTON2, INPUT);

  //Serial.begin(2400);

  Serial.begin(9600);
  HC12.begin(2400);

  flash(50, 10);
}

void connect()
{


}

void loop()
{
  toAddress = 45 + 1 * digitalRead(SELECT_BUTTON) + 2 * digitalRead(SELECT_BUTTON2);

  if (digitalRead(BUTTON))
  {
    hcpSend(toAddress, random(4, 8));

    while (digitalRead(BUTTON));
  }

  int fromAddress, data;

  if (hcpReceive(&fromAddress, &data, false))
    received(fromAddress, data);

  delay(10);
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


