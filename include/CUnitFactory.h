#ifndef CUNITFACTORY_H
#define CUNITFACTORY_H

#include "IUnitFactory.h"
#include "IUnit.h"
#include "CUnitFighter.h"
#include "CUnitVillager.h"
#include "CUnitKnight.h"
#include "CLocator.h"
#include "IMap.h"

class CUnitFactory : public IUnitFactory {
protected:

public:
    CUnitFactory() {
    }

    virtual IUnit* createUnit(int unitType, float x, float y) {
        CEventManager* events=CEventManager::getInstance();
        IMap* map = CLocator::getMap();
        IUnit* createdUnit=NULL;

        switch(unitType) {
        case UNIT_TYPE_FIGHTER:
            createdUnit = (IUnit*)new CUnitFighter(x,y,0);
            break;
        case UNIT_TYPE_KNIGHT:
            createdUnit = (IUnit*)new CUnitKnight(x,y,0);
            break;
        case UNIT_TYPE_VILLAGER:
            createdUnit = (IUnit*)new CUnitVillager(x,y,0);
            break;
        default:
            break;
        }

        if (createdUnit == NULL) {
            INFO(LOG) << "Failed to create unit. Exiting.";
            events->notify(EVENT_GAME_ENDED);
        }

        events->notify(EVENT_UNIT_CREATED);

        map->getChunk(x,y)->addObject(createdUnit);
        return createdUnit;
    }
    virtual void notify( Event* e ) {
    }
};

#endif
