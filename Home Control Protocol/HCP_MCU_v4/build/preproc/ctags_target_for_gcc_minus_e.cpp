# 1 "/Users/stijnrogiest/Documents/GitHub/Home-Control-GIP/Home Control Protocol/HCP_MCU_v4/HCP_MCU_v4.ino"
# 1 "/Users/stijnrogiest/Documents/GitHub/Home-Control-GIP/Home Control Protocol/HCP_MCU_v4/HCP_MCU_v4.ino"
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
    https://en.wikipedia.org/wiki/Cyclic_redundancy_check#CRC-32_algorithm

  Packet types/prefixes:
    0x20: Set slave properties.
    0x1: Ping slave.
    0x15: Refresh slave live data.
    0x10: Bind slave.
    0x2: Unbind slave.
*/

# 30 "/Users/stijnrogiest/Documents/GitHub/Home-Control-GIP/Home Control Protocol/HCP_MCU_v4/HCP_MCU_v4.ino" 2
# 31 "/Users/stijnrogiest/Documents/GitHub/Home-Control-GIP/Home Control Protocol/HCP_MCU_v4/HCP_MCU_v4.ino" 2
# 32 "/Users/stijnrogiest/Documents/GitHub/Home-Control-GIP/Home Control Protocol/HCP_MCU_v4/HCP_MCU_v4.ino" 2


# 35 "/Users/stijnrogiest/Documents/GitHub/Home-Control-GIP/Home Control Protocol/HCP_MCU_v4/HCP_MCU_v4.ino" 2
# 36 "/Users/stijnrogiest/Documents/GitHub/Home-Control-GIP/Home Control Protocol/HCP_MCU_v4/HCP_MCU_v4.ino" 2
# 37 "/Users/stijnrogiest/Documents/GitHub/Home-Control-GIP/Home Control Protocol/HCP_MCU_v4/HCP_MCU_v4.ino" 2
# 38 "/Users/stijnrogiest/Documents/GitHub/Home-Control-GIP/Home Control Protocol/HCP_MCU_v4/HCP_MCU_v4.ino" 2
# 39 "/Users/stijnrogiest/Documents/GitHub/Home-Control-GIP/Home Control Protocol/HCP_MCU_v4/HCP_MCU_v4.ino" 2
# 40 "/Users/stijnrogiest/Documents/GitHub/Home-Control-GIP/Home Control Protocol/HCP_MCU_v4/HCP_MCU_v4.ino" 2
# 41 "/Users/stijnrogiest/Documents/GitHub/Home-Control-GIP/Home Control Protocol/HCP_MCU_v4/HCP_MCU_v4.ino" 2


// Note: HC12 TX to RX and RX to TX


// This masters addr, can be 1, 2 or 3.




SoftwareSerial ss = SoftwareSerial(12, 14);
PacketSenderReceiver sr = PacketSenderReceiver(&ss, false, 2);
Packet temp;
Device* devices[32];

ESP8266WiFiMulti wifiMulti;
WiFiServer server(80);
WiFiClient client;
String clientData;
void* slaveBoundClient = nullptr;

const unsigned int retryBindMillisInterval = 400;
unsigned long lastRetryBindMillis = 1;
const unsigned int refreshMillisInterval = 2220;
unsigned long lastRefreshMillis = 1;

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

// Prototypes
void refreshSlave(unsigned char addr, void* state = nullptr);
void pingSlave(unsigned char addr, void* state = nullptr);
void unbindSlave(unsigned char withAddress, void* state = nullptr);
void setSlaveProperties(unsigned char addr, unsigned char startPos, unsigned char* values, unsigned char valueCount, void * state = nullptr);
bool bindSlave(unsigned char ufid[7], unsigned char withAddress, void* state = nullptr);
bool bindSlave(unsigned char ufid[7], void* state = nullptr);
void rebindSlave(unsigned char ufid[7], unsigned char withAddress);

