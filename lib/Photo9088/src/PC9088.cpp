#include "PC9088.h"   // Use for Build IDE

void PC9088::turnOn(void)
{
	// turn the control pin high to supply current to the voltage divider
	digitalWrite(_controlPin, HIGH);
	_state = 1;
}

void PC9088::turnOff(void)
{
	digitalWrite(_controlPin, LOW);
	_state = 0;
}

float PC9088::readLight(void)
{
	if (_state <= 0)
		return -1;

	int lightADC = analogRead(_lightPin);

	if (lightADC > 0)
	{
	  // Use the ADC reading to calculate voltage and resistance
	  float lightV = lightADC * VCC / 4095.0;
	  float lightR = R_DIV * (VCC / lightV - 1.0);

		return lightR;
	}

	return -2;
}

float PC9088::readVolt(void)
{
	if (_state <= 0)
		return -1;

	int lightADC = analogRead(_lightPin);

	if (lightADC > 0)
	{
	  // Use the ADC reading to calculate voltage and resistance
		float lightV = lightADC * VCC / 4095.0;
	  return lightV;
	}

	return -2;
}
