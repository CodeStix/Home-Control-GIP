class Device
{
    public:
        Device(unsigned char address);
        unsigned char get_address();
    
    private:
        unsigned char address;
};