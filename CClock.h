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
  CClock() : time(0.0), fps(60.0), speed_scalar(1.0) { std::clog << "Clock was created successfully" << std::endl; }
  
  virtual ~CClock() {}
  float getTime() {
    return time;
  }
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
//    std::clog << "Clock was updated, time is : " << time << std::endl;
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

class CTimer {
  
  float time_started;
  float time_ending;
  CTimer(float time_end) : time_ending(time_end) {
    time_started=CClock::getInstance()->getTime();
  }
  CTimer(float time_end, bool repeat) {
  }
  //returns the number of times it finished, usually just need to know one happened
  float time_since() {
    return CClock::getInstance()->getTime() - time_started;
  }
  int finished() {
    if (time_since() < time_ending) {
      return 0;
    } else {
      return time_since()/time_ending;
    }
  }
  
};

#endif
