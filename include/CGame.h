#ifndef CGAME_H
#include <vector>
#include "CEventManager.h"
#include "enum.h"
class IState : public IEventable {
private:
  int state_id;
public:
  IState() {
  }
  virtual int get_state_id() {
      return state_id;
  }
};
class CGame : public IEventable {
private:
  std::vector<IState*> state_list;
  IState* current_state;
public:
  CGame() {
  }
  void add(IState* state) {
    if (state) {
//      std::clog << "Added State ID " << state->ID << std::endl;
      state_list.push_back(state);
    }
    std::clog << "Could not add state to game" << std::endl;
  }
  int state() {
    return current_state->get_state_id();
  }
  virtual void notify(Event* e) {
    if (e->type == EVENT_SWITCH_STATES) {
      if (e->a==0) {
        std::clog << "Error: Invalid State to switch to, defaulting to Intro" << std::endl;
      }
      switch(e->a) {
        case STATE_INTRO:
          current_state=state_list[STATE_INTRO];
          break;
        case STATE_MENU:
          current_state=state_list[STATE_MENU];
          break;
        case STATE_CHARACTER_SELECT:
          current_state=state_list[STATE_CHARACTER_SELECT];
          break;
        case STATE_GAME:
          current_state=state_list[STATE_GAME]; 
          break;
      }
    }
  }
  
};

#endif