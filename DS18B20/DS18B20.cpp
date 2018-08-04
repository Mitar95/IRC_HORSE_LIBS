#include "DS18B20.h"
#include "../OneWire/OneWire.h"
#include "../DallasTemperature/DallasTemperature.h"

void DS18B20::init(uint8_t pin)	{
	oneWire = new OneWire(pin);
	tempSens = new DallasTemperature(&*oneWire);
	tempSens->setWaitForConversion(false);
	tempSens->begin();
}

float DS18B20::getTemperature()	{
	float temp;

	tempSens->requestTemperatures();
	temp = tempSens->getTempCByIndex(0);

	return temp;
}

void DS18B20::requestTemperature()	{
	// tempSens->setWaitForConversion(false);
	tempSens->requestTemperaturesByIndex(0);
}

bool DS18B20::isReady()	{
	return tempSens->isConversionAvailable(0);
}

float DS18B20::get()	{
	tempSens->getTempCByIndex(0);
}