void setup()
{
  pinMode(2, 0x01);
  digitalWrite(2, false);

  Serial.begin(19200);
  delay(5000);
  veryCoolSplashScreen();
  Serial.print("----> My address (master): ");
  Serial.println(2);
  Serial.println("----> Loading devices...");
  EEPROM.begin(4096);
  //clearRomDevices();
  loadDevicesFromRom();
  printDevices();

  Serial.print("----> Connecting to WiFi");
  wifiMulti.addAP("PollenPatatten", "Ziektes123");
  wifiMulti.addAP("RogiestHuis", "Vrijdag1!");
  wifiMulti.addAP("pollenpattten", "ziektes123");
  wifiMulti.addAP("Stijn Rogiest", "HoiDaag2");
  while (wifiMulti.run() != WL_CONNECTED)
  {
    delay(250);
    Serial.print('.');
  }
  Serial.println();
  Serial.print("----> Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("----> IP addr: ");
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
  Serial.println("----> Starting...");
  delay(500);
  ss.begin(4800);
  Serial.println("\t-> OK");
}

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
        memcpy(bound->deviceType, temp.getData(), temp.getDataLength());
        saveDevicesToRom();

        Serial.print("----> Slave is now bound (2): ");
        bound->printToSerial();
        Serial.println();

        if (slaveBoundClient)
        {
          WiFiClient* wc = (WiFiClient*)slaveBoundClient;
          wc->println("okey");
          wc->stop();
        }
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

  if ((millis() - lastRefreshMillis) > refreshMillisInterval)
  {
    refreshSlaves();

    lastRefreshMillis = millis();
  }

  WiFiClient newClient = server.available();
  if (newClient && (newClient != client) && (!client || !client.connected()))
  {
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
      int i = clientData.indexOf("GET "), j = clientData.indexOf(" HTTP/");
      bool open = false;
      if (i >= 0 && j >= 0)
      {
        String request = clientData.substring(i + 4, j);
        request.trim();
        open = requested(request);
      }
      if (!open)
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
    Serial.print("of addr ");
    Serial.println(addr);

    sr.sendRequest(addr, propertySetAnswer, data, argsLen - 1);
  }
  else if (args[0] == "wifi")
  {
    Serial.print("----> Connected to ");
    Serial.println(WiFi.SSID());
    Serial.print("----> IP addr: ");
    Serial.println(WiFi.localIP());
  }
  else if (argsLen == 2 && args[0] == "ping")
  {
    pingSlave(args[1].toInt());
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

      for(unsigned char i = 0; i < 32; i++)
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

bool requested(String path)
{
  Serial.println("PATH: " + path);

  String sub[20];
  unsigned char subCount = 0;
  for(int i = 1; i < path.length() && subCount < 20; i++)
  {
    char c = path[i];

    if (c == '/')
    {
        subCount++;
        continue;
    }

    sub[subCount] += c;
  }
  subCount++;

  // HEADER
  client.println("HTTP/1.1 200 OK");
  client.println("Connection: Keep-Alive");
  client.println("Keep-Alive: timeout=15, max=1000");
  client.println("Content-type: text/html");
  client.println();

  if (sub[0] == "interface")
  {
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
    return false;
  }
  else if (sub[0] == "test")
  {
    client.println("okey");
    return false;
  }
  else if (sub[0] == "deviceList")
  {
    for(unsigned char i = 0; i < 32; i++)
    {
      if (devices[i])
      {
        client.print(devices[i]->name);
        client.print(',');
        client.print(devices[i]->address);
        client.print(',');
        for(unsigned char j = 0; j < 7; j++)
        {
          if (j != 0)
            client.print(' ');
          client.print(devices[i]->uniqueFactoryId[j]);
        }
        client.print(',');
        for(unsigned char j = 0; j < 4; j++)
        {
          if (j != 0)
            client.print(' ');
          client.print(devices[i]->deviceType[j]);
        }
        client.print(',');
        client.print(devices[i]->online ? "true" : "false");
        client.print(',');
        client.print(devices[i]->working ? "true" : "false");
        client.print(';');
      }
    }
    return false;
  }
  else if (sub[0] == "setDeviceName" && subCount == 3)
  {
    unsigned char addr = sub[1].toInt();

    Device* d = getDeviceWithAddress(addr);
    if (d && sub[2].length() > 1 && sub[2].length() < 25)
    {
      sub[2].toCharArray(d->name, sub[2].length() + 1);
      saveDevicesToRom();
      client.println("okey");
    }
    else
    {
      client.println("not okey");
    }
    return false;
  }
  else if (sub[0] == "ping" && subCount == 2)
  {
    unsigned char addr = sub[1].toInt();
    pingSlave(addr, &client);
    return true;
  }
  else if (sub[0] == "bind" && subCount > 1 && subCount <= 8)
  {
    unsigned char ufid[7];
    memset(ufid, 0x0, sizeof(ufid));
    for (unsigned char i = 1; i < subCount; i++)
      ufid[i - 1] = sub[i].toInt();
    bindSlave(ufid, &client);
    return true;
  }
  else if (sub[0] == "unbind" && subCount == 2)
  {
    unsigned char addr = sub[1].toInt();
    unbindSlave(addr, &client);
    return true;
  }
  else if (sub[0] == "prop" && subCount > 3 && subCount < 20)
  {
    unsigned char addr = sub[1].toInt();
    unsigned char startPos = sub[2].toInt();
    unsigned char data[16] = {0x20, startPos};
    for (unsigned char i = 0; i < subCount - 3; i++)
      data[i + 2] = sub[i + 3].toInt();
    sr.sendRequest(addr, propertySetAnswer, data, subCount - 1, &client);
    return true;
  }
  else
  {
    client.println("nope");
    return false;
  }

  return false;
}

void setSlaveProperties(unsigned char addr, unsigned char startPos, unsigned char* values, unsigned char valueCount, void* state)
{
  if (valueCount == 0)
    return;

  unsigned char data[16] = {0x20, startPos};
  for (unsigned char i = 0; i < valueCount && i < 14; i++)
    data[i + 2] = values[i];
  sr.sendRequest(addr, propertySetAnswer, data, valueCount + 2, state);
}

void propertySetAnswer(ResponseStatus status, Request* requested)
{
  if (status == Okay)
  {
    Serial.print("\t-> Propery for slave ");
    Serial.print(requested->fromAddress);
    Serial.println(" was set successfully!");

    Device* setDevice = getDeviceWithAddress(requested->fromAddress);
    if (setDevice)
    {
      unsigned char startPos = requested->sentData[1];
      unsigned char valueCount = requested->sentDataLength - 2;
      for(unsigned char i = 0; i < valueCount; i++)
        setDevice->knownProperties[startPos + i] = requested->sentData[i + 2];
    }
  }

  if (requested->state)
  {
    WiFiClient* wc = (WiFiClient*)requested->state;
    wc->println(status);
    wc->stop();
  }
}

void pingSlave(unsigned char addr, void* state)
{
  unsigned char data[1] = {0x1};

  sr.sendRequest(addr, pingAnswer, data, sizeof(data), state);
}

void pingAnswer(ResponseStatus status, Request* requested)
{
  Serial.print("\t-> Slave ");
  Serial.print(requested->fromAddress);
  Serial.print(" was pinged: ");
  Serial.println(status == Okay ? "Okay" : (status == Failed ? "Failed" : "No response"));

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

  if (requested->state)
  {
    WiFiClient* wc = (WiFiClient*)requested->state;
    wc->println(status);
    wc->stop();
  }
}

void refreshSlave(unsigned char addr, void* state)
{
  unsigned char data[1] = {0x15};

  sr.sendRequest(addr, refreshAnswer, data, sizeof(data), state);
}

void refreshAnswer(ResponseStatus status, Request* requested)
{
  Device* dev = getDeviceWithAddress(requested->fromAddress);

  if (dev)
  {
    bool online = status != NoResponse;

    if (online)
    {
      Serial.print("Received ");
      Serial.print(requested->responseLength);
      Serial.println(" bytes for live data.");

      for(unsigned char i = 0; i < requested->responseLength; i++)
        dev->liveDeviceInfo[i] = requested->response[i];
    }

    if (dev->online != online)
    {
      dev->online = online;
      saveDevicesToRom();
    }
  }
}

bool bindSlave(unsigned char ufid[7], void* state)
{
  return bindSlave(ufid, getNewAddress(), state);
}

bool bindSlave(unsigned char ufid[7], unsigned char withAddress, void* state)
{
  for(unsigned char i = 0; i < 32; i++)
  {
    if (devices[i] && (devices[i]->address == withAddress || memcmp(ufid, devices[i]->uniqueFactoryId, 7) == 0))
    {
      Serial.println("----> Warning: tried to bind 2 slaves with either the same addr or ufid.");

      return false;
    }
  }

  /*unsigned char data[9];
  memcpy(&data[1], &ufid[0], 7);
  data[0] = 0x10;
  data[8] = withAddress;
  sr.broadcast(data, sizeof(data), DataRequest, 130);*/ // Multi-purpose-byte is 130, slave will return 130.
  rebindSlave(ufid, withAddress);
  slaveBoundClient = state;

  registerNewDevice(ufid, withAddress);
  saveDevicesToRom();
  return true;
}

void rebindSlave(unsigned char ufid[7], unsigned char withAddress)
{
  unsigned char data[9];
  memcpy(&data[1], &ufid[0], 7);
  data[0] = 0x10;
  data[8] = withAddress;
  sr.broadcast(data, sizeof(data), DataRequest, 130);
}

void unbindSlave(unsigned char withAddress, void * state)
{
  unsigned char data[1] = { 0x2 };
  sr.sendRequest(withAddress, unbindAnswer, data, sizeof(data), state);

  for(unsigned char i = 0; i < 32; i++)
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

void refreshSlaves()
{
  static unsigned char i = 0;

  if (i >= 32)
    i = 0;

  for(; i < 32; i++)
  {
    if (devices[i] && devices[i]->working)
    {
      refreshSlave(devices[i]->address);

      i++;
      break;
    }
  }
}

void pingSlaves()
{
  static unsigned char i = 0;

  if (i >= 32)
    i = 0;

  for(; i < 32; i++)
  {
    if (devices[i] && devices[i]->working)
    {
      pingSlave(devices[i]->address);

      i++;
      break;
    }
  }
}

void retryNotWorkingBinds()
{
  static unsigned char i = 0;

  if (i >= 32)
    i = 0;

  for(; i < 32; i++)
  {
    if (devices[i] && !(devices[i]->working))
    {
      Serial.print("----> Trying to let device ");
      devices[i]->printToSerial();
      Serial.println(" work...");

      rebindSlave(devices[i]->uniqueFactoryId, devices[i]->address);
      /*unsigned char data[9];
      memcpy(&data[1], devices[i]->uniqueFactoryId, 7);
      data[0] = 0x10;
      data[8] = devices[i]->address;
      sr.broadcast(data, sizeof(data), DataRequest, 130);*/

      i++;
      break;
    }
  }
}

unsigned char getNewAddress()
{
  unsigned char s = EEPROM.read(0);
  if (s == 0xFF)
    s = 1;
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

  if (requested->state)
  {
    WiFiClient* wc = (WiFiClient*)requested->state;
    wc->println(status);
    wc->stop();
  }
}


void printDevices()
{
  Serial.println("----> List of devices that are controlled by this master:");
  unsigned char deviceCount = 0;
  for(unsigned char i = 0; i < 32; i++)
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

  for (int i = 0; i < 32; i++)
  {
    if (EEPROM.read(i * 120 + 100 + 120 - 1) == 0xFF)
    {
      // Device save location is empty
      devices[i] = nullptr;
    }
    else
    {
      // Device save location is used, read it
      unsigned char bytes[120];
      for(int j = 0; j < 120; j++)
          bytes[j] = EEPROM.read(i * 120 + 100 + j);
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
  for (int i = 100; i < 100 + 32 * 120; i++)
    EEPROM.write(i, 0xFF);
  for(unsigned char i = 0; i < 32; i++)
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

  for (int i = 0; i < 32; i++)
  {
    if (devices[i])
    {
      /*Serial.print("Saving device ");
      Serial.print(i);
      Serial.print(": ");
      devices[i]->printToSerial();
      Serial.println();*/
      unsigned char* bytes = devices[i]->getBytes();
      for(int j = 0; j < 120; j++)
          EEPROM.write(i * 120 + 100 + j, bytes[j]);
      deviceCount++;
    }
    else
    {
      EEPROM.write(i * 120 + 100 + 120 - 1, 0xFF);
    }
  }

  EEPROM.commit();

  Serial.print("\t-> ");
  Serial.print(deviceCount);
  Serial.println(" devices were saved to ROM.");
}

Device* registerNewDevice(unsigned char ufid[7], unsigned char addr)
{
  for(unsigned char i = 0; i < 32; i++)
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
  for(unsigned char i = 0; i < 32; i++)
  {
    if (devices[i] && devices[i]->address == addr)
      return devices[i];
  }

  return nullptr;
}
