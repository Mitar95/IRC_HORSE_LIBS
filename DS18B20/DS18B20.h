#ifndef TemperatureSensor_h
#define TemperatureSensor_h

#include "Arduino.h"
#include "OneWire.h"
#include "DallasTemperature.h"

class DS18B20	{
public:
	void init(uint8_t pin);
	float getTemperature();
	void requestTemperature();
	bool isReady();
	float get();

private:
	OneWire *oneWire;
	DallasTemperature *tempSens;
};

#endif