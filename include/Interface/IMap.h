#ifndef IMAP_H
#define IMAP_H

#include "enum.h"
#include "CEventManager.h"
#include "CRenderable.h"
#include "CUnit.h"
#include "IChunk.h"
#include "IUnitFactory.h"
#include "ITileFactory.h"
#include "CLocator.h"
#include "CTile.h"
#include "IMapGenerator.h"
#include "constants.h"

class CUnit;

class CChunk : public IChunk {
private:
    std::vector<CTile*> tiles;
    std::vector<CRenderable*> objects;
    std::vector<IUnit*> units;

    bool initialized;
    int chunk_number;
    
public:
    CChunk(int chunk_number) :
        initialized(false),
        chunk_number(chunk_number) {}

    virtual CTile* at(int x, int y) {
        if (!initialized) {
            setTiles(x,y);
        }
        x=x%Globals::chunk_size;
        y=y%Globals::chunk_size;
        return tiles[y*Globals::chunk_size+x];
    }
    
    virtual CRenderable* getObject(int index) {
        if ( index >= objects.size() ) {
            return NULL;
        } else {
            return objects.at(index);
        }
    }
    
    virtual IUnit* getUnit(int index) {
        if ( index >= units.size() ) {
            return NULL;
        } else {
            return units.at(index);
        }       
    }
    
    virtual void addObject(CRenderable* object) {
        for (int i=0; i<objects.size(); i++) {
            if ( object == objects[i] ) {
                INFO(LOG) << "Object already registered inside chunk number " << chunk_number;
                return;
            }
        }
        
        INFO(LOG) << "Object added to chunk number " << chunk_number;
        objects.push_back(object);
        
        INFO(LOG) << "obect type" << object->type;
        if ( object->isAUnit() ) {
            INFO(LOG) << "Unit added to chunk";
            units.push_back((IUnit*)object);
        }
    }
    
    virtual void removeObject(CRenderable* object) {
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
    
    virtual void setTiles(int x, int y) {
        IUnitFactory* unitGenerator = CLocator::getUnitFactory();
        ITileFactory* tileGenerator = CLocator::getTileFactory();
        IMapGenerator* mapGenerator = CLocator::getMapGenerator();
        
        /* a create chunk function */
        int offsetx=x%Globals::chunk_size;
        int offsety=y%Globals::chunk_size;
        
        for ( int i = y - offsety; i < y - offsety + Globals::chunk_size; i++ ) {
            for ( int j = x - offsetx; j < x - offsetx + Globals::chunk_size; j++ ) {
                TileInstantiation tile_properties=mapGenerator->getTileAt(j,i);
                
                if ( tile_properties.unit_type ) {
                    CRenderable* unit = NULL;
                    /* cheap way of instantiating units for now, i think we should sent off a CREATE_UNIT message and register them with the unit manager */
                    unit = unitGenerator->createUnit(tile_properties.unit_type,j,i);
                }
                
                if ( tile_properties.object_type ) {
                    CMapObjectRenderable* mapObject = new CMapObjectRenderable( tile_properties.object_type, j, i, 0 );
                    
                    addObject( (CRenderable*) mapObject );
                }
                CTile* tile = tileGenerator->createTile( tile_properties.tile_number, tile_properties.area_type );
                
                tiles.push_back( tile );
            }
        }
        INFO(LOG) << "Creating chunk " << chunk_number;
        initialized=true;
    }
    
    virtual bool isInitialized() {
        return initialized;
    }
    
    virtual void setInitialized() {
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
public:
    virtual bool isInitialized() = 0;
    virtual bool collide(IUnit* srcUnit, int posx, int posy) = 0;
    virtual CTile* at(int x, int y) = 0;
    virtual CChunk* getChunk(int x, int y) = 0;
    virtual bool hasTileWall(int direction, int x, int y) = 0;
    virtual void renderRoof(int tile) = 0;
    virtual void renderWall(int direction, int tile) = 0;
    virtual void initialize(int chunk_number) = 0;
    virtual void renderChunk(float posx, float posy, float pos_frac_x, float pos_frac_y, int render_tiles_view) = 0;
    virtual void renderTiles(float time) = 0;
};

#endif