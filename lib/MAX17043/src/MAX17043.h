#ifndef MAX17043_h
#define MAX17043_h

#include "Wire.h"

#define MAX17043_DEFAULT_ADDRESS  0x36

// All registers contain two bytes of data and span two addresses.
#define MAX17043_VCELL    0x02 // R - 12-bit A/D measurement of battery voltage
#define MAX17043_SOC      0x04 // R - 16-bit state of charge (SOC)
#define MAX17043_MODE     0x06 // W - Sends special commands to IC
#define MAX17043_VERSION  0x08 // R - Returns IC version
#define MAX17043_CONFIG   0x0C // R/W - Battery compensation (default 0x971C)
#define MAX17043_COMMAND  0xFE // W - Sends special comands to IC

#define MAX17043_CONFIG_SLEEP     7
#define MAX17043_CONFIG_ALERT     5
#define MAX17043_CONFIG_THRESHOLD 0

#define MAX17043_MODE_QUICKSTART 0x4000
#define MAX17043_COMMAND_POR 0x5400

class MAX17043
{
public:
  MAX17043(TwoWire& wire)
    : _wire(wire)
  {
      _i2cAddress = MAX17043_DEFAULT_ADDRESS;
  }

  void Initialize();

  uint8_t quickStart();
  uint16_t getVersion();
  uint8_t reset();

  float getVoltage();
  float getSOC();

  uint8_t getThreshold();
  uint8_t setThreshold(uint8_t percent);
  uint8_t getAlert(bool clear = false);
  uint8_t clearAlert();
  uint8_t sleep();
  uint8_t wake();
  uint16_t getConfigRegister();
  uint8_t getCompensation();
  uint8_t setCompensation(uint8_t newCompensation);

private:
  int8_t  _i2cAddress;
  int16_t _timeout = 100;
  TwoWire& _wire;

  uint8_t write16(uint8_t address, uint16_t data);
  uint16_t read16(uint8_t address);
};

#endif
