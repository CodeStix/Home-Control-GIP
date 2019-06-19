#include <SoftwareSerial.h>
#include <EEPROM.h>
#include "Shared.h"
#include "Packet.h"
#include "PacketSenderReceiver.h"

/****** Uncomment the current slave, comment others ******/
#define SLAVE_1

/****** Unique for each slave ******/
// UNIQUE_FACTORY_ID: An 7-byte integer to identify each slave node on the planet. (ufid)
// DEVICE_TYPE: [switch button count] [push button count] [slider count] [nothing]
/* Ledstrip */
#ifdef SLAVE_1
#define UNIQUE_FACTORY_ID {0xEE, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0}
#define DEVICE_TYPE {0, 0, 3, 0}
//#include <FastLED.h>
//#define LEDSTRIP_NUM_LEDS 30
//CRGB leds[LEDSTRIP_NUM_LEDS];
#endif
/* Addressable Ledstrip */
#ifdef SLAVE_2
#define UNIQUE_FACTORY_ID {0xEE, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0}
#define DEVICE_TYPE {0, 0, 5, 0}
#endif
/* Relay module 1 */
#ifdef SLAVE_3
#define UNIQUE_FACTORY_ID {0xEE, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0}
#define DEVICE_TYPE {8, 0, 0, 0}
#endif
/* Relay module 2 */
#ifdef SLAVE_4
#define UNIQUE_FACTORY_ID {0xEE, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0}
#define DEVICE_TYPE {8, 0, 0, 0}
#endif
/* Led Matrix */
#ifdef SLAVE_5
#define UNIQUE_FACTORY_ID {0xEE, 0x5, 0x0, 0x0, 0x0, 0x0, 0x0}
#define DEVICE_TYPE {0, 0, 1, 0}
#include "LedControl.h"
LedControl lc = LedControl(3, 5, 6, 4);
#endif

/****** NOT Unique for each slave ******/
#define DEBUG_PIN LED_BUILTIN
#define STATUS_LED_PIN 13
// Note: HC12 TX to RX and RX to TX.
#define TX_PIN 11
#define RX_PIN 10
#define PROPERTY_COUNT 64
#define MAX_CONCURRENT_REQUESTS 2
#define BAUD_COMMUNICATE 2400
#define BAUD_SERIAL 19200

unsigned char getAddress();

SoftwareSerial ss = SoftwareSerial(RX_PIN, TX_PIN);
PacketSenderReceiver sr = PacketSenderReceiver(&ss, true, getAddress());
Packet temp;

//__attribute__((section(".noinit")))
unsigned char startupMode = 0;
//unsigned char properties[PROPERTY_COUNT];

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
#ifdef SLAVE_1
  //FastLED.addLeds<WS2812B, 3, GRB>(leds, LEDSTRIP_NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.setBrightness(100);
#endif
#ifdef SLAVE_2
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
#endif
#ifdef SLAVE_3
  for (int i = 0; i < 8; i++)
  {
    pinMode(i + 2, OUTPUT);
  }
#endif
#ifdef SLAVE_4
  for (int i = 0; i < 8; i++)
  {
    pinMode(i + 2, OUTPUT);
  }
#endif
#ifdef SLAVE_5
  for (int i = 0; i < 4; i++)
  {
    lc.shutdown(i, false);
    /* Set the brightness to a medium values */
    lc.setIntensity(i, 8);
    /* and clear the display */
    lc.clearDisplay(i);
  }
#endif
}

void propertyUpdate()
{
#ifdef SLAVE_1
  // Set adressable ledstrip
#endif
#ifdef SLAVE_2
  analogWrite(3, getProperty(0));
  analogWrite(5, getProperty(1));
  analogWrite(6, getProperty(2));
#endif
#ifdef SLAVE_3
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(i + 2, getProperty(i));
  }
#endif
#ifdef SLAVE_4
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(i + 2, getProperty(i));
  }
#endif
#ifdef SLAVE_5
  lc.setLed(0, 0, 0, true);
#endif
}

