#include <stdio.h>
#include "Packet.cpp"

#define MAX_CONCURRENT_REQUESTS 16
#define MY_ADDRESS 16

unsigned char* sendReceive(unsigned char data[8])
{
    printf("Packet --> send --> respond\n");
    data[4] = 0;
    data[5] = 0;
    data[6] = 0;
    data[7] = 0;
    return data;
    /*for(int i = 0; i < 8; i++)
    {
        printf("Sending data with index %d: %d\n", i, data[i]);
    }*/
}

Packet pending[MAX_CONCURRENT_REQUESTS];

bool send(unsigned char too, unsigned char data1, unsigned char data2, unsigned char data3, unsigned char data4)
{
    Packet a;

    // Find an available packet.
    for(int i = 0; i < MAX_CONCURRENT_REQUESTS; i++)
    {
        if (!pending[i].isUsed)
        {
            pending[i] = Packet(MY_ADDRESS, too, data1, data2, data3, data4);
            a = pending[i];
            break;
        }
        else if (i == MAX_CONCURRENT_REQUESTS - 1)
            return false;
    }

    printf("Found unused packet: %s", a.to_string());

    return true;
}


int main()
{
    //int from;
    //scanf("%d", &from);


    //for(int i = 0; i < 8; i++)
    //{
    //    printf("%s", pending[i].isUsed ? "true" : "false");
    //}


    //Packet test1(255, 160, 0, 0, 0, 1);

    //unsigned char * data = sendReceive(test1.to_bytes());



    return 0;
}
