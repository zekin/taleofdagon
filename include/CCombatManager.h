#ifndef CCOMBATMANAGER_H
#define CCOMBATMANAGER_H
#include "CEventManager.h"
#include "enum.h"

class CCombatManager : public IEventable {
public:
    CCombatManager() {
    }
    virtual void notify(Event* e) {
      switch(e->type) {
        case EVENT_UNIT_HIT_UNIT_WITH:
          CEventManager::getInstance()->notify(Event(EVENT_COMBAT_STARTED,0));
          break;
      }
    }
  
};
#endif