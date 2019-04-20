# 1 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino"
# 1 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino"
# 2 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino" 2
# 3 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino" 2
# 4 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino" 2
# 5 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino" 2
# 6 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino" 2
# 7 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino" 2


// Note: HC12 TX to RX and RX to TX


// This masters address, can be 1, 2 or 3.



const char* ssid = "PollenPatatten";
const char* password = "Ziektes123";

WiFiServer server(80);
SoftwareSerial ss = SoftwareSerial(12, 14);
PacketSenderReceiver sr = PacketSenderReceiver(&ss, false, 2);
Packet temp;
Device* devices[64];

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
  Serial.print("My address (master): ");
  Serial.println(2);
  Serial.println("Loading devices...");
  EEPROM.begin(4096);
  //clearRomDevices();
  loadDevicesFromRom();
  Serial.println("Starting...");

  delay(500);

  ss.begin(2400);
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
    if (c == ' ')
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

    if (temp.needsResponse())
    {
      Serial.println("Packet needs response.");
    }

    // Slave is bound.
    if (temp.getMultiPurposeByte() == 130)
    {
      Device* bound = getDeviceWithAddress(temp.getSlave());

      if (bound != nullptr)
      {
          bound->working = true;
          memcpy(bound->deviceInfo, temp.getData(), temp.getDataLength());

          Serial.print("Slave is now bound: ");
          bound->printToSerial();
          Serial.println();
      }
      else
      {
        Serial.println("FATAL: Count not let slave work!");
      }
    }
  }

  sr.resendUnansweredRequests();
}

void command(String args[16], unsigned char argsLen)
{
  if (argsLen >= 4 && args[0] == "prop")
  {
    Serial.print("Trying to set property ");
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

    sr.sendRequest(addr, answer, data, argsLen - 1);
  }
  else if (argsLen >= 2 && argsLen <= 8 && args[0] == "bind")
  {
    unsigned char ufid[7];
    memset(ufid, 0x0, sizeof(ufid));
    Serial.print("Binding slave with ufid ");
    for (unsigned char i = 1; i < argsLen; i++)
    {
      ufid[i - 1] = args[i].toInt();

      Serial.print(ufid[i - 1]);
      Serial.print(' ');
    }
    Serial.println();

    bindSlave(ufid);
  }
  else if (argsLen == 2 && args[0] == "ping")
  {
    unsigned char addr = args[1].toInt();
    unsigned char data[1] = {0x1};

    sr.sendRequest(addr, pingAnswer, data, sizeof(data));
  }
  else if (argsLen == 2 && args[0] == "unbind")
  {
    unsigned char addr = args[1].toInt();

    Serial.print("Unbinding slave ");
    Serial.print(addr);
    Serial.println("...");

    unbindSlave(addr);
  }
  else if (args[0] == "save")
  {
    Serial.println("Saving...");

    unsigned char ufid[7] = {0, 1, 2, 3, 4, 5, 6};
    devices[0] = new Device(ufid, 9);

    saveDevicesToRom();

    Serial.println("Saved?");
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

void bindSlave(unsigned char ufid[7])
{
  bindSlave(ufid, getNewAddress());
}

void bindSlave(unsigned char ufid[7], unsigned char withAddress)
{
  unsigned char data[9];
  memcpy(&data[1], &ufid[0], 8);
  data[0] = 0x10;
  data[8] = withAddress;
  sr.broadcast(data, sizeof(data), DataRequest, 130); // Multi-purpose-byte is 130, slave will return 130.

  registerNewDevice(ufid, withAddress);
  saveDevicesToRom();
}

void unbindSlave(unsigned char withAddress)
{
  unsigned char data[1] = { 0x2 };
  sr.sendRequest(withAddress, unbindAnswer, data, sizeof(data));
}

unsigned char getNewAddress()
{
  unsigned char s = EEPROM.read(0);
  EEPROM.write(0, ++s);
  return s;
}

void pingAnswer(ResponseStatus status, Request* requested)
{
  Serial.print("Slave was pinged: ");
  Serial.println(status == Okay ? "Okay" : (status == Failed ? "Failed" : "No response"));
}

void unbindAnswer(ResponseStatus status, Request* requested)
{
  if (status == Okay)
    Serial.println("Slave was successfully unbound.");
}

void answer(ResponseStatus status, Request* requested)
{
  if (requested->responseLength == 8 && requested->response[0] == 0x11)
  {
    Serial.println("Received new slave information!");
  }

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

void loadDevicesFromRom()
{
  /*Serial.print("Size of device: ");

  Serial.println(sizeof(Device));*/
# 289 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino"
  unsigned char deviceCount = 0;

  for (int i = 0; i < 64; i++)
  {
    if (EEPROM.read(i * sizeof(Device) + 100 + sizeof(Device) - 1) == 0xFF)
    {
      // Device save location is empty
      devices[i] = nullptr;
    }
    else
    {
      // Device save location is used, read it
      unsigned char bytes[50];
      for(int j = 0; j < sizeof(Device); j++)
          bytes[j] = EEPROM.read(i * sizeof(Device) + 100 + j);
      devices[i] = new Device(bytes);
      /*Serial.print("Red device: ");

      devices[i]->printToSerial();

      Serial.println();*/
# 308 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino"
      deviceCount++;
    }
  }

  Serial.print(deviceCount);
  Serial.println(" devices were loaded from ROM.");
}

void clearRomDevices()
{
  for (int i = 100; i < 100 + 64 * 50; i++)
  {
    EEPROM.write(i, 0xFF);
  }

  EEPROM.commit();

  Serial.println("All devices were ereased from ROM.");
}

void saveDevicesToRom()
{
  unsigned char deviceCount = 0;

  for (int i = 0; i < 64; i++)
  {
    if (devices[i] != nullptr)
    {
      /*Serial.print("Saving device ");

      Serial.print(i);

      Serial.print(": ");

      devices[i]->printToSerial();

      Serial.println();*/
# 341 "c:\\Users\\Stijn Rogiest\\Documents\\GitHub\\Home-Control-GIP\\Home Control Protocol\\HCP_MCU_v4\\HCP_MCU_v4.ino"
      unsigned char* bytes = devices[i]->getBytes();
      for(int j = 0; j < sizeof(Device); j++)
          EEPROM.write(i * sizeof(Device) + 100 + j, bytes[j]);
      deviceCount++;
    }
    else
    {
      EEPROM.write(i * sizeof(Device) + 100 + sizeof(Device) - 1, 0xFF);
    }
  }

  EEPROM.commit();

  Serial.print(deviceCount);
  Serial.println(" devices were saved to ROM.");
}

Device* registerNewDevice(unsigned char ufid[7], unsigned char addr)
{
  for(unsigned char i = 0; i < 64; i++)
  {
    if (devices[i] == nullptr)
    {
      devices[i] = new Device(ufid, addr);

      return devices[i];
    }

  }

  return nullptr;
}

Device* getDeviceWithAddress(unsigned char addr)
{
  for(unsigned char i = 0; i < 64; i++)
  {
    if (devices[i]->address == addr)
      return devices[i];
  }

  return nullptr;
}
