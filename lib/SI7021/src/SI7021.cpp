#include "SI7021.h"   // Use for Build IDE
/**************************************************************************/

void SI7021::turnOn(void)
{
	// turn the control pin high to supply current
	digitalWrite(_controlPin, HIGH);
	_state = 1;
}

void SI7021::turnOff(void)
{
	digitalWrite(_controlPin, LOW);
	_state = 0;
}

bool SI7021::verify(void)
{
  if (_state <= 0)
    return false;

	reset();

  if (read8(SI7021_READRHT_REG_CMD) != 0x3A)
    return false;

	readSerialNumber();

  return true;
}

void SI7021::reset(void)
{
  _wire.beginTransmission(_i2cAddress);
  _wire.write((uint8_t)SI7021_RESET_CMD);
  _wire.endTransmission();
  delay(50);
}

float SI7021::readHumidity(void)
{
  if (_state <= 0)
    return -1;

  _wire.beginTransmission(_i2cAddress);
  _wire.write((uint8_t)SI7021_MEASRH_NOHOLD_CMD);
  _wire.endTransmission(false);
  delay(25);

  _wire.requestFrom(_i2cAddress, 3);

  int16_t counter = 0;

  while ((_wire.available() < 3) && (counter++ < _timeout))
    delay(1);

  if ((_wire.available() < 3) && (counter >= _timeout))
    return -2;

  uint16_t hum = _wire.read();
  hum <<= 8;
  hum |= _wire.read();

  uint8_t chxsum = _wire.read();

  float humidity = hum;
  humidity *= 125;
  humidity /= 65536;
  humidity -= 6;

  return humidity;
}

float SI7021::readTemperature(void)
{
  if (_state <= 0)
    return -1;

  _wire.beginTransmission(_i2cAddress);
  _wire.write((uint8_t)SI7021_MEASTEMP_NOHOLD_CMD);
  _wire.endTransmission(false);
  delay(25);

  _wire.requestFrom(_i2cAddress, 3);

  int16_t counter = 0;

  while ((_wire.available() < 3) && (counter++ < _timeout))
    delay(1);

  if ((_wire.available() < 3) && (counter >= _timeout))
    return -2;

  uint16_t temp = _wire.read();
  temp <<= 8;
  temp |= _wire.read();

  uint8_t chxsum = _wire.read();

  float temperature = temp;
  temperature *= 175.72;
  temperature /= 65536;
  temperature -= 46.85;

  return temperature;
}

float SI7021::readTemperatureF(void)
{
  if (_state <= 0)
    return -1;

  float temperatureC = readTemperature();

  if (temperatureC < 0)
    return temperatureC;

  return((readTemperature() * 1.8) + 32.0); // Convert celsius to fahrenheit
}

void SI7021::readSerialNumber(void)
{
  _wire.beginTransmission(_i2cAddress);
  _wire.write((uint8_t)SI7021_ID1_CMD>>8);
  _wire.write((uint8_t)SI7021_ID1_CMD&0xFF);
  _wire.endTransmission();

  _wire.requestFrom(_i2cAddress, 8);
  _sernum_a = _wire.read();
  _wire.read();
  _sernum_a <<= 8;
  _sernum_a |= _wire.read();
  _wire.read();
  _sernum_a <<= 8;
  _sernum_a |= _wire.read();
  _wire.read();
  _sernum_a <<= 8;
  _sernum_a |= _wire.read();
  _wire.read();

  _wire.beginTransmission(_i2cAddress);
  _wire.write((uint8_t)SI7021_ID2_CMD>>8);
  _wire.write((uint8_t)SI7021_ID2_CMD&0xFF);
  _wire.endTransmission();

  _wire.requestFrom(_i2cAddress, 8);
  _sernum_b = _wire.read();
  _wire.read();
  _sernum_b <<= 8;
  _sernum_b |= _wire.read();
  _wire.read();
  _sernum_b <<= 8;
  _sernum_b |= _wire.read();
  _wire.read();
  _sernum_b <<= 8;
  _sernum_b |= _wire.read();
  _wire.read();
}

uint8_t SI7021::write8(uint8_t address, uint8_t value)
{
  _wire.beginTransmission(_i2cAddress);
  _wire.write(address);
  _wire.write(value);
  return (_wire.endTransmission());
}

uint8_t SI7021::read8(uint8_t address)
{
  uint8_t value;

  _wire.beginTransmission(_i2cAddress);
  _wire.write((uint8_t)address);
  _wire.endTransmission(false);

  _wire.requestFrom(_i2cAddress, 1);

  while ((_wire.available() < 1) && (_timeout-- > 0))
    delay(1);

  value = _wire.read();

  return value;
}

uint16_t SI7021::read16(uint8_t address)
{
  uint16_t value;

  _wire.beginTransmission(_i2cAddress);
  _wire.write((uint8_t)address);
  _wire.endTransmission();

  _wire.requestFrom(_i2cAddress, 2);

  while ((_wire.available() < 2) && (_timeout-- > 0))
    delay(1);

  value = _wire.read();
  value <<= 8;
  value |= _wire.read();

  return value;
}
