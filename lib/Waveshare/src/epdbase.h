#ifndef EPDBASE_H
#define EPDBASE_H

#include <SPI.h>

class EpdBase {
public:
    EpdBase(SPIClass& spi, int16_t dc, int16_t cs, int16_t reset, int16_t busy)
      : _spi(spi), _dc(dc), _cs(cs), _reset(reset), _busy(busy)
    {}

    ~EpdBase()
    {}

    int Initialize(void);

    void SendCommand(unsigned char command);
    void SendData(unsigned char data);
    void WaitUntilIdle(void);
    void Reset(void);

    void DigitalWrite(int pin, int value);
    int  DigitalRead(int pin);
    void DelayMs(unsigned int delaytime);
    void SpiTransfer(unsigned char data);

  private:
    SPIClass& _spi;
    unsigned int _dc;
    unsigned int _cs;
    unsigned int _reset;
    unsigned int _busy;
};

#endif
