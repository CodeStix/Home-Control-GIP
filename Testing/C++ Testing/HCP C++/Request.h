class Request
{
    public:
        Request();
        //Request(unsigned char from, unsigned char data1, unsigned char data2, unsigned char data3, unsigned char data4);
        bool gotResponded;
        bool isUsed;
        unsigned char from;
        unsigned char data1;
        unsigned char data2;
        unsigned char data3;
        unsigned char data4;
        unsigned char forIndex;
        bool is_packet_response(Packet packet);
        Packet get_request_packet();
        char* to_string();
        void use_for(unsigned char from, unsigned char data1, unsigned char data2, unsigned char data3, unsigned char data4);

        static unsigned char currentIndex;

};