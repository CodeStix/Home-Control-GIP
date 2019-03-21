class Packet 
{
    public:
        Packet();
        Packet(unsigned char data[8]);
        Packet(unsigned char fromAddress, unsigned char tooAddress, unsigned char data1, unsigned char data2, unsigned char data3, unsigned char data4);
        unsigned char index;
        unsigned char from;
        unsigned char too;
        unsigned char data1;
        unsigned char data2;
        unsigned char data3;
        unsigned char data4;
        bool isValid;

        bool isUsed;

        static unsigned char identifierByte;
        static unsigned char currentIndex;

        char* to_string();
        unsigned char* get_bytes();
};