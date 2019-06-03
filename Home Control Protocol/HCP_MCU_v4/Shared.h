#ifndef Shared_h
#define Shared_h

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

unsigned char temperatureToByte(float floatTemp)
{
  float f = (floatTemp - 10.0f) * 10.0f;
  if (f > 255)
    f = 255;
  else if (f < 0)
    f = 0

  return (unsigned char)f;
}

float byteToTemperature(unsigned char byteTemp)
{
  return (byteTemp / 10.0f) + 10.0f;
}

#endif