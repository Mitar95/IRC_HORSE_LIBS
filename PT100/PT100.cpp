#include "PT100.h"

void PT100::init(uint8_t input_pin, double *points, double R1, double voltage)	{
	pin = input_pin;
	pinMode(pin, INPUT_PULLDOWN);

	r1 = R1;
	vcc = voltage;

	double va1 = points[0];
	double vd1 = points[1];
	double va2 = points[2];
	double vd2 = points[3];

	double a1 = vd1/va1;
	double a2 = vd2/va2;

	a[0] = (a2 - a1)/(vd1 - vd2);
	a[1] = a1 + vd1*a[0];

	rttk[0] = (points[6] - points[4])/(points[7] - points[5]);
	rttk[1] = points[4] - points[5] * rttk[0];

	adK = vcc/4180.0;

	requested = false;
	ready = false;
	currSample = 0;

	adOld = analogRead(pin);

	acceleration = 0.1;
}



bool PT100::calibrate(uint16_t maxTimeInMs)	{
	requestTemperature();

	SoftwareTimer exitTimer;
	exitTimer.start(maxTimeInMs);
	bool looping = true;
	double oldTemp = 0;
	bool firstRun = true;

	while(looping)	{

		getSample();

		if(isReady())	{
			if(firstRun)	{
				oldTemp = temperature;
				firstRun = false;
			}else	{
				if(oldTemp > temperature - 0.5 && oldTemp < temperature + 0.5)	{
					return true;
				}else	{
					oldTemp = temperature;
				}
			}
			requestTemperature();
		}

		if(exitTimer.check())	{
			return false;
		}
	}
}

void PT100::requestTemperature()	{
	// ad = 0;
	ready = false;
	requested = true;
}

bool PT100::isReady()	{
	return ready;
}

bool PT100::isRequested()	{
	return requested;
}

double PT100::getTemperature()	{
	// ad = ad / maxNumOfSamples;
	vMeasured = adK * ad;
	aCalculated = getAmplification(&vMeasured);
	vaCalculated = vMeasured / aCalculated;
	rx = (vaCalculated*r1)/(vcc - vaCalculated);
	temperature = rttk[0]*rx + rttk[1];
	ready = false;
	return temperature;
}

void PT100::getSample()	{
	if(requested && !ready)	{
		if(!dampTimer.isStarted())	{
			adNew = analogRead(pin);

				if(adNew > adOld - adOffs && adNew < adOld + adOffs)	{
					adOld = adNew;
					ad += adNew;
				}else	{
					if(adNew > adOld)	{
						adOld += acceleration;
					}else	{
						adOld -= acceleration;
					}
					ad += adOld;
				}
				currSample++;

				if(currSample == maxNumOfSamples)	{
					ready = true;
					requested = false;
					currSample = 0;
					ad = ad / maxNumOfSamples;
				}
			dampTimer.start(10);
		}else	{
			if(dampTimer.check())	{
				dampTimer.stop();
			}
		}
	}
}

double PT100::getLastAD()	{	return ad;	}
double PT100::getLastTemperature()	{	return temperature;	}
double PT100::getLastVoltageA()	{	return vaCalculated;	}
double PT100::getLastVoltageD()	{	return vMeasured;	}
double PT100::getLastResistance()	{	return rx;	}
double PT100::getLastAmplification()	{	return aCalculated;	}



double PT100::get()	{
	vMeasured = atv();
	aCalculated = getAmplification(&vMeasured);
	vaCalculated = vMeasured / aCalculated;
	double rx = (vaCalculated*r1)/(vcc - vaCalculated);
	return rtt(&rx);
}

double PT100::getCalculatedVa(double *voltage)	{
	return (*voltage/getAmplification(voltage));
}

double PT100::getRawAD()	{
	return analogRead(pin);
}

double PT100::getResistance()	{
	ad = atv();
	vaCalculated = getCalculatedVa(&ad);
	return vtr(&vaCalculated);
}

double PT100::getRawVoltage()	{
	return atv();
}

double PT100::atv()	{
	return (adK*float(analogRead(pin)));
}

double PT100::getAmplification(double *voltage)	{
	return (-a[0]*(*voltage) + a[1]);
}

double PT100::vtr(double *voltage)	{
	return (*voltage/(vcc - *voltage))*r1;
}

double PT100::rtt(double *resistance)	{
	return (rttk[0]*(*resistance) + rttk[1]);
}