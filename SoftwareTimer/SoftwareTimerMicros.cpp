#include "SoftwareTimerMicros.h"

SoftwareTimerMicros::SoftwareTimerMicros()	{
	timerFlag = false;
}

void SoftwareTimerMicros::start(unsigned int timee)    {
    // If timer is not started
    if(!timerFlag)  {
        timerFlag = true;
        cTime = timee;
        cTimer = micros();
    }
}

bool SoftwareTimerMicros::isStarted()    {
    return timerFlag;
}

bool SoftwareTimerMicros::check()  {
    if(timerFlag)   {
        if(micros() - cTimer >= cTime)  {
            return true;
        }
    }
    return false;
}

void SoftwareTimerMicros::stop()   {
    if(timerFlag)   {
        timerFlag = false;
    }
}

long SoftwareTimerMicros::getCurrTime()    {
    return (micros() - cTimer);
}
