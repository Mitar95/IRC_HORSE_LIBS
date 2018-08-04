#include "SoftwareTimer.h"

SoftwareTimer::SoftwareTimer()	{
	timerFlag = false;
}

void SoftwareTimer::start(unsigned int timee)    {
    // If timer is not started
    if(!timerFlag)  {
        timerFlag = true;
        cTime = timee;
        cTimer = millis();
    }
}

bool SoftwareTimer::isStarted()    {
    return timerFlag;
}

bool SoftwareTimer::check()  {
    if(timerFlag)   {
        if(millis() - cTimer >= cTime)  {
            return true;
        }
    }
    return false;
}

void SoftwareTimer::stop()   {
    if(timerFlag)   {
        timerFlag = false;
    }
}

long SoftwareTimer::getCurrTime()    {
    return (millis() - cTimer);
}
