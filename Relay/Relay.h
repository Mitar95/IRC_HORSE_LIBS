#ifndef Relay_h
#define Relay_h

#include "Arduino.h"

class Relay	{
	public:
		void init(uint8_t relayPin);
		void on();
		void off();
		bool isOn();
		bool tryChangeState(float realTemp, float targetTemp, float offsetTemp);
	private:
		uint8_t pin;
		bool state;
};

#endif