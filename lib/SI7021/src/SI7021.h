#ifndef SI7021_h
#define SI7021_h

#include "Wire.h"

//    I2C ADDRESS/BITS
#define SI7021_DEFAULT_ADDRESS         (0x40)

#define SI7021_MEASRH_HOLD_CMD           0xE5
#define SI7021_MEASRH_NOHOLD_CMD         0xF5
#define SI7021_MEASTEMP_HOLD_CMD         0xE3
#define SI7021_MEASTEMP_NOHOLD_CMD       0xF3
#define SI7021_READPREVTEMP_CMD          0xE0
#define SI7021_RESET_CMD                 0xFE
#define SI7021_WRITERHT_REG_CMD          0xE6
#define SI7021_READRHT_REG_CMD           0xE7
#define SI7021_WRITEHEATER_REG_CMD       0x51
#define SI7021_READHEATER_REG_CMD        0x11
#define SI7021_ID1_CMD                   0xFA0F
#define SI7021_ID2_CMD                   0xFCC9
#define SI7021_FIRMVERS_CMD              0x84B8

class SI7021 {
 public:
  SI7021(TwoWire& wire, int controlPin)
    : _wire(wire), _controlPin(controlPin)
  {
      _i2cAddress = SI7021_DEFAULT_ADDRESS;
      _sernum_a = _sernum_b = 0;

      pinMode(_controlPin, OUTPUT);
  }

  void turnOn(void);
	void turnOff(void);

  bool verify();
  void reset(void);

  float readTemperature(void);
  float readTemperatureF(void);
  float readHumidity(void);

 private:
   int _state = -1;
   int _controlPin = -1;
   int16_t _timeout = 100;
   int8_t  _i2cAddress;
   uint32_t _sernum_a, _sernum_b;
   TwoWire& _wire;

   void readSerialNumber(void);

   uint8_t read8(uint8_t address);
   uint16_t read16(uint8_t address);
   uint8_t write8(uint8_t address, uint8_t value);
};

#endif
