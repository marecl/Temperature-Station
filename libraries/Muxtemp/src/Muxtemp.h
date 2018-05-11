class Muxtemp {
  public:
    Muxtemp(TwoWire&);
    ~Muxtemp();
    bool begin(uint8_t);
    void probePorts();
    void sendPorts();
    void readPorts();
    float getTemp(uint8_t);
    uint8_t getCount();
    uint8_t typeOf(uint8_t);

  private:
    TwoWire *_Wire;
    uint8_t _addr;
    uint8_t _count;
    uint8_t _readCount();
    uint8_t *_sensors;
};

