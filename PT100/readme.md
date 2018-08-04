# VCC = 3.402V #

DS3          | MS1          | AS1          | HS11
------------ | ------------ | ------------ | -------------
va1 = 0.037  | va1 = 0.037  | va1 = 0.037  | va1 = 0.037
vd1 = 0.220  | vd1 = 0.217  | vd1 = 0.219  | vd1 = 0.217
va2 = 0.061  | va2 = 0.0628 | va2 = 0.0604 | va2 = 0.0604
vd2 = 0.349  | vd2 = 0.349  | vd2 = 0.344  | vd2 = 0.344
t1 = 0       | t1 = 0       | t1 = 0       | t1 = 0
rt1 = 100    | rt1 = 100    | rt1 = 100    | rt1 = 100
t2 = 440     | t2 = 440     | t2 = 440     | t2 = 440
rt2 = 260.72 | rt2 = 260.72 | rt2 = 260.72 | rt2 = 260.72

---

# Example 1 #

```
#include "PT100.h'
#include "Arduino.h"

void setup()	{}
void loop()	{
	// Create Object.
	PT100 ds3;

	// Initialize the sensor.
	double points[8] = {
		0.037, 0.220, 0.0614, 0.349, 
		0.0, 100.0, 440.0, 260.72
	};	// Data points from table.
	ds3.init(PA0, points, 9960.0, 3.402);

	if(ds3.calibrate(10000))	{
		// Sensor was calibrated.
	}else	{
		// Was not able to calibrate sensor in 10s.
		// Try again or continue without calibration.
	}

	while(1)	{
		if(<someTimer>)	{
			ds3.requestTemperature();
		}

		if(ds3.isRequested())	{
			ds3.getSample();

			if(d3.isReady())	{
				d3.getTemperature();
				d3.getLastTemperature();

				// Do something with data.
			}
		}
	}
}
```