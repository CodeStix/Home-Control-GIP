#include <stdio.h>
#include <iostream>
#include "Packet.h"

using namespace std;

int main()
{
    Packet p = Packet();

    printf("Hello world\n");
    printf("Creation time of packet: %d", p.index);

}