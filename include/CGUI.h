#ifndef CGUI_H
#define CGUI_H

#include "CEventManager.h"
#include "enum.h"
class IWidget : public IEventable {
public:
IWidget() {
}
virtual void render()=0;
};

class CButton : public IWidget {
public:
CButton() {

}
void notify(Event* e) {
  if (e->type == EVENT_KEYPRESS_DOWN) {
    CEventManager::getInstance()->notify(Event());
  }
  
}
void render() {
}
};

class CGUI : public IEventable {
public:
  CGUI() {
  }
  virtual void notify(Event* e) {
    switch (e->type) {
    case EVENT_CAMERA_MOVE_START:
      switch(e->a) {
      case DIRECTION_NORTH:
        break;
      case DIRECTION_SOUTH:
        break;
      }
      break;
    case EVENT_RENDER_FRAME:
      
      break;
    default:
      break;
      
    }
  }
};

#endif

