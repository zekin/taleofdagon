#ifndef CCLOCK_H
#define CCLOCK_H

#include "CEventManager.h"
#include <SDL/SDL.h>
#include "enum.h"

class CClock : public IEventable {
private:
  float time;
  float fps;
  float speed_scalar;
  
public:
  CClock(float fps, float gamespeed) : time(0.0), fps(fps), speed_scalar(gamespeed) {}
  
  virtual ~CClock() {}
  void setFPS(float fps) {
    if (fps < 1) {
      std::clog << "Error: Can not set FPS below 1" << std::endl;
    } else {
      fps=fps;
    }
  }
  
  void setSpeed(float speed) {
    if (speed < 0.1) 
      speed_scalar=0.1;
    else if (speed > 3.0) 
      speed_scalar=3.0;
    else 
      speed_scalar=speed;
    std::clog << "Game speed set to " << speed << std::endl;
  }
  
  void update() {
    float delay=1.0/fps;
    float time_now=SDL_GetTicks()/1000.0;
    CEventManager::getInstance()->notify(Event(EVENT_RENDER_FRAME, time_now,0,0));
    if (time_now-time+delay > 0) {
      SDL_Delay((time_now-time+delay)*1000);
    }
    time=SDL_GetTicks()/1000.0;
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
};

#endif
