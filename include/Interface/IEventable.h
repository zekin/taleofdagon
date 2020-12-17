#ifndef IEVENTABLE
#define IEVENTABLE

#include "struct.h"

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


#endif