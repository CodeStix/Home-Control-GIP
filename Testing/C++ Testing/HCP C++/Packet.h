class Packet
{
    public:
        Packet(unsigned char fromAddress, unsigned char toAddress, unsigned char data1, unsigned char data2, unsigned char data3, unsigned char data4);
        Packet(unsigned char data[8]);
        unsigned char data1;
        unsigned char data2;
        unsigned char data3;
        unsigned char data4;
        unsigned char toAddress;
        unsigned char fromAddress;
        unsigned char index;
        static unsigned char identifierByte;
        /// Converts this packet to a sequence of bytes.
        unsigned char * getBytes();
        int getCreationMillis();
        bool getIsValid();
        bool responded;
        char* toString();

    private:
        /// The creation time of this packet in ms.
        int creationMillis;
        /// Was this packet constructed successfully?
        bool isValid;
        static long currentIndex;
};