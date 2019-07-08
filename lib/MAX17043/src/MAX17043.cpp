#include "MAX17043.h"

void MAX17043::Initialize()
{
  quickStart();
  setThreshold(20);
}

uint8_t MAX17043::quickStart()
{
  // A quick-start allows the MAX17043 to restart fuel-gauge calculations in the
  // same manner as initial power-up of the IC. If an application's power-up
  // sequence is exceedingly noisy such that excess error is introduced into the
  // IC's first guess of SOC, the host can issue a quick-start to reduce the
  // error. A quick-start is initiated by a rising edge on the QSTRT pin, or
  // through software by writing 4000h to MODE register.
  return write16(MAX17043_MODE_QUICKSTART, MAX17043_MODE);
}

uint16_t MAX17043::getVersion()
{
  return read16(MAX17043_VERSION);
}

uint8_t MAX17043::reset()
{
  return write16(MAX17043_COMMAND_POR, MAX17043_COMMAND);
}

float MAX17043::getVoltage()
{
  uint16_t voltage;

  voltage = read16(MAX17043_VCELL);
  // vCell is a 12-bit register where each bit represents 1.25mV
  voltage = (voltage) >> 4;

  return ((float) voltage / 800.0);
}

float MAX17043::getSOC()
{
  uint16_t soc;
  float percent;

  soc = read16(MAX17043_SOC);

  percent = (soc & 0xFF00) >> 8;
  percent += (float) (((uint8_t) soc) / 256.0);

  return percent;
}

uint8_t MAX17043::getThreshold()
{
  uint16_t configRegister = read16(MAX17043_CONFIG);
  uint8_t threshold = (configRegister & 0x001F);

  // It has an LSb weight of 1%, and can be programmed from 1% to 32%.
  // Values are in 2's complement, e.g.: 00000=32%, 00001=31%, 11111=1%.
  // Let's convert our percent to that first:
  threshold = 32 - threshold;
  return threshold;
}

uint8_t MAX17043::setThreshold(uint8_t percent)
{
   // The alert threshold is a 5-bit value that sets the state of charge level
   // where an interrupt is generated on the ALRT pin.

   // It has an LSb weight of 1%, and can be programmed from 1% to 32%.
   // Values are in 2's complement, e.g.: 00000=32%, 00001=31%, 11111=1%.
   // Let's convert our percent to that first:
   percent = constrain(percent, 0, 32);
   percent = 32 - percent;

   // Read config reg, so we don't modify any other values:
   uint16_t configRegister = read16(MAX17043_CONFIG);
   configRegister &= 0xFFE0; // Mask out threshold bits
   configRegister |= percent; // Add new threshold

   return write16(configRegister, MAX17043_CONFIG);
}

uint8_t MAX17043::clearAlert()
{
   // Read config reg, so we don't modify any other values:
   uint16_t configRegister = read16(MAX17043_CONFIG);
   configRegister &= ~(1<<6); // Clear ALRT bit manually.

   return write16(configRegister, MAX17043_CONFIG);
}

uint8_t MAX17043::getAlert(bool clear)
{
   // Read config reg, so we don't modify any other values:
   uint16_t configRegister = read16(MAX17043_CONFIG);

   if (configRegister & (1<<6))
   {
      if (clear) // If the clear flag is set
      {
        configRegister &= ~(1<<6); // Clear ALRT bit manually.
        write16(configRegister, MAX17043_CONFIG);
      }

      return 1;
   }

    return 0;
}

uint8_t MAX17043::sleep()
{
   // Read config reg, so we don't modify any other values:
   uint16_t configRegister = read16(MAX17043_CONFIG);

   if (configRegister & (1<<7))
      return 5; // Already sleeping, do nothing but return an error

   configRegister |= (1<<7); // Set sleep bit

   return write16(configRegister, MAX17043_CONFIG);
}

uint8_t MAX17043::wake()
{
   // Read config reg, so we don't modify any other values:
  uint16_t configRegister = read16(MAX17043_CONFIG);

  if (!(configRegister & (1<<7)))
    return 5; // Already awake, do nothing but return an error

  configRegister &= ~(1<<7); // Clear sleep bit

  return write16(configRegister, MAX17043_CONFIG);
}

uint16_t MAX17043::getConfigRegister()
{
  return read16(MAX17043_CONFIG);
}

uint8_t MAX17043::getCompensation()
{
   uint16_t configRegister = read16(MAX17043_CONFIG);
   uint8_t compensation = (configRegister & 0xFF00) >> 8;
   return compensation;
}

uint8_t MAX17043::setCompensation(uint8_t newCompensation)
{
   // The CONFIG register compensates the ModelGauge algorith. The upper 8 bits
   // of the 16-bit register control the compensation.
   // Read the original configReg, so we can leave the lower 8 bits alone:
   uint16_t configRegister = read16(MAX17043_CONFIG);

   configRegister &= 0x00FF; // Mask out compensation bits
   configRegister |= ((uint16_t)newCompensation << 8) | configRegister;

   return write16(configRegister, MAX17043_CONFIG);
}

uint8_t MAX17043::write16(uint8_t address, uint16_t data)
{
   uint8_t msb, lsb;
   msb = (data & 0xFF00) >> 8;
   lsb = (data & 0x00FF);

   _wire.beginTransmission(_i2cAddress);
   _wire.write(address);
   _wire.write(msb);
   _wire.write(lsb);

   return (_wire.endTransmission());
}

uint16_t MAX17043::read16(uint8_t address)
{
  uint8_t msb, lsb;
  int16_t timeout = 1000;

  _wire.beginTransmission(_i2cAddress);
  _wire.write(address);
  _wire.endTransmission(false);

  _wire.requestFrom(_i2cAddress, 2);

  int16_t counter = 0;

  while ((_wire.available() < 2) && (counter++ < _timeout))
    delay(1);

  if ((_wire.available() < 2) && (counter >= _timeout))
    return -2;

  msb = _wire.read();
  lsb = _wire.read();

  return ((uint16_t) msb << 8) | lsb;
}
