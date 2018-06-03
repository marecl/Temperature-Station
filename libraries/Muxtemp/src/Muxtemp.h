class Muxtemp {
  public:
    Muxtemp(TwoWire&);
    ~Muxtemp();
    uint8_t begin(uint8_t);
    void refreshPorts();
    void getPorts();
    float getTemp(uint8_t);
    uint8_t getCount();
    uint8_t typeOf(uint8_t);
    uint8_t* getAddress(uint8_t);
    bool bypass1Wire();

  private:
    TwoWire *_Wire;
    uint8_t _addr;
    uint8_t _count;
    uint8_t _readCount();
    uint8_t *_sensors;
};