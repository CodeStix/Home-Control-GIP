class Packet
{
    public:
        Packet(byte fromAddress, byte toAddress, byte data);
        byte data;
        byte toAddress;
        byte fromAddress;
        byte index;
        static byte identifierByte = 0xf;

    private:
        int creationMillis;
        bool gotResponded = false;
        static long currentIndex = 0;
}