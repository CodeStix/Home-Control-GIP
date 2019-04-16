#include "Logger.h"
#include "Arduino.h"

Logger::Logger()
{
    this->enable = true;
}

void Logger::log(char* s)
{
    if (enable)
        Serial.print(s);
}

void Logger::log(char s)
{
    if (enable)
        Serial.print(s);
}

void Logger::logln(char* s)
{
    if (enable)
        Serial.println(s);
}

void Logger::logln()
{
  if (enable)
      Serial.println();
}
