#ifndef ITILEFACTORY_H
#define ITILEFACTORY_H

#include "CEventManager.h"
#include "enum.h"

class CTile;

class ITileFactory : public IEventable {
public:
    virtual CTile* createTile(int tileType, int areaType) = 0;
};

#endif