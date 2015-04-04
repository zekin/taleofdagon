#ifndef IUNITFACTORY_H
#define IUNITFACTORY_H

#include "CEventManager.h"
#include "IUnit.h"

class IUnitFactory : public IEventable {
public:
    enum {
        UNIT_FIGHTER,
        UNIT_KNIGHT,
        UNIT_VILLAGER
    };
    virtual IUnit* createUnit(int unitType, float x, float y) = 0;

};

#endif
