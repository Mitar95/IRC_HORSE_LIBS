#ifndef BLDC_h
#define BLDC_h

#include "Arduino.h"

class BLDC	{
	public:
		void init(uint8_t enablePin, uint8_t directionPin, uint8_t speedPin);
		void start();
		void stop();
		bool isStarted();
		void setDirection(bool direction);
		void setSpeed(uint8_t pwm);
		void update();
	private:
		uint8_t enPin, dirPin, spePin;
		bool running, dir;
		uint8_t speed;
};

#endif