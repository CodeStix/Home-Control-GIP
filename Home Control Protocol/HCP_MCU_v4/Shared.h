#ifndef Shared_h
#define Shared_h

#define HCP_VERSION "0.4.1"
/*
Version 0.4.0:
  - Core components, everything, initial commit.
Version 0.4.1.*:
  - Flush sending buffer after write.
  - Fixed led not working properly on ESP8266.
  - Fixed baud 2800 -> 2400.
  - Low memory mode: LOW_MEM
  - Fixed rebind not working.
  - ...
*/

#ifdef LOW_MEM
#define MAX_CONCURRENT_RE QUESTS 2
#endif

void veryCoolSplashScreen()
{
#ifndef LOW_MEM
  Serial.println();
  Serial.println("    _   _      ____    ____    ");
  Serial.println("   |'| |'|  U /\"___| U|  _\"\\ u ");
  Serial.println("  /| |_| |\\ \\| | u   \\| |_) |/ ");
  Serial.println("  U|  _  |u  | |/__   |  __/   ");
  Serial.println("   |_| |_|    \\____|  |_|      ");
  Serial.println("   //   \\\\   _// \\ \\  ||>>_    ");
  Serial.println("  (_\") (\"_) (__)(__) (__)__)");
#endif
  Serial.print("Home Control Protocol - v");
  Serial.println(HCP_VERSION);
  Serial.println("\tby Stijn Rogiest (c) 2019");
  Serial.println();
}

unsigned char temperatureToByte(float floatTemp)
{
  float f = (floatTemp - 10.0f) * 10.0f;
  if (f > 255)
    f = 255;
  else if (f < 0)
    f = 0;

  return (unsigned char)f;
}

float byteToTemperature(unsigned char byteTemp)
{
  return (byteTemp / 10.0f) + 10.0f;
}

unsigned char currentFlowToByte(float floatCurrent)
{
  return 0;
}

#endif