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

void Logger::log(int i)
{
    if (enable)
        Serial.print(i, DEC);
}

void Logger::log(unsigned char i)
{
    if (enable)
        Serial.print(i, DEC);
}

void Logger::log(long i)
{
    if (enable)
        Serial.print(i, DEC);
}

void Logger::logln(char* s)
{
    if (enable)
        Serial.println(s);
}

void Logger::logln(unsigned char i)
{
    if (enable)
        Serial.println(i, DEC);
}

void Logger::logln(int i)
{
    if (enable)
        Serial.println(i, DEC);
}

void Logger::logln(long i)
{
    if (enable)
        Serial.println(i, DEC);
}

void Logger::logln(char s)
{
    if (enable)
        Serial.println(s, DEC);
}

void Logger::logln()
{
  if (enable)
      Serial.println();
}
