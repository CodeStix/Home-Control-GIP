class Packet
{
    public:
        Packet(byte fromAddress, byte toAddress, byte data1, byte data2);
        Packet(byte data[8]);
        byte data1;
        byte data2;
        byte data3;
        byte data4;
        byte toAddress;
        byte fromAddress;
        byte index;
        static byte identifierByte = 0xf;
        /// Converts this packet to a sequence of bytes.
        byte * getBytes();
        int getCreationMillis();
        bool getIsValid();
        bool responded;

    private:
        /// The creation time of this packet in ms.
        int creationMillis;
        /// Was this packet constructed successfully?
        bool isValid;
        static long currentIndex = 0;
}