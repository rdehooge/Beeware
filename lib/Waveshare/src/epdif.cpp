#include "epdif.h"

EpdIf::~EpdIf() {

};

void EpdIf::DigitalWrite(int pin, int value) {
    digitalWriteFast(pin, value);
}

int EpdIf::DigitalRead(int pin) {
    return pinReadFast(pin);
}

void EpdIf::DelayMs(unsigned int delaytime) {
    delay(delaytime);
}

void EpdIf::SpiTransfer(unsigned char data) {
    digitalWrite(_pin_disp_cs, LOW);
    _spiPort.transfer(data);
    digitalWrite(_pin_disp_cs, HIGH);
}

int EpdIf::IfInit(void) {
    pinMode(_pin_disp_cs, OUTPUT);
    pinMode(_pin_disp_rst, OUTPUT);
    pinMode(_pin_disp_dc, OUTPUT);
    pinMode(_pin_disp_bsy, INPUT);
    // End existing transaction. Important when reinitializing.
    // On scenario is when a reinitialization needs to be done
    // to switch from full updates to partial updates.
    if (!_1stInitialized) {
      _1stInitialized = true;
    }
    else {
      _spiPort.end();
      _spiPort.endTransaction();
    }
    _spiPort.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
    _spiPort.begin();
    return 0;
}
