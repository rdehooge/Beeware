#ifndef Display_H
#define Display_H

#include <epd29.h>
#include <epdpaint.h>
#include <SPI.h>

#define COLORED     0
#define UNCOLORED   1

//296 × 128
const int width = 128;
const int height = 296;

typedef struct _DeviceData
{
  double Voltage;
  double Charge;
  int Alert;
  double Strength;
  double Quality;
  double Humidity;
  double Temp;
  double Light;
  double Weight;
} DeviceData;

class Display {
public:
    Display(SPIClass& spi, int16_t dc, int16_t cs, int16_t reset, int16_t busy)
    {
      _epd = new Epd(spi, dc, cs, reset, busy);
      _image = new unsigned char[4736];
      _paint = new Paint(_image, width, height);
    }

    ~Display()
    {
      delete _paint;
      delete _image;
      delete _epd;
    }

    int Setup(void);
    int Update(DeviceData &data);

  private:
    unsigned char* _image;
    Epd* _epd;
    Paint* _paint;    //width should be the multiple of 8
};

#endif
