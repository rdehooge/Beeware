#ifndef EPDIF_H
#define EPDIF_H

#include <Arduino.h>
#include <SPI.h>

class EpdIf {

public:
    EpdIf(
        SPIClass& spiPort,
        int16_t dispDcPin,
        int16_t dispCsPin,
        int16_t dispResetPin,
        int16_t dispBusyPin
    ) : _spiPort(spiPort),
        _pin_disp_dc(dispDcPin),
        _pin_disp_cs(dispCsPin),
        _pin_disp_rst(dispResetPin),
        _pin_disp_bsy(dispBusyPin)
    {}

    ~EpdIf(void);

    int  IfInit(void);
    void DigitalWrite(int pin, int value);
    int  DigitalRead(int pin);
    void DelayMs(unsigned int delaytime);
    void SpiTransfer(unsigned char data);

protected:
    SPIClass& _spiPort;
    int16_t _pin_disp_cs;
    int16_t _pin_disp_dc;
    int16_t _pin_disp_rst;
    int16_t _pin_disp_bsy;
    bool _1stInitialized;
};

#endif
