#ifndef CENGINE_H
#define CENGINE_H
#include <vector>
#include "CEventManager.h"
#include "enum.h"
#include "CStateIntro.h"
#include "logging.h"

class CEngine : public IEventable {
private:
    CStateMainMenu        stateMainMenu;
    CStateCharacterSelect stateCharacterSelect;
    CStateGame            stateGame;
    IState*               currentState;
public:
    CEngine() :
        currentState(0) {
        CEventManager::getInstance()->subscribe(0,this);
        currentState=&stateMainMenu;
        CLocator::setMap(CWorldMap::getInstance());
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
                CEventManager::getInstance()->unsubscribe(0,currentState);
                currentState=&stateMainMenu;
                currentState->notify(&initialize_event); //in with the new
                CEventManager::getInstance()->subscribe(0,currentState);
                break;
            case STATE_CHARACTER_SELECT:
                currentState->notify(&cleanup_event); //out with the old
                CEventManager::getInstance()->unsubscribe(0,currentState);
                currentState=&stateCharacterSelect;
                currentState->notify(&initialize_event); //in with the new
                CEventManager::getInstance()->subscribe(0,currentState);
                break;
            case STATE_GAME:
                currentState->notify(&cleanup_event); //out with the old
                CEventManager::getInstance()->unsubscribe(0,currentState);
                currentState=&stateGame;
                currentState->notify(&initialize_event); //in with the new
                CEventManager::getInstance()->subscribe(0,currentState);
                break;
            }
        }
    }
};

#endif
