#if (ARDUINO < 100)
#include <WProgram.h>
#else
#include <Arduino.h>
#endif
#include <cstdlib>
#include <Wire.h>
//N is a second byte sent
#define MUXTEMP_REFRESH 0x75  //refresh sensors
#define MUXTEMP_SEND   0x78   //send connected sensors
#define MUXTEMP_ADDR  0x81    //sensor address from port N (only when 1wire bypass is not used!)
#define MUXTEMP_READ  0x84    //send measurement from port N
#define MUXTEMP_COUNT 0x87    //get port count
#define MUXTEMP_BYPASS  0x90  //is 1wire bypassed

class Muxtemp {
  public:
    Muxtemp(TwoWire&, uint8_t);
    ~Muxtemp();
    uint8_t begin();
    void refresh();
    void getPorts();
    float getTemp(uint8_t);
    uint8_t getCount();
    uint8_t typeOf(uint8_t);
    uint8_t* getAddress(uint8_t);
    bool bypass1Wire();
    uint32_t lock();
    void setDelay(uint32_t);

  private:
    TwoWire *_Wire;
    int8_t _addr;
    int8_t _count;
    uint8_t _readCount();
    uint8_t *_sensors;
    uint32_t _available;
    uint32_t _delay;
};