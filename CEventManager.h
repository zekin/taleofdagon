#ifndef CEVENTMANAGER_H
#define CEVENTMANAGER_H

#include <iostream>
#include <vector>

static unsigned int GlobalID=1000;
struct Event {
  Event() : type(0), a(0), b(0), c(0), x(0.0), y(0.0), z(0.0) {}
  Event(int type) : type(type), a(0), b(0), c(0), x(0.0), y(0.0), z(0.0) {}
  Event(int type, long data) : type(type), a(data), b(0), c(0), x(0.0), y(0.0), z(0.0) {}
  Event(int type, float x, float y, float z) : type(type), x(x), y(y), z(z) {}
  Event(int type, long data, float x) : type(type), a(data), b(0), c(0), x(x), y(0), z(0) {}
  
  unsigned int type;
  long a,b,c;
  float q,r,s,x,y,z;
};
class IEventable {
protected:
  unsigned int ID;
public:
IEventable() {
  ID=GlobalID;
  GlobalID++;
}
virtual void notify(Event*)=0;
unsigned int getID() {
  return ID;
}
};
class CEventManager {
private:
  std::vector<IEventable*> notify_list;
public:
  CEventManager() {
    std::clog << "EventManager been created" << std::endl;
  }
  ~CEventManager() {
  }
  void notify(Event e) {
//    std::clog << "notified that:" << e.type << " happened" << std::endl;
    for(int i=0; i<notify_list.size(); i++) {
      notify_list[i]->notify(&e);
    }
  }
  void subscribe(int EventType,IEventable* eventable) {
    std::clog << "ID " << eventable->getID() << " listening to events(" << EventType << ")" << std::endl;
    notify_list.push_back(eventable);
  }
  static CEventManager* getInstance() {
    static CEventManager* instance=0;
    if (instance==0) {
      instance = new CEventManager();
    }
    return instance;
  }
};
#endif
