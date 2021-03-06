#ifndef CCLOCK_H
#define CCLOCK_H

#include "CEventManager.h"
#include <SDL/SDL.h>
#include "enum.h"
#include "logging.h"

class CClock : public IEventable {
private:
    float time;
    float delta;
    float fps;
    float speed_scalar;

public:
    CClock() :
        delta(0),
        time(0.0),
        fps(30.0),
        speed_scalar(1.0) {
        INFO(LOG) << "Clock was created successfully";
    }

    virtual ~CClock() {}
    float getTime() {
        return time;
    }
    void setFPS(float fps) {
        if (fps < 1) {
            INFO(LOG) << "Error: Can not set FPS below 1";
        } else {
            fps=fps;
        }
    }

    float time_of_day() {
        return fmod(getTime(),360.0f);
    }
    void setSpeed(float speed) {
        if (speed < 0.1)
            speed_scalar=0.1;
        else if (speed > 3.0)
            speed_scalar=3.0;
        else
            speed_scalar=speed;
        INFO(LOG) << "Game speed set to " << speed;
    }

    float deltaT() {
        return delta;
    }
    void update() {
        float delay=1.0/fps;
        float time_now=SDL_GetTicks()/1000.0;
        delta = time_now - time;
        CEventManager::getInstance()->notify(Event(EVENT_RENDER_FRAME, time_now,0,0));
        time=time_now;
    }

    virtual void notify(Event* e) {
        switch(e->type) {
        case EVENT_SET_SPEED:
            setSpeed(e->a);
            break;
        default:
            break;
        }
    }

    static CClock* getInstance() {
        static CClock* instance=0;

        if (instance==0) {
            instance=new CClock();
        }
        return instance;
    }
};

#endif
