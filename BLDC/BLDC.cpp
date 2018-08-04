#include "BLDC.h"

void BLDC::init(uint8_t enablePin, uint8_t directionPin, uint8_t speedPin)	{
	dirPin = directionPin;
	enPin = enablePin;
	spePin = speedPin;
	speed = 0;
	dir = false;
	running = false;

	pinMode(enPin, OUTPUT);
	pinMode(spePin, OUTPUT);
	pinMode(dirPin, OUTPUT);

	digitalWrite(spePin, speed);
	
	digitalWrite(enPin, 1);
	// delay(100);
	// digitalWrite(enPin, 0);
	// delay(100);
	// digitalWrite(enPin, 1);

	digitalWrite(dirPin, dir);
}

void BLDC::start()	{
	running = true;
	analogWrite(spePin, speed);
	digitalWrite(dirPin, dir);
	delay(50);
	digitalWrite(dirPin, 1);
	digitalWrite(enPin, 0);
	delay(50);
	digitalWrite(enPin, 1);
}

void BLDC::stop()	{
	running = false;
	digitalWrite(enPin, 0);
	delay(50);
	digitalWrite(enPin, 1);
}

bool BLDC::isStarted()	{
	return running;
}

void BLDC::setDirection(bool direction)	{
	dir = direction;
}

void BLDC::setSpeed(uint8_t pwm)	{
	speed = pwm;
}

void BLDC::update()	{
	if(running)	{
		analogWrite(spePin, speed);
		digitalWrite(dirPin, dir);
		delay(10);
		digitalWrite(dirPin, 1);
	}
}