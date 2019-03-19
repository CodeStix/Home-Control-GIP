#include "Device.h"

Device::Device(byte address)
{
    this->address = address;
}

byte Device::getAddress()
{
    return this->address;
}