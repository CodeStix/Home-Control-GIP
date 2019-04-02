class Packet 
{
    public:
        Packet();
        Packet(unsigned char data[8]);
        Packet(unsigned char fromAddress, unsigned char toAddress, unsigned char index, unsigned char data1, unsigned char data2, unsigned char data3, unsigned char data4);
        unsigned char index;
        unsigned char from;
        unsigned char to;
        unsigned char data1;
        unsigned char data2;
        unsigned char data3;
        unsigned char data4;
        bool isValid;

        static unsigned char identifierByte;

        char* to_string();
        unsigned char* get_bytes();
};