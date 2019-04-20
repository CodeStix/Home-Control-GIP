#include <SoftwareSerial.h>
#include <EEPROM.h>
#include "Packet.h"
#include "PacketSenderReceiver.h"

/****** Uncomment the current slave, comment others ******/
//#define SLAVE_NANO4LED
#define SLAVE_PROMINIBLUE

/****** Unique for each slave ******/
// UNIQUE_FACTORY_ID: An 7-byte integer to identify each slave node on the planet. (ufid)
// DEVICE_INFO: Non-private information about this slave.
#ifdef SLAVE_NANO4LED
#define UNIQUE_FACTORY_ID {0xFF, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}
#define DEVICE_INFO {0x11, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}
#endif
#ifdef SLAVE_PROMINIBLUE
#define UNIQUE_FACTORY_ID {0xFF, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0}
#define DEVICE_INFO {0x12, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}
#endif

/****** NOT Unique for each slave ******/
#define DEBUG_PIN LED_BUILTIN
// Note: HC12 TX to RX and RX to TX.
#define TX_PIN 10
#define RX_PIN 11
#define PROPERTY_COUNT 128

unsigned char getAddress();

SoftwareSerial ss = SoftwareSerial(RX_PIN, TX_PIN);
PacketSenderReceiver sr = PacketSenderReceiver(&ss, true, getAddress());
Packet temp;

//__attribute__((section(".noinit")))
unsigned char startupMode = 0;
unsigned char properties[PROPERTY_COUNT];

unsigned long lastLedBlink = 0;
unsigned int ledBlinks = 0;
unsigned int ledBlinkInterval = 200;
void led(int blinks, int interval = 200)
{
  ledBlinks = blinks * 2;
  ledBlinkInterval = interval;
}

void setupSlave()
{
#ifdef SLAVE_NANO4LED
  for (unsigned char i = 2; i <= 9; i++)
  {
    pinMode(i, OUTPUT);
  }
#endif
}

void propertyUpdate()
{
#ifdef SLAVE_NANO4LED
  for (unsigned char i = 2; i <= 9; i++)
  {
    digitalWrite(i, properties[i] > 0);
  }
#endif
}

