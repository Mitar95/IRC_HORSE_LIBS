
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Class used for software timer. Based on millis() function
// that uses TIME1 for counting ellapsed time since program
// start. millis() is type UNSIGNED LONG.
// Class provides set of functions for starting, stoping and
// chacing if set time has ellapsed.
//
// TIMER IS TYPE TON.
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifndef SoftwareTimer_h
#define SoftwareTimer_h

#include "Arduino.h"

class SoftwareTimer  {

public:
	SoftwareTimer();
    // Start conunting.
    // @param timee - how much time needs to pass in millis.
    void start(unsigned int timee);

    // Returns TRUE if set time has ellapsed.
    bool check();

    // Stops counting.
    void stop();

    // Is timer started.
    bool isStarted();

    // Get ellapsed time since calling of start() function.
    long getCurrTime();

private:
    // Stores millis() value up on calling start().
    unsigned long cTimer;

    // Stores set time.
    unsigned int cTime;

    // Is timer is started.
    bool timerFlag;
};

#endif
