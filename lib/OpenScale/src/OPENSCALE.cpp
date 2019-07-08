#include "OPENSCALE.h"   // Use for Build IDE

float OPENSCALE::readWeight(void)
{
	while(Serial1.available()){
		char headerChar = Serial1.read();
		//Serial.print(headerChar);
	}

	//Send any character to trigger a read
  Serial1.print('0');

  int16_t counter = 0;

	//Now we need to spin to the first comma after the time stamp

  // while(Serial1.read() != ',')
	// {
	//   if(counter++ >= _timeout)
	// 	  return -2;
	//
  //   delay(1);
	// }

  //Now we read the weight
  counter = 0;
  String weightStr;

	while(1)
  {
    if(Serial1.available())
    {
      char incoming = Serial1.read();

			if(incoming == ',')
      {
	      return(weightStr.toFloat());
			}
      else
	    {
	      weightStr.concat(String(incoming));
	    }
	  }

    if(counter++ >= _timeout)
		  return -2;

		delay(1);
	}
}
