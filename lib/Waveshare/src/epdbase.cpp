#include "epdbase.h"
#include <SPI.h>

int EpdBase::Initialize(void)
{
    pinMode(_cs, OUTPUT);
    pinMode(_reset, OUTPUT);
    pinMode(_dc, OUTPUT);
    pinMode(_busy, INPUT);

    _spi.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
    _spi.begin();

    return 0;
}

/**
 *  @brief: basic function for sending commands
 */
void EpdBase::SendCommand(unsigned char command)
{
    DigitalWrite(_dc, LOW);
    SpiTransfer(command);
}

/**
 *  @brief: basic function for sending data
 */
void EpdBase::SendData(unsigned char data)
{
    DigitalWrite(_dc, HIGH);
    SpiTransfer(data);
}

/**
 *  @brief: Wait until the busy_pin goes HIGH
 */
void EpdBase::WaitUntilIdle(void)
{
    while(DigitalRead(_busy) == 0)
    {      //0: busy, 1: idle
        DelayMs(100);
    }
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Epd::Sleep();
 */
void EpdBase::Reset(void)
{
    DigitalWrite(_reset, LOW);
    DelayMs(200);
    DigitalWrite(_reset, HIGH);
    DelayMs(200);
}

void EpdBase::DigitalWrite(int pin, int value)
{
    digitalWrite(pin, value);
}

int EpdBase::DigitalRead(int pin)
{
    return digitalRead(pin);
}

void EpdBase::DelayMs(unsigned int delaytime)
{
    delay(delaytime);
}

void EpdBase::SpiTransfer(unsigned char data)
{
    digitalWrite(_cs, LOW);
    _spi.transfer(data);
    digitalWrite(_cs, HIGH);
}
