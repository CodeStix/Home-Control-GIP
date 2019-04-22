# 1 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino"
# 1 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino"
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

    https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Keep-Alive

*/
# 21 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino"
# 22 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino" 2
# 23 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino" 2
# 24 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino" 2
# 25 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino" 2
# 26 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino" 2
# 27 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino" 2
# 28 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino" 2
# 29 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino" 2
# 30 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino" 2
# 31 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino" 2


// Note: HC12 TX to RX and RX to TX


// This masters address, can be 1, 2 or 3.




SoftwareSerial ss = SoftwareSerial(12, 14);
PacketSenderReceiver sr = PacketSenderReceiver(&ss, false, 2);
Packet temp;
Device* devices[64];

ESP8266WiFiMulti wifiMulti;
WiFiServer server(80);
WiFiClient client;
String clientData;
//ESP8266WebServer server(80);

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
  pinMode(2, 0x01);
  digitalWrite(2, false);

  Serial.begin(19200);
  veryCoolSplashScreen();
  Serial.print("----> My address (master): ");
  Serial.println(2);
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
  if (MDNS.begin("homecontrol")) // Start the mDNS responder for esp8266.local
  {
    Serial.print("\t-> mDNS responder started: ");
    Serial.println("homecontrol");
  }
  else
  {
    Serial.println("\t-> FATAL: Error setting up MDNS responder!");
  }
  server.begin();
  /*server.on("/help", handleRootWebPage);

  server.onNotFound(handleNotFoundPage);

  server.begin();*/
# 110 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino"
  Serial.println("----> Starting...");
  delay(500);
  ss.begin(2400);
  Serial.println("\t-> OK");
}

/*void handleRootWebPage()

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

  server.sendContent("jatest\n");

}*/
# 155 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino"
void loop()
{
  if (ledBlinks > 0 && (millis() - lastLedBlink) > ledBlinkInterval)
  {
    digitalWrite(2, ledBlinks % 2 == 0);

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

  //server.handleClient();

  WiFiClient newClient = server.available();
  if (newClient && (newClient != client) && (!client || !client.connected()))
  {
    Serial.println("New client!");
    client = newClient;
    clientData = "";
  }

  while (client && client.available())
  {
    char c = client.read();

    if (c == '\r')
      continue;

    clientData += c;

    if (clientData.length() > 2 && c == '\n' && clientData[clientData.length() - 2] == '\n')
    {
      Serial.println("End of request received. Responsing...");

      int i = clientData.indexOf("GET "), j = clientData.indexOf(" HTTP/");
      bool close = true;
      if (i >= 0 && j >= 0)
      {
        String request = clientData.substring(i + 4, j);
        request.trim();
        Serial.println("REQUEST: " + request);

        if (request == "/ping")
        {
          close = false;
          pingSlave(27, false);
        }
      }

      // HEADER
      client.println("HTTP/1.1 200 OK");
      client.println("Connection: Keep-Alive");
      client.println("Keep-Alive: timeout=15, max=1000");
      client.println("Content-type: text/html");
      client.println();
      // CSS + HTML HEAD
      client.println("<!DOCTYPE html><html>");
      client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
      client.println("<link rel=\"icon\" href=\"data:,\">");
      client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
      client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
      client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
      client.println(".button2 { background-color: #77878A; }</style></head>");
      // HTML
      client.println("<body><h1>Home Control</h1>");
      client.println("<p>TESTING:</p>");
      client.println("<p><a href=\"/nice\"><button class=\"button\">OKE COOL</button></a></p>");
      client.println("</body></html>");
      client.println();

      if (close)
        client.stop();
    }
  }
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

      for(unsigned char i = 0; i < 64; i++)
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
    else if (argsLen >= 3 && argsLen <= 9 && args[1] == "bind")
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

  if (client && client.connected())
  {
    client.print("pinged: ");
    client.println(status);
    client.stop();
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
  for(unsigned char i = 0; i < 64; i++)
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

  for(unsigned char i = 0; i < 64; i++)
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

  if (i >= 64)
    i = 0;

  for(; i < 64; i++)
  {
    if (devices[i] && devices[i]->working)
    {
      /*Serial.print("----> Checking if device ");

      devices[i]->printToSerial();

      Serial.println(" is online...");*/
# 532 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino"
      pingSlave(devices[i]->address, true);

      i++;
      break;
    }
  }
}

void retryNotWorkingBinds()
{
  static unsigned char i = 0;

  if (i >= 64)
    i = 0;

  for(; i < 64; i++)
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
# 602 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino"
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
  for(unsigned char i = 0; i < 64; i++)
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
# 638 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino"
  unsigned char deviceCount = 0;

  for (int i = 0; i < 64; i++)
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
# 657 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino"
      deviceCount++;
    }
  }

  Serial.print("\t-> ");
  Serial.print(deviceCount);
  Serial.println(" devices were loaded from ROM.");
}

void clearRomDevices()
{
  for (int i = 100; i < 100 + 64 * 50; i++)
    EEPROM.write(i, 0xFF);
  for(unsigned char i = 0; i < 64; i++)
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

  for (int i = 0; i < 64; i++)
  {
    if (devices[i])
    {
      /*Serial.print("Saving device ");

      Serial.print(i);

      Serial.print(": ");

      devices[i]->printToSerial();

      Serial.println();*/
# 697 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino"
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
  for(unsigned char i = 0; i < 64; i++)
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
  for(unsigned char i = 0; i < 64; i++)
  {
    if (devices[i] && devices[i]->address == addr)
      return devices[i];
  }

  return nullptr;
}
