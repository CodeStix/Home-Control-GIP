#include "Device.h"

Device::Device(unsigned char address)
{
    this->address = address;
}

unsigned char Device::get_address()
{
    return this->address;
}