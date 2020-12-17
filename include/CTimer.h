#ifndef CTIMER_H
#define CTIMER_H
#include "CClock.h"

class CTimer {

    float time_started;
    float time_ending;
    bool repeat;
public:
    CTimer(float time_end) : time_ending(time_end) {
        time_started=CClock::getInstance()->getTime();
        time_ending=time_end;
    }
    CTimer(float time_end, bool repeat) : repeat(repeat) {
        time_started=CClock::getInstance()->getTime();
        time_ending=time_end;
    }
    //returns the number of times it finished, usually just need to know one happened
    float time_since() {
        return CClock::getInstance()->getTime() - time_started;
    }
    float set_time_ending(float time_end) {
        time_ending=time_end;
    }
    int finished() {
        if (time_since() < time_ending) {
            return 0;
        } else {
            if (repeat) {
                time_started=CClock::getInstance()->getTime();
            }
            return 1;
        }
    }

};

#endif