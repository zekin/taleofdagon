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
  char num;
  char area;
  CTile(char tile_number, char area=AREA_GRASSLANDS) : num(tile_number), area(area) {}
  CLASSIFIER getClassifier() {
    switch(num) {
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
          
          if ( unit ) {
            addObject( unit );
          }

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

class IMap : public IEventable {
protected:
  int width;
  int height;
  bool initialized_tile_data;
  int chunks_wide;
  int chunks_high;
  std::vector<CChunk*> chunks;
  unsigned int texture_tiles[20];
  CMapSheet* map_tiles_resource;
  CSpriteSheet* map_walls_resource;
  SDL_Surface* map_tiles_surface; 
public:
  IMap(int width, int height) :
  width(width), 
  height(height), 
  map_tiles_surface(0), 
  map_tiles_resource(0), 
  initialized_tile_data(false), 
  chunks_wide(0), 
  chunks_high(0) {
    /* load image for tiles */
    if (width%Globals::chunk_size>0) {
      this->width=(width/Globals::chunk_size)*Globals::chunk_size; // or modulus
      INFO(LOG) << "Invalid width specified, not a multiple of chunksize(" << Globals::chunk_size << "), reverting to floor of nearest:" << width;
    } 
    if (height%Globals::chunk_size>0) {
      this->height=(height/Globals::chunk_size)*Globals::chunk_size;
      INFO(LOG) << "Invalid height specified, not a multiple of chunksize(" << Globals::chunk_size << "), reverting to floor of nearest:" << height;
    }
    chunks_wide=width/Globals::chunk_size;
    chunks_high=height/Globals::chunk_size;
    map_tiles_resource=CResourceManager::getInstance()->getMapSheet("./graphics/tile3.png",16,16);
    map_walls_resource=CResourceManager::getInstance()->getSpriteSheet("./graphics/tile3.png",16,16);   
    if (Globals::tile_call_initialized==false) {
      initializeTileCalls();
    }
  }
  void initializeTileCalls() {
    for (int i=0; i<29; i++) {
      Globals::tile_call.push_back(glGenLists(1));
      glNewList(Globals::tile_call[i],GL_COMPILE);
      glBindTexture(GL_TEXTURE_2D, map_tiles_resource->getTextureID(i));
      glBegin(GL_QUADS);
      if (i==TILE_ICE_MOUNTAIN || i==TILE_RIGID_MOUNTAIN || i==TILE_MOUNTAIN) {
        glTexCoord2d(0.0,1.0);
        glVertex3d(-1,-1,0);
        glTexCoord2d(0.0,0.0);
        glVertex3d(-1,1,0);
        glTexCoord2d(1.0,0.0);
        glVertex3d(1,1,0);
        glTexCoord2d(1.0,1.0);
        glVertex3d(1,-1,0);
      } else {
        glTexCoord2d(0.0,1.0);
        glVertex3d(-1,-1,0.1);
        glTexCoord2d(0.0,0.0);
        glVertex3d(-1,1,0.1);
        glTexCoord2d(1.0,0.0);
        glVertex3d(1,1,0.1);
        glTexCoord2d(1.0,1.0);
        glVertex3d(1,-1,0.1);
        glEnd();
     }
      glEnd();
      glEndList();
    }
     
    Globals::tile_call_initialized=true;
  }
  virtual bool collide(int posx, int posy)=0;
};
#endif