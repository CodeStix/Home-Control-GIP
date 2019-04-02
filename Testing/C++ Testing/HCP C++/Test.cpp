#define NO_ARDUINO
#define MY_ADDRESS 16

#include <stdio.h>
#include "HCPClient.cpp"
#include <ncurses.h>

#define MAX_CONCURRENT_REQUESTS 16

typedef void (*ResponseHandler)(void);

int main()
{
    HCPClient cl = HCPClient(18);

    while(true)
    {
        cl.run();

        char c = getchar();

        if (c == 'r')
        {
            Request* r;
            cl.request(r, 10, 0, 0, 0, 1);
        }
        else if (c == 'a')
        {
            
        }
        
        cl.run();
    }



    return 0;
}

