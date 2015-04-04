#ifndef CENGINE_H
#define CENGINE_H
#include <vector>
#include "CEventManager.h"
#include "enum.h"
#include "CStateIntro.h"
#include "logging.h"

class CEngine : public IEventable {
private:
    CStateMainMenu        state_mainmenu;
    CStateCharacterSelect state_characterselect;
    CStateGame            state_game;
    IState*               current_state;
public:
    CEngine() :
        current_state(0) {
        CEventManager::getInstance()->subscribe(0,this);
        current_state=&state_mainmenu;
    }
    virtual void notify(Event* e) {
        if (e->type == EVENT_SWITCH_STATES) {
            if (e->a==0) {
                INFO(LOG) << "Error: Invalid State to switch to, defaulting to Intro";
            }
            Event initialize_event(EVENT_INITIALIZE);
            Event cleanup_event(EVENT_CLEANUP);
            switch(e->a) {
            case STATE_MENU:
//          current_state->notify(&cleanup_event); //out with the old
                CEventManager::getInstance()->unsubscribe(0,current_state);
                current_state=&state_mainmenu;
                current_state->notify(&initialize_event); //in with the new
                CEventManager::getInstance()->subscribe(0,current_state);
                break;
            case STATE_CHARACTER_SELECT:
                current_state->notify(&cleanup_event); //out with the old
                CEventManager::getInstance()->unsubscribe(0,current_state);
                current_state=&state_characterselect;
                current_state->notify(&initialize_event); //in with the new
                CEventManager::getInstance()->subscribe(0,current_state);
                break;
            case STATE_GAME:
                current_state->notify(&cleanup_event); //out with the old
                CEventManager::getInstance()->unsubscribe(0,current_state);
                current_state=&state_game;
                current_state->notify(&initialize_event); //in with the new
                CEventManager::getInstance()->subscribe(0,current_state);
                break;
            }
        }
    }
};

#endif
