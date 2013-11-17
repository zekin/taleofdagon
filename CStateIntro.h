#ifndef CSTATEINTRO_H
#define CSTATEINTRO_H
#include "CGame.h"
#include <SDL/SDL.h>

#include "CEventManager.h"
#include "enum.h"

class CStateIntro : public IState {
private:
public:
  CStateIntro() {
    CEventManager::getInstance()->subscribe(0,this);
  }
  virtual void notify(Event* e) {
    
    if (e->type == EVENT_GAME_STARTED) {
      CEventManager::getInstance()->notify(Event(EVENT_PLAY_MUSIC,MUSIC_FOREST));
    }
    if (e->type == EVENT_KEYPRESS_DOWN) {
      if (e->a=='k')
        CEventManager::getInstance()->notify(Event(EVENT_PLAY_MUSIC,MUSIC_INTRO));
      if (e->a=='j')
        CEventManager::getInstance()->notify(Event(EVENT_PLAY_MUSIC,MUSIC_DESERT));
      if (e->a=='l')
        CEventManager::getInstance()->notify(Event(EVENT_PLAY_MUSIC,MUSIC_COMBAT1));
      if (e->a=='w')
        CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_START, DIRECTION_NORTH));
      if (e->a=='s')
        CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_START, DIRECTION_SOUTH));
      if (e->a=='a')
        CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_START, DIRECTION_WEST));
      if (e->a=='d')
        CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_START, DIRECTION_EAST));
    }
    if (e->type == EVENT_KEYPRESS_UP) {
      switch(e->a) {
        case 'w':
          CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_END, DIRECTION_NORTH)); break;
        case 'd':
          CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_END, DIRECTION_EAST)); break;
        case 's':
          CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_END, DIRECTION_SOUTH)); break;
        case 'a':
          CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_END, DIRECTION_WEST)); break;
        default:
          break;
      }
    }
    if (e->type == EVENT_RENDER_FRAME) {
      
    }
    if (e->type == EVENT_SWITCH_STATES) {
      if (e->a==STATE_INTRO)
        messaging_enabled=true;
    }
  }
};
class CStateGame : public IState {
private:
  bool enabled;
public:  
  CStateGame() : enabled(false) {
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