void slaveLoop()
{
  // Do not delay!

#ifdef SLAVE_1
  //static unsigned char hue = 0;
  //fill_rainbow(leds, LEDSTRIP_NUM_LEDS, hue++, 7);
  //FastLED.show();  
#endif

}

unsigned char refreshLiveData(unsigned char liveData[16])
{
  for (unsigned char i = 1; i <= 4; i++)
    liveData[i - 1] = i;


  return 0;
}

void setup()
{
  pinMode(DEBUG_PIN, OUTPUT);
  pinMode(STATUS_LED_PIN, OUTPUT);
  digitalWrite(DEBUG_PIN, false);
  digitalWrite(STATUS_LED_PIN, true);

  Serial.begin(BAUD_SERIAL);
  veryCoolSplashScreen();
  Serial.print("----> My address (slave): ");
  Serial.println(getAddress());
  Serial.print("----> Registered: ");
  Serial.println(getRegistered() ? "yeah" : "nope");
  Serial.print("----> My master address: ");
  Serial.println(getMaster());
  Serial.print("----> Unique factory id (ufid): ");
  unsigned char ufid[7] = UNIQUE_FACTORY_ID;
  for (unsigned char i = 0; i < 7; i++)
  {
    Serial.print(ufid[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.print("----> Starting mode: ");
  Serial.println(++startupMode);
  Serial.println("----> Starting...");

  setupSlave();
  propertyUpdate();

  ss.begin(BAUD_COMMUNICATE);

  //setAddress(0);
  if (!getRegistered())
    led(5000000);

  Serial.println("\t-> OK");
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
      processRequest(temp.getMaster(),  temp.getData(), temp.getDataLength(), temp.getSlave() == 0);
    }
  }

  slaveLoop();
}

void processRequest(unsigned char fromMaster, unsigned char* data, unsigned char len, bool isBroadcast)
{
  bool reg = getRegistered();

  // Bind command when unregistered
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

    unsigned char resp[] = DEVICE_TYPE;
    sr.answer(&temp, resp, sizeof(resp));
    return;
  }
  else if (!reg || fromMaster != getMaster() || isBroadcast)
  {
    if (isBroadcast)
      Serial.println("Broadcast got ignored (WIP?)");

    return;
  }

  // Set, set-range command
  if (len >= 3 && data[0] == 0x20)
  {
    unsigned char startPos = data[1];
    unsigned char propDataLen = len - 2;

    if (startPos + propDataLen - 1 < PROPERTY_COUNT)
    {
      //memcpy(&properties[startPos], &data[2], propDataLen);

      setProperties(startPos, &data[2], propDataLen);

      Serial.print(propDataLen);
      Serial.print(" properties were updated: ");
      for (unsigned char i = startPos; i < startPos + propDataLen; i++)
      {
        Serial.print('[');
        Serial.print(i);
        Serial.print(" = ");
        //Serial.print(properties[i]);
        Serial.print(getProperty(i));
        Serial.print("] ");
      }
      Serial.println();
      led(2, 25);

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
    led(25, 50);

    unsigned char resp[] = {0xFF};
    sr.answer(&temp, resp, sizeof(resp));
    return;
  }
  // Unbind command
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
  // Bind command while registered
  else if (len == 9 && data[0] == 0x10)
  {
    Serial.print("Device bind request while bound: ");

    unsigned char ufid[7] = UNIQUE_FACTORY_ID;
    if (memcmp(&data[1], &ufid[0], 7) == 0)
    {
      Serial.println("for me.");

      unsigned char resp[] = DEVICE_TYPE;
      sr.answer(&temp, resp, sizeof(resp));
      return;
    }
    else
    {
      Serial.println("not for me.");
    }
  }
  // Refresh command.
  else if (len == 1 && data[0] == 0x15)
  {
    static unsigned char resp[16];
    memset(resp, 0, sizeof(resp));

    unsigned char dataLen = refreshLiveData(resp);

    Serial.print("Sending ");
    Serial.print(dataLen);
    Serial.println(" bytes of live data.");

    sr.answer(&temp, resp, dataLen);
    return;
  }

  // Mark request as failed.
  unsigned char resp[] = {0x0};
  sr.answer(&temp, resp, sizeof(resp));
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
