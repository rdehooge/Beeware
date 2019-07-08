#ifndef OPENSCALE_h
#define OPENSCALE_h

#include "application.h"

class OPENSCALE {
 public:
  OPENSCALE(Stream& stream)
    : _stream(stream)
  {
  }

  float readWeight(void);

 private:
   int16_t _timeout = 10000;
   Stream& _stream;
};

#endif
