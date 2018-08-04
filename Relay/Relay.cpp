#include "Relay.h"

void Relay::init(uint8_t relayPin)	{
	state = false;
	pin = relayPin;

	pinMode(pin, OUTPUT);
	digitalWrite(pin, state);
}

void Relay::on()	{
	state = true;
	digitalWrite(pin, state);
}

void Relay::off()	{
	state = false;
	digitalWrite(pin, state);
}

bool Relay::isOn()	{
	return state;
}

bool Relay::tryChangeState(float realTemp, float targetTemp, float offsetTemp){
	if(isOn()){
        if(realTemp > targetTemp + offsetTemp){
			off();
			return true;
		}
    }else{
		if(realTemp < targetTemp - offsetTemp){
			on();
			return true;
		}
	}

	return false;
}
