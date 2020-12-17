#ifndef CEVENTMANAGER_H
#define CEVENTMANAGER_H

#include <iostream>
#include <vector>
#include "logging.h"
#include "struct.h"
#include "IEventable.h"

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
