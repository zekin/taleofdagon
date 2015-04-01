#ifndef CEVENTMANAGER_H
#define CEVENTMANAGER_H

#include <iostream>
#include <vector>
#include "logging.h"

static unsigned int global_id=1000;
struct Event {
  Event() : type(0), a(0), b(0), c(0), x(0.0), y(0.0), z(0.0) {}
  Event(int type) : type(type), a(0), b(0), c(0), x(0.0), y(0.0), z(0.0) {}
  Event(int type, long data) : type(type), a(data), b(0), c(0), x(0.0), y(0.0), z(0.0) {}
  Event(int type, std::string string) : type(type), a(0), b(0), c(0), x(0), y(0), z(0), string(string) {}
  Event(int type, float x, float y, float z) : type(type), x(x), y(y), z(z) {}
  Event(int type, long data, float x) : type(type), a(data), b(0), c(0), x(x), y(0), z(0) {}
  std::string string;
  unsigned int type;
  long a,b,c;
  float q,r,s,x,y,z;
};
class IEventable {
  protected:
  unsigned int id;
  bool enabled;
  public:
  IEventable() : enabled(false) {
    id=global_id;
    global_id++;
  }  
  virtual bool itsOKToContinue(Event* e) {
    /* events we respond to while we're in memory */
   return enabled;
  }
  void enable(bool on) {
    enabled=on;
  }
  bool isEnabled() {
    return enabled;
  }
  virtual void notify(Event*)=0;
  unsigned int getID() {
    return id;
  }
};
class CEventManager {
private:
  std::vector<IEventable*> notify_list;
  
public:
  CEventManager() {
    INFO(LOG) << "EventManager been created";
  }
  ~CEventManager() {}
  void notify(Event e) {
//    std::clog << "notified that:" << e.type << " happened" << std::endl;
    for(int i=0; i<notify_list.size(); i++) {
      notify_list[i]->notify(&e);
    }
  }
  void unsubscribe(int EventType, IEventable* eventable) {
    for(int i=0; i<notify_list.size(); i++) {
      if (notify_list[i]==eventable) {
        INFO(LOG) << "ID " << eventable->getID() << " has unsubscribed from " << EventType;
        notify_list.erase(notify_list.begin()+i);
      }
    }
    
  }
  void subscribe(int EventType,IEventable* eventable) {
    for (int i=0; i<notify_list.size(); i++) {
      if (notify_list[i]==eventable) {
        INFO(LOG) << "ID " << eventable->getID() << " is already listening to events.";
        return;
      }
    }
    
    INFO(LOG) << "ID " << eventable->getID() << " listening to events(" << EventType << ")";
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