void setup()
{
  pinMode(DEBUG_PIN, OUTPUT);
  digitalWrite(DEBUG_PIN, false);

  Serial.begin(19200);
  veryCoolSplashScreen();
  Serial.print("My address (slave): ");
  Serial.println(getAddress());
  Serial.print("Registered: ");
  Serial.println(getRegistered() ? "yeah" : "nope");
  Serial.print("My master address: ");
  Serial.println(getMaster());
  Serial.print("Unique factory id (ufid): ");
  unsigned char ufid[7] = UNIQUE_FACTORY_ID;
  for (unsigned char i = 0; i < 7; i++)
  {
    Serial.print(ufid[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.print("Starting mode: ");
  Serial.println(++startupMode);
  Serial.println("Starting...");

  setupSlave();

  delay(500);

  ss.begin(2400);

  if (!getRegistered())
    led(500000);
}

void loop()
{
  if (ledBlinks > 0 && (millis() - lastLedBlink) > ledBlinkInterval)
  {
    digitalWrite(DEBUG_PIN, ledBlinks % 2 == 0);

    ledBlinks--;
    lastLedBlink = millis();
  }

  if (sr.receive(&temp))
  {
    Serial.print("Received packet: ");
    temp.printToSerial();
    Serial.println();

    if (temp.needsResponse())
    {
      Serial.println("Packet needs response");
      processRequest(temp.getMaster(),  temp.getData(), temp.getDataLength());
    }
  }
}

void processRequest(unsigned char fromMaster, unsigned char* data, unsigned char len)
{
  bool reg = getRegistered();

  // Register command
  if (!reg && len == 9 && data[0] == 0x10)
  {
    unsigned char ufid[7] = UNIQUE_FACTORY_ID;
    for (int i = 0; i < 7; i++)
      if (data[i + 1] != ufid[i])
        return false;

    setMaster(fromMaster);
    setAddress(data[8]);
    sr = PacketSenderReceiver(&ss, true, getAddress());

    Serial.print("Registered master: ");
    Serial.println(getMaster());
    Serial.print("My address: ");
    Serial.println(getAddress());
    led(10, 50);

    unsigned char resp[] = DEVICE_INFO;
    sr.answer(&temp, resp, sizeof(resp));
    return;
  }
  else if (!reg && led == 1 && data[0] == 0x2)
  {
    unsigned char resp[] = {0xFF};
    sr.answer(&temp, resp, sizeof(resp));
  }
  else if (!reg || fromMaster != getMaster())
  {
    return;
  }

  // Set, set-range command
  if (len >= 3 && data[0] == 0x20)
  {
    unsigned char startPos = data[1];
    unsigned char propDataLen = len - 2;

    if (startPos + propDataLen - 1 < PROPERTY_COUNT)
    {
      memcpy(&properties[startPos], &data[2], propDataLen);

      Serial.print(propDataLen);
      Serial.print(" properties were updated: ");
      for (unsigned char i = startPos; i < startPos + propDataLen; i++)
      {
        Serial.print('[');
        Serial.print(i);
        Serial.print(" = ");
        Serial.print(properties[i]);
        Serial.print("] ");
      }
      Serial.println();

      propertyUpdate();

      unsigned char resp[] = {0xFF};
      sr.answer(&temp, resp, sizeof(resp));
      return;
    }
  }
  // Ping command
  else if (len == 1 && data[0] == 0x1)
  {
    Serial.println("<-- Me is got being pinged, yay!");

    unsigned char resp[] = {0xFF};
    sr.answer(&temp, resp, sizeof(resp));
    return;
  }
  // Unregister command
  else if (len == 1 && data[0] == 0x2)
  {
    unsigned char resp[] = {0xFF};
    sr.answer(&temp, resp, sizeof(resp));

    setAddress(0);
    sr = PacketSenderReceiver(&ss, true, getAddress());

    Serial.println("Device is now unbound.");
    led(500000);
    return;
  }

  // Mark request as failed.
  unsigned char resp[] = {0x0};
  sr.answer(&temp, resp, sizeof(resp));
  led(2);
  return;
}

unsigned char getAddress()
{
  return 0xFF - EEPROM.read(0);
}

bool getRegistered()
{
  unsigned char s = getAddress();
  return s > 0 && s < 64;
}

unsigned char getMaster()
{
  return 0xFF - EEPROM.read(1);
}

void setAddress(unsigned char addr)
{
  EEPROM.write(0, 0xFF - addr);
}

void setMaster(unsigned char masterAddress)
{
  EEPROM.write(1, 0xFF - masterAddress);
}

unsigned char getProperty(unsigned char address)
{
  return EEPROM.read(address + 100);
}

void getProperties(unsigned char address, unsigned char* store, unsigned char len)
{
  for (unsigned char i = address, j = 0; i < address + len; i++, j++)
  {
    store[j] = EEPROM.read(i + 100);
  }
}

void setProperty(unsigned char address, unsigned char value)
{
  EEPROM.update(address + 100, value);
}

void setProperties(unsigned char address, unsigned char* values, unsigned char len)
{
  for (unsigned char i = address, j = 0; i < address + len; i++, j++)
  {
    EEPROM.update(i + 100, values[j]);
  }
}

void veryCoolSplashScreen()
{
  Serial.println();
  Serial.println("  _   _      ____    ____    ");
  Serial.println(" |'| |'|  U /\"___| U|  _\"\\ u ");
  Serial.println("/| |_| |\\ \\| | u   \\| |_) |/ ");
  Serial.println("U|  _  |u  | |/__   |  __/   ");
  Serial.println(" |_| |_|    \\____|  |_|      ");
  Serial.println(" //   \\\\   _// \\ \\  ||>>_    ");
  Serial.println("(_\") (\"_) (__)(__) (__)__)");
  Serial.println("Home Control Protocol - v0.4.0");
  Serial.println("\tby Stijn Rogiest 2019 (c)");
  Serial.println();
}
