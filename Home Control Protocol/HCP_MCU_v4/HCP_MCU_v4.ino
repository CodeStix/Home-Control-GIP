/*
  Home Control Protocol v0.4.0
    by Stijn Rogiest (copyright 2019)

  Random console characters legend: 
    _: The last packet was resent, caused by faulty integrity at the receiver.
    !: The last request did not get answered and was disposed.
    .: The last request was resent.

  Sources:
    https://tttapa.github.io/ESP8266/Chap10%20-%20Simple%20Web%20Server.html
    https://www.arduino.cc/en/Reference/EEPROM
    http://www.cplusplus.com/doc/tutorial/pointers/
    https://www.arduino.cc/en/Reference/softwareSerial
    https://stackoverflow.com/questions/3698043/static-variables-in-c
    https://randomnerdtutorials.com/esp8266-web-server/
    http://arduino.esp8266.com/stable/package_esp8266com_index.json
    https://en.wikipedia.org/wiki/Multicast_DNS
*/
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include "Packet.h"
#include "PacketSenderReceiver.h"
#include "Device.h"

#define DEBUG_PIN LED_BUILTIN
// Note: HC12 TX to RX and RX to TX
#define TX_PIN 14
#define RX_PIN 12
// This masters address, can be 1, 2 or 3.
#define MASTER_ADDRESS 2
#define MAX_DEVICES 64
#define WIFI_MDNS "esp8266"

SoftwareSerial ss = SoftwareSerial(RX_PIN, TX_PIN);
PacketSenderReceiver sr = PacketSenderReceiver(&ss, false, MASTER_ADDRESS);
Packet temp;
Device* devices[MAX_DEVICES];

ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(80);

const unsigned int retryBindMillisInterval = 20000;
unsigned long lastRetryBindMillis = 1;
const unsigned int pingMillisInterval = 5000;
unsigned long lastPingMillis = 1;

unsigned long lastLedBlink = 0;
unsigned int ledBlinks = 0;
unsigned int ledBlinkInterval = 200;
void led(int blinks, int interval = 200)
{
  ledBlinks = blinks * 2;
  ledBlinkInterval = interval;
}

// Console
unsigned char currentArg = 0;
String args[16];

