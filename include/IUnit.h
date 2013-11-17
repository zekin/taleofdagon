#ifndef UNIT_H
#define UNIT_H

#include "CEventManager.h"
#include <iostream>

class IUnit {
private:
public:
IUnit() {
}
virtual void notify() {
}
};
class CUnit : public IUnit, public IEventable {
public:
CUnit() {
  std::clog << "Created Unit : ";
  CEventManager::getInstance()->subscribe(10,this);
}
virtual void notify(Event *e) {
  std::clog << this->getID() << " got message type: " << e->type << std::endl;
}
};
#endif
