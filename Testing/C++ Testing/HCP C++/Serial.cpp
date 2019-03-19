#include "Serial.h"
#include <stdio.h>
#include <iostream>

using namespace std;

void Serial::available()
{
    return 0;
}

int Serial::peek()
{
    return 0;
}

int Serial::read()
{
    printf("");

    int c;
    scanf("%d", &c);
    return c;
}

void Serial::write(int d)
{
    printf("Data was being written using Serial: %x", d);
}