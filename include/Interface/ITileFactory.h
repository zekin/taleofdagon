#ifndef ITILEFACTORY_H
#define ITILEFACTORY_H

#include "CEventManager.h"
#include "enum.h"

class CTile;

class ITileFactory : public IEventable {
public:
//    enum {
//        UNIT_FIGHTER,
//        UNIT_KNIGHT,
//        UNIT_VILLAGER
//    };
    
    virtual CTile* createTile(int tileType, int areaType) = 0;
};

#endif