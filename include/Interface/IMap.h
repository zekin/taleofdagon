#ifndef IMAP_H
#define IMAP_H

#include "enum.h"
#include "CEventManager.h"
#include "CRenderable.h"
#include "CUnit.h"
#include "CObjectUtil.h"
#include "IChunk.h"
#include "IUnitFactory.h"
#include "CLocator.h"

class CUnit;

class CTile {
public:
    char tileType;
    char area;
    CTile(char tile_number, char area=AREA_GRASSLANDS) : tileType(tile_number), area(area) {}
    CLASSIFIER getClassifier() {
        switch(tileType) {
        case TILE_GRASS:
        case TILE_SAND:
            return GROUND;
        case TILE_WATER:
            return SEA;
        case TILE_MOUNTAIN:
            return CLIFF;
        }
    }
};


class CChunk : public IChunk {
private:
    std::vector<CTile*> tiles;
    std::vector<CRenderable*> objects;
    std::vector<CUnit*> units;

    bool initialized;
    int chunk_number;
public:
    CChunk(int chunk_number) :
        initialized(false),
        chunk_number(chunk_number) {}

    CTile* at(int x, int y) {
        if (!initialized) {
            setTiles(x,y);
        }
        x=x%Globals::chunk_size;
        y=y%Globals::chunk_size;
        return tiles[y*Globals::chunk_size+x];
    }
    CRenderable* getObject(int number) {
        if (number >= objects.size()) {
            return 0;
        } else {
            return objects.at(number);
        }
    }
    void addObject(CRenderable* object) {
        for (int i=0; i<objects.size(); i++) {
            if ( object == objects[i] ) {
                INFO(LOG) << "Object already registered inside chunk number " << chunk_number;
                return;
            }
        }

        INFO(LOG) << "Object added to chunk number " << chunk_number;

        objects.push_back(object);
        
        if (CObjectUtil::isTypeAUnit(object->type)) {
            units.push_back((CUnit*)object);
        }
    }
    void removeObject(CRenderable* object) {
        for ( int i=0; i < objects.size(); i++ ) {
            if ( object == objects[i] ) {
                INFO(LOG) << "Object removed from chunk number " << chunk_number;
                objects.erase(objects.begin()+i);
            }
        }

        for ( int i=0; i < units.size(); i++ ) {
            /* Remove the unit from the units list as well. */
            if ( object == (CRenderable*)units[i] ) {
                INFO(LOG) << "Object removed was a unit.";
                units.erase(units.begin()+i);
            }
        }
    }
    void setTiles(int x, int y) {
        IUnitFactory* unitGenerator=CLocator::getUnitFactory();
        /* a create chunk function */
        int offsetx=x%Globals::chunk_size;
        int offsety=y%Globals::chunk_size;
        for ( int i = y - offsety; i < y - offsety + Globals::chunk_size; i++ ) {
            for ( int j = x - offsetx; j < x - offsetx + Globals::chunk_size; j++ ) {
                TileInstantiation tile_properties=Globals::getTileAt(2000,2000,j,i);
//        if (tile_properties.object_type)
//          addObject(new CMapObjectRenderable(tile_properties.object_type,j,i,0));
                if ( tile_properties.unit_type ) {
                    CRenderable* unit = NULL;
                    /* cheap way of instantiating units for now, i think we should sent off a CREATE_UNIT message and register them with the unit manager */
                    switch (tile_properties.unit_type) {
                    case UNIT_TYPE_VILLAGER:
                        unit = unitGenerator->createUnit(IUnitFactory::UNIT_VILLAGER,j,i);
                        break;
                    case UNIT_TYPE_KNIGHT:
                        unit = unitGenerator->createUnit(IUnitFactory::UNIT_KNIGHT,j,i);
                        break;
                    case UNIT_TYPE_FIGHTER:
                        unit = unitGenerator->createUnit(IUnitFactory::UNIT_FIGHTER,j,i);
                        break;
                    }

//          if ( unit ) {
//            addObject( unit );
//          }

                }
                this->tiles.push_back(new CTile(tile_properties.tile_number,tile_properties.area_type));
            }
        }
        INFO(LOG) << "Creating chunk " << chunk_number;
        initialized=true;
    }
    bool isInitialized() {
        return initialized;
    }
    void setInitialized() {
        initialized=true;
    }
};

struct WallInformation {
    float height;
    int tile_type;
    WallInformation(float height, int tile_type) {}
};

class CTile;
class IMap : public IEventable {
protected:

public:
    virtual void initializeTileCalls() = 0;
    virtual bool collide(int posx, int posy) = 0;
    virtual CTile* at(int x, int y) = 0;
    virtual CChunk* getChunk(int x, int y) = 0;
    virtual int hasTileWall(int direction, int x, int y) = 0;
    virtual void renderRoof(int tile) = 0;
    virtual void renderWall(int direction, int tile) = 0;
    virtual void initialize(int chunk_number) = 0;
    virtual void renderChunk(float posx, float posy, float pos_frac_x, float pos_frac_y, int render_tiles_view) = 0;
    virtual void renderTiles(float time) = 0;
};

#endif