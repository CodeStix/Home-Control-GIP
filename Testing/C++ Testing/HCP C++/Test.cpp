#include <stdio.h>
#include "Packet.cpp"

void send(unsigned char data[8])
{
    printf("---Sending data packet of size 8:----\n");
    for(int i = 0; i < 8; i++)
    {
        printf("Sending data with index %d: %d\n", i, data[i]);
    }
}

int main()
{
    //int from;
    //scanf("%d", &from);

    Packet test1(255, 160, 0, 0, 0, 1);

    send(test1.get_bytes());
    printf("Packet to string: %s\n", test1.to_string());

    Packet test2(test1.get_bytes());

    send(test2.get_bytes());
    printf("Packet to string: %s\n", test2.to_string());

    Packet test3(160, 255, 0, 0, 0, 1);
    printf("New packet: %s\n", test3.to_string());

    return 0;
}