void setup()
{
  pinMode(DEBUG_PIN, OUTPUT);
  digitalWrite(DEBUG_PIN, false);

  Serial.begin(19200);
  veryCoolSplashScreen();
  Serial.print("----> My address (master): ");
  Serial.println(MASTER_ADDRESS);
  Serial.println("----> Loading devices...");
  EEPROM.begin(4096);
  //clearRomDevices();
  loadDevicesFromRom();
  printDevices();

  Serial.print("----> Connecting to WiFi");
  wifiMulti.addAP("RogiestHuis", "Vrijdag1!");
  while (wifiMulti.run() != WL_CONNECTED) 
  {
    delay(250);
    Serial.print('.');
  }
  Serial.println();
  Serial.print("----> Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("----> IP address: ");
  Serial.println(WiFi.localIP());
  if (MDNS.begin(WIFI_MDNS))  // Start the mDNS responder for esp8266.local
  {             
    Serial.print("\t-> mDNS responder started: ");
    Serial.println(WIFI_MDNS);
  } 
  else 
  {
    Serial.println("\t-> FATAL: Error setting up MDNS responder!");
  }
  server.on("/help", handleRootWebPage);
  server.onNotFound(handleNotFoundPage);
  server.begin();
  Serial.println("----> Starting...");
  delay(500);
  ss.begin(2400);
  Serial.println("\t-> OK");
}

void handleRootWebPage()
{
  server.send(200, "text/plain", "This is a help page.");
}

void handleNotFoundPage()
{
  String page = server.uri();
  String splitted[20];
  int len = 1;

  for(int i = 1, j = 0; i < page.length() && j < 20; i++)
  {
    char c = page[i];
    if (c == '/')
    {
      j++;
      len++;
      continue;
    }
    else
    {
      splitted[j] += c;
    }
  }

  String content = "esp8266\n";

  if (splitted[0] == "command")
  {
    command(&splitted[1], len - 1);

    content += "command execution\n";
  }

  server.send(404, "text/plain", content);
}

void loop()
{
  if (ledBlinks > 0 && (millis() - lastLedBlink) > ledBlinkInterval)
  {
    digitalWrite(DEBUG_PIN, ledBlinks % 2 == 0);

    ledBlinks--;
    lastLedBlink = millis();
  }

  if (Serial.available() > 0)
  {
    char c = Serial.read();
    if (c == ' ' || c == ',')
    {
      if (args[currentArg].length() > 0)
      {
        currentArg++;
        args[currentArg] = "";
      }
    }
    else if (c == ';' || c == '\n')
    {
      command(args, currentArg + 1);

      currentArg = 0;
      args[currentArg] = "";
    }
    else
    {
      args[currentArg] += c;
    }
  }

  if (sr.receive(&temp))
  {
    //Serial.print("Received packet: ");
    //temp.printToSerial();
    //Serial.println();
    led(1);

    // Slave is bound.
    if (temp.getMultiPurposeByte() == 130)
    {
      Serial.print("Received bind response from ");
      Serial.println(temp.getSlave());

      Device* bound = getDeviceWithAddress(temp.getSlave());

      if (bound)
      {
          Serial.print("----> Slave is now getting bound (1): ");
          bound->printToSerial();
          Serial.println();

          bound->working = true;
          bound->online = true;
          memcpy(bound->deviceInfo, temp.getData(), temp.getDataLength());
          saveDevicesToRom();

          Serial.print("----> Slave is now bound (2): ");
          bound->printToSerial();
          Serial.println();
      }
      else
      {
        Serial.println("----> FATAL: Count not let slave work!");
      }
    }
  }

  sr.resendUnansweredRequests();

  if ((millis() - lastRetryBindMillis) > retryBindMillisInterval)
  {
    retryNotWorkingBinds();

    lastRetryBindMillis = millis();
  }

  if ((millis() - lastPingMillis) > pingMillisInterval)
  {
    checkOnlineBinds();

    lastPingMillis = millis();
  }

  server.handleClient();
}

void command(String args[16], unsigned char argsLen)
{
  if (argsLen >= 4 && args[0] == "prop")
  {
    Serial.print("----> Trying to set property ");
    unsigned char addr = args[1].toInt();
    unsigned char startPos = args[2].toInt();
    unsigned char value = args[3].toInt();
    unsigned char data[16] = {0x20, startPos};
    for (unsigned char i = 0; i < argsLen - 3; i++)
    {
      data[i + 2] = args[i + 3].toInt();

      Serial.print('[');
      Serial.print(startPos + i);
      Serial.print(" = ");
      Serial.print(args[i + 3]);
      Serial.print("] ");
    }
    Serial.print("of address ");
    Serial.println(addr);

    sr.sendRequest(addr, propertySetAnswer, data, argsLen - 1);
  }
  else if (argsLen == 2 && args[0] == "ping")
  {
    pingSlave(args[1].toInt(), false);
  }
  else if (args[0] == "device")
  {
    if (argsLen == 1 || args[1] == "list")
    {
      printDevices();
    }
    else if (argsLen == 2 && args[1] == "unbindall")
    {
      Serial.println("----> Unbinding all slaves, please wait...");

      for(unsigned char i = 0; i < MAX_DEVICES; i++)
      {
        if (devices[i])
        {
          Serial.print("Unbinding slave ");
          Serial.print(devices[i]->address);
          Serial.println("...");

          unbindSlave(devices[i]->address);
          delay(300);
        }
      }
      clearRomDevices();

      Serial.println("----> All bound slaves are now not bound anymore.");
    }
    else if (argsLen >= 3 && argsLen <= 9 &&  args[1] == "bind")
    {
      unsigned char ufid[7];
      memset(ufid, 0x0, sizeof(ufid));
      Serial.print("----> Binding slave with ufid [");
      for (unsigned char i = 2; i < argsLen; i++)
      {
        ufid[i - 2] = args[i].toInt();

        Serial.print(ufid[i - 2]);
        Serial.print(' ');
      }
      Serial.println(']');

      bindSlave(ufid);
    }
    else if (argsLen == 3 && args[1] == "unbind")
    {
      unsigned char addr = args[2].toInt();

      Serial.print("----> Unbinding slave ");
      Serial.print(addr);
      Serial.println("...");

      unbindSlave(addr);
    }
    else
    {
      Serial.println("Command syntax invalid: device [list|bind <ufid...>|unbind <addr>|unbindall]");
    }
  }
  else
  {
    Serial.print("Unknown command: ");
    Serial.print(args[0]);
    Serial.print(" (");
    Serial.print(argsLen);
    Serial.println(")");
  }
}

void setSlaveProperties(unsigned char addr, unsigned char startPos, unsigned char* values, unsigned char valueCount)
{
  if (valueCount == 0)
    return;

  unsigned char data[16] = {0x20, startPos};
  for (unsigned char i = 0; i < valueCount && i < 14; i++)
    data[i + 2] = values[i];
  sr.sendRequest(addr, propertySetAnswer, data, valueCount + 2);
}

void propertySetAnswer(ResponseStatus status, Request* requested)
{
  if (status == Okay)
  {
    Serial.print("\t-> Propery for slave ");
    Serial.print(requested->fromAddress);
    Serial.println(" was set successfully!");
  }
}

void pingSlave(unsigned char addr, bool silent)
{
  if (silent)
  {
    unsigned char data[1] = {0x1};

    sr.sendRequest(addr, pingAnswer, data, sizeof(data));
  }
  else
  {
    unsigned char data[2] = {0x1, 0x0};

    sr.sendRequest(addr, pingAnswer, data, sizeof(data));
  }
}

void pingAnswer(ResponseStatus status, Request* requested)
{
  if (requested->sentDataLength == 2)
  {
    Serial.print("\t-> Slave ");
    Serial.print(requested->fromAddress);
    Serial.print(" was pinged: ");
    Serial.println(status == Okay ? "Okay" : (status == Failed ? "Failed" : "No response"));
  }

  Device* dev = getDeviceWithAddress(requested->fromAddress);
  if (dev)
  {
    bool online = status == Okay;

    if (dev->online != online)
    {
      dev->online = online;
      saveDevicesToRom();
    }
  }
}

bool bindSlave(unsigned char ufid[7])
{
  return bindSlave(ufid, getNewAddress());
}

bool bindSlave(unsigned char ufid[7], unsigned char withAddress)
{
  for(unsigned char i = 0; i < MAX_DEVICES; i++)
  {
    if (devices[i] && (devices[i]->address == withAddress || memcmp(ufid, devices[i]->uniqueFactoryId, 7) == 0))
    {
      Serial.println("----> Warning: tried to bind 2 slaves with either the same address or ufid.");

      return false;
    }
  }

  unsigned char data[9];
  memcpy(&data[1], &ufid[0], 7);
  data[0] = 0x10;
  data[8] = withAddress;
  sr.broadcast(data, sizeof(data), DataRequest, 130); // Multi-purpose-byte is 130, slave will return 130.

  registerNewDevice(ufid, withAddress);
  saveDevicesToRom();
  return true;
}

void unbindSlave(unsigned char withAddress)
{
  unsigned char data[1] = { 0x2 };
  sr.sendRequest(withAddress, unbindAnswer, data, sizeof(data));

  for(unsigned char i = 0; i < MAX_DEVICES; i++)
  {
    if (devices[i] && devices[i]->address == withAddress)
    {
       delete devices[i];
       devices[i] = nullptr;

       saveDevicesToRom();

       Serial.println("\t-> Device is unregistered, waiting for unbind request... (no answer is ok)");
       break;
    }
  }
}

void checkOnlineBinds()
{
  static unsigned char i = 0;

  if (i >= MAX_DEVICES)
    i = 0;

  for(; i < MAX_DEVICES; i++)
  {
    if (devices[i] && devices[i]->working)
    {
      /*Serial.print("----> Checking if device ");
      devices[i]->printToSerial();
      Serial.println(" is online...");*/

      pingSlave(devices[i]->address, true);

      i++;
      break;
    }
  }
}

void retryNotWorkingBinds()
{
  static unsigned char i = 0;

  if (i >= MAX_DEVICES)
    i = 0;

  for(; i < MAX_DEVICES; i++)
  {
    if (devices[i] && !(devices[i]->working))
    {
      Serial.print("----> Trying to let device ");
      devices[i]->printToSerial();
      Serial.println(" work...");

      unsigned char data[9];
      memcpy(&data[1], devices[i]->uniqueFactoryId, 7);
      data[0] = 0x10;
      data[8] = devices[i]->address;
      sr.broadcast(data, sizeof(data), DataRequest, 130);

      i++;
      break;
    }
  }
}

unsigned char getNewAddress()
{
  unsigned char s = EEPROM.read(0);
  EEPROM.write(0, ++s);
  return s;
}

void unbindAnswer(ResponseStatus status, Request* requested)
{
  if (status == Okay)
  {
    Serial.print("\t-> Slave ");
    Serial.print(requested->fromAddress);
    Serial.println(" was successfully unbound from this master.");
  }
}

// Obsolete!
/*void answer(ResponseStatus status, Request* requested)
{
  if (status == NoResponse)
    Serial.print("Packet did not get answered: ");
  else if (status == Failed)
    Serial.print("Packet got answered (failed): ");
  else
    Serial.print("Packet got answered (okey): ");

  for (int i = 0; i < requested->responseLength; i++)
  {
    Serial.print(requested->response[i]);
    Serial.print(' ');
  }
  Serial.println();
}*/

void veryCoolSplashScreen()
{
  Serial.println();
  Serial.println("    _   _      ____    ____    ");
  Serial.println("   |'| |'|  U /\"___| U|  _\"\\ u ");
  Serial.println("  /| |_| |\\ \\| | u   \\| |_) |/ ");
  Serial.println("  U|  _  |u  | |/__   |  __/   ");
  Serial.println("   |_| |_|    \\____|  |_|      ");
  Serial.println("   //   \\\\   _// \\ \\  ||>>_    ");
  Serial.println("  (_\") (\"_) (__)(__) (__)__)");
  Serial.println("Home Control Protocol - v0.4.0");
  Serial.println("\tby Stijn Rogiest (c) 2019");
  Serial.println();
}

void printDevices()
{
  Serial.println("----> List of devices that are controlled by this master:");
  unsigned char deviceCount = 0;
  for(unsigned char i = 0; i < MAX_DEVICES; i++)
  {
    if (devices[i])
    {
      Serial.print("\t");
      Serial.print(++deviceCount);
      Serial.print(": ");
      devices[i]->printToSerial();
      Serial.println();
    }
  }
}

void loadDevicesFromRom()
{
  /*Serial.print("Size of device: ");
  Serial.println(sizeof(Device));*/
  unsigned char deviceCount = 0;

  for (int i = 0; i < MAX_DEVICES; i++)
  {
    if (EEPROM.read(i * 50 + 100 + 50 - 1) == 0xFF)
    {
      // Device save location is empty
      devices[i] = nullptr;
    }
    else
    {
      // Device save location is used, read it
      unsigned char bytes[50];
      for(int j = 0; j < 50; j++)
          bytes[j] = EEPROM.read(i * 50 + 100 + j);
      devices[i] = new Device(bytes);
      /*Serial.print("Red device: ");
      devices[i]->printToSerial();
      Serial.println();*/
      deviceCount++;
    }
  }
  
  Serial.print("\t-> ");
  Serial.print(deviceCount);
  Serial.println(" devices were loaded from ROM.");
}

void clearRomDevices()
{
  for (int i = 100; i < 100 + MAX_DEVICES * 50; i++)
    EEPROM.write(i, 0xFF);
  for(unsigned char i = 0; i < MAX_DEVICES; i++)
  {
    if (devices[i])
    {
      delete devices[i];
      devices[i] = nullptr;
    }
  }

  EEPROM.commit();

  Serial.println("\t-> All devices were ereased from ROM.");
}

void saveDevicesToRom()
{
  unsigned char deviceCount = 0;

  for (int i = 0; i < MAX_DEVICES; i++)
  {
    if (devices[i])
    {
      /*Serial.print("Saving device ");
      Serial.print(i);
      Serial.print(": ");
      devices[i]->printToSerial();
      Serial.println();*/
      unsigned char* bytes = devices[i]->getBytes();
      for(int j = 0; j < 50; j++)
          EEPROM.write(i * 50 + 100 + j, bytes[j]);
      deviceCount++;
    }
    else
    {
      EEPROM.write(i * 50 + 100 + 50 - 1, 0xFF);
    }
  }

  EEPROM.commit();

  Serial.print("\t-> ");
  Serial.print(deviceCount);
  Serial.println(" devices were saved to ROM.");
}

Device* registerNewDevice(unsigned char ufid[7], unsigned char addr)
{
  for(unsigned char i = 0; i < MAX_DEVICES; i++)
  {
    if (!devices[i])
    {
      devices[i] = new Device(ufid, addr, "Test");

      return devices[i];
    }
  }

  return nullptr;
}

Device* getDeviceWithAddress(unsigned char addr)
{
  for(unsigned char i = 0; i < MAX_DEVICES; i++)
  {
    if (devices[i] && devices[i]->address == addr)
      return devices[i];
  }

  return nullptr;
}