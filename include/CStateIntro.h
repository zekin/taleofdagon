#ifndef CSTATEINTRO_H
#define CSTATEINTRO_H
#include "CGame.h"
#include <SDL/SDL.h>

#include "CEventManager.h"
#include "enum.h"
#include "CMap.h"
#include "CGUI.h"

class CStateIntro : public IState {
private:
  CGUIMainMenu GUI;
public:
  CStateIntro() : GUI() {
    CEventManager::getInstance()->subscribe(0,this);
  }
  virtual void notify(Event* e) {
    
    if (e->type == EVENT_GAME_STARTED) {
      CEventManager::getInstance()->notify(Event(EVENT_PLAY_MUSIC,MUSIC_INTRO));
    }
    if (e->type == EVENT_KEYPRESS_DOWN) {
      if (e->a=='w')
        CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_START, DIRECTION_NORTH));
      if (e->a=='s')
        CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_START, DIRECTION_SOUTH));
    }
    if (e->type == EVENT_RENDER_FRAME) {
      
    }
    if (e->type == EVENT_SWITCH_STATES) {
//      if (e->a==STATE_INTRO)
//        messaging_enabled=true;
    }
  }
};
class CStateGame : public IState {
private:
  bool enabled;
  CMap WorldMap;
public:  
  CStateGame() : enabled(false), WorldMap(100,100) {
    CEventManager::getInstance()->subscribe(0,this);
  }
  virtual void notify(Event* e) {
   switch(e->type) {
      case EVENT_GAME_STARTED:
        CEventManager::getInstance()->notify(Event(EVENT_PLAY_MUSIC,MUSIC_FOREST));
        break;
      case EVENT_KEYPRESS_DOWN:
        if (e->a=='w')
          CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_START, DIRECTION_NORTH));
        if (e->a=='s')
          CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_START, DIRECTION_SOUTH));
        if (e->a=='a')
          CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_START, DIRECTION_WEST));
        if (e->a=='d')
          CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_START, DIRECTION_EAST));
        break;
      case EVENT_KEYPRESS_UP:
        if (e->a=='w')
          CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_START, DIRECTION_NORTH));
        if (e->a=='s')
          CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_START, DIRECTION_SOUTH));
        if (e->a=='a')
          CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_START, DIRECTION_WEST));
        if (e->a=='d')
          CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_START, DIRECTION_EAST));
        break;
      case EVENT_RENDER_FRAME:
        break;
    }
  }
  
};
#endif