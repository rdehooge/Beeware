#ifndef PC9088_h
#define PC9088_h

#include "application.h"

// Measure the voltage at 3.3V and the actual resistance of your
// 47k resistor, and enter them below:
const float VCC = 3.3; // Measured voltage of Photon 3.3V line
const float R_DIV = 4610.0; // Measured resistance of 4.7k resistor

// Set this to the minimum resistance require to turn an LED on:
const float DARK_THRESHOLD = 10000.0;

class PC9088 {
 public:
  PC9088(int lightPin, int controlPin)
  : _lightPin(lightPin), _controlPin(controlPin)
  {
    pinMode(_lightPin, INPUT);
  	pinMode(_controlPin, OUTPUT);
  }

  void turnOn(void);
	void turnOff(void);
	float readVolt(void);
	float readLight(void);

 private:
	 int _state = -1;
	 int _lightPin = -1;
	 int _controlPin = -1;
   int16_t _timeout = 100;
};

#endif
