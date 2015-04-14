#ifndef CTILEFACTORY_H
#define CTILEFACTORY_H

#include "ITileFactory.h"
#include "IMap.h"

class CTileFactory : public ITileFactory {
protected:

public:
    CTileFactory() {
    }

    virtual CTile* createTile(int tileType, int areaType) {
        CEventManager* events=CEventManager::getInstance();
        IMap* map = CLocator::getMap();
        CTile* createdTile=NULL;

        switch(tileType) {
            case TILE_CITY_WALL:
                createdTile = new CTileCityWall();
                break;
            case TILE_CITY_HOUSE_WALL:
                createdTile = new CTileShopWall();
                break;
            case TILE_CITY_HOUSE_FLOOR:
                createdTile = new CTileShopFloor();
                break;
            default:
                createdTile = new CTile(tileType, areaType);
                break;
        }

        if (createdTile == NULL) {
            INFO(LOG) << "Failed to create tile. Exiting.";
            events->notify(EVENT_GAME_ENDED);
        }
        

        return createdTile;
    }
    virtual void notify( Event* e ) {
    }
};

#endif
