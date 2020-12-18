#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include "constants.h"
#include "enum.h"
#include "IMapGenerator.h"
#include "struct.h"
#include "zekin/noise.h"
#include <string>
using namespace Globals;

class CMapGenerator : public IMapGenerator {
protected:
    int width;
    int height;
public:
    CMapGenerator() : width(1), height(1) {}
    /* */
    virtual int getWidth() { return width; }
    virtual int getHeight() { return height; }
    virtual TileInstantiation getTileAt(float x, float y) { return TileInstantiation(0,0,0);}
    virtual void generateWorldImage() {
        SDL_Surface * surf = SDL_CreateRGBSurface(0,width,height,32,0xFF,0xFF00,0xFF0000,0);
        for (int i=0; i<width; i++) {
            for (int j=0; j<height; j++) {
                TileInstantiation tile=getTileAt(j,i);
                int color=0;
    
                switch (tile.tile_number) {
                case TILE_BEACH_SAND:
                    color=0xB3DFE3;
                    break;
                case TILE_GRASS:
                    color=0x89E0B0;
                    break;
                case TILE_MOUNTAIN:
                    color=0x606060;
                    break;
                case TILE_SAND:
                    color=0x87C1E0;
                    break;
                case TILE_CRACKED_GROUND:
                    color=0x5FABD4;
                    break;
                case TILE_RIGID_MOUNTAIN:
                    color=0x686060;
                    break;
                case TILE_CRACKED_ICE:
                    color=0xE8EBD1;
                    break;
                case TILE_SNOW:
                    color=0xFAEDD9;
                    break;
                case TILE_ICE_MOUNTAIN:
                    color=0x606060;
                    break;
                case TILE_FROZEN_WATER:
                    color=0xB38B89;
                    break;
                case TILE_WATER:
                    color=0xE3716D;
                    break;
                case TILE_CITY_GROUND:
                    color=0x808080;
                    break;
                case TILE_CITY_WALL:
                    color=0x303030;
                    break;
                case TILE_CITY_STONE_WALL:
                    color=0xAAAABB;
                    break;
                }
    
                if (tile.area_type==AREA_FOREST) {
                    color=0x599080;
                }
                if (tile.object_type==MAP_OBJECT_TREE ||
                        tile.object_type==MAP_OBJECT_EVERGREEN_L ||
                        tile.object_type==MAP_OBJECT_EVERGREEN_S ||
                        tile.object_type==MAP_OBJECT_CACTUS_L ||
                        tile.object_type==MAP_OBJECT_CACTUS_S) {
                    color=0x297060;
                }
                ((unsigned int*)surf->pixels)[i*height+j]=color;
            }
        }
        SDL_SaveBMP_RW(surf, SDL_RWFromFile("WorldMap_Uncompressed.bmp", "wb"), 1) ;
    }
};
 
class CWorldMapGenerator : public CMapGenerator {
public:
    CWorldMapGenerator() {width=2000; height=2000;}

    virtual TileInstantiation getTileAt(float x, float y) 
    {
        int elevation=Globals::getTileHeight(2000,2000,x,y);
        int color=0;
        double temperature=(fbmXY(x*0.004+28000,y*0.004+40000,0.1,3)+1)*125.0/2.0;
        int temperature_category=getTemperature(temperature);
        double precipitation=getPrecipitation(x,y);
        TileInstantiation tile_being_built(0,0,0);
        switch(elevation) {
        case ELEVATION_SEA:
            switch(temperature_category) {
            case TEMP_COLD:
                tile_being_built.tile_number=TILE_FROZEN_WATER;
                tile_being_built.area_type=AREA_SNOWLANDS;
                break;
            case TEMP_WARM:
                tile_being_built.tile_number=TILE_WATER;
                tile_being_built.area_type=AREA_GRASSLANDS;
                break;
            case TEMP_HOT:
                tile_being_built.tile_number=TILE_WATER;
                tile_being_built.area_type=AREA_GRASSLANDS;
                break;
            }
            break;
        case ELEVATION_ABOVE_SEA:
            switch(temperature_category) {
            case TEMP_COLD:
                tile_being_built.tile_number=TILE_CRACKED_ICE;
                tile_being_built.area_type=AREA_SNOWLANDS;
                break;
            case TEMP_WARM:
                tile_being_built.tile_number=TILE_BEACH_SAND;
                tile_being_built.area_type=AREA_GRASSLANDS;
                break;
            case TEMP_HOT:
                tile_being_built.tile_number=TILE_SAND;
                tile_being_built.area_type=AREA_DESERT;
                break;
            }
            break;
        case ELEVATION_LAND:
            switch(temperature_category) {
            case TEMP_COLD:
                tile_being_built.tile_number=TILE_SNOW;
                tile_being_built.area_type=AREA_SNOWLANDS;
                break;
            case TEMP_WARM:
                tile_being_built.tile_number=TILE_GRASS;
                tile_being_built.area_type=AREA_GRASSLANDS;
                break;
            case TEMP_HOT:
                tile_being_built.tile_number=TILE_CRACKED_GROUND;
                tile_being_built.area_type=AREA_DESERT;
                break; //HOT DESERT SAND
            }
            break;
        case ELEVATION_HIGH_MOUNTAIN:
            switch(temperature_category) {
            case TEMP_COLD:
                tile_being_built.tile_number=TILE_ICE_MOUNTAIN;
                tile_being_built.area_type=AREA_SNOWLANDS;
                break; //ICE MOUNTAIN / TUNDRA
            case TEMP_WARM:
                tile_being_built.tile_number=TILE_MOUNTAIN;
                tile_being_built.area_type=AREA_GRASSLANDS;
                break; //REGULAR MOUNTAIN
            case TEMP_HOT:
                tile_being_built.tile_number=TILE_RIGID_MOUNTAIN;
                tile_being_built.area_type=AREA_DESERT;
                break; //RUGGED CLIFFY MOUNTAIN
            }
            break;

        default:
            tile_being_built.tile_number=TILE_WATER;
            tile_being_built.area_type=AREA_GRASSLANDS;
            break;
        }
        if (precipitation == PRECIPITATION_WET && tile_being_built.tile_number==TILE_GRASS) {
            double chance = (generateNoiseXY((int)x,(int)y)+1)/2.0;
            tile_being_built.area_type=AREA_FOREST;
            if (chance > 0.95) {
                if (chance > 0.99) {
                    tile_being_built.object_type=MAP_OBJECT_TREE;
                } else if (chance > 0.97) {
                    tile_being_built.object_type=MAP_OBJECT_EVERGREEN_S;
                } else if (chance > 0.95) {
                    tile_being_built.object_type=MAP_OBJECT_EVERGREEN_L;
                }
            }
        }
        if (precipitation == PRECIPITATION_DRY && tile_being_built.tile_number==TILE_CRACKED_GROUND) {
            double chance = (generateNoiseXY((int)x,(int)y)+1)/2.0;
            tile_being_built.area_type=AREA_DESERT;
            if (chance > 0.97) {
                if (chance > 0.99) {
                    tile_being_built.object_type=MAP_OBJECT_CACTUS_S;
                } else if (chance > 0.98) {
                    tile_being_built.object_type=MAP_OBJECT_CACTUS_L;
                } else if (chance > 0.97) {
                    tile_being_built.object_type=MAP_OBJECT_ROCK;
                }
            }
        }

        /* now check for hardcoded static objects */ //Building the first town (Valleyholme?)
        fillRectWith(&tile_being_built,TILE_CITY_GROUND,AREA_CITY,596,1414,710,1533,x,y);

        buildTownWalls(&tile_being_built,TILE_CITY_WALL,596,1414,710,1533,x,y);
        fillRectWith(&tile_being_built,TILE_CITY_STONE_WALL,AREA_CITY,594,1470,712,1472,x,y);
        fillRectWith(&tile_being_built,TILE_CITY_STONE_WALL,AREA_CITY,620,1350,623,1535,x,y);
        fillRectWith(&tile_being_built,TILE_CITY_STONE_WALL,AREA_CITY,655,1410,658,1535,x,y);


        for (int i=0; i< 10; i++)
            buildHouse(&tile_being_built,600+i*10,1430,600+i*10+8,1440,x,y);   


        if (tile_being_built.area_type==AREA_CITY && tile_being_built.tile_number != TILE_CITY_WALL) {
            double chance = (generateNoiseXY((int)x,(int)y)+1)/2.0;
            if (chance > 0.998) {
                tile_being_built.unit_type=UNIT_TYPE_KNIGHT;
            } else if (chance > 0.995) {
                tile_being_built.unit_type=UNIT_TYPE_FIGHTER;
            } else if (chance > 0.99) {
                tile_being_built.unit_type=UNIT_TYPE_VILLAGER;
            }
        }
        return tile_being_built;
    }
};

class CDebugMapGenerator : public CMapGenerator {
private:
    std::string path;
public:
   CDebugMapGenerator(std::string path) {
      this->path = path; 
      DEBUG(LOG) << "In debug mode, grabbing path " << path;
      width=100;
      height=100;
   } 
   virtual TileInstantiation getTileAt(float x, float y) {
//        int elevation=Globals::getTileHeight(2000,2000,x,y);
//        int color=0;
//        double temperature=(fbmXY(x*0.004+28000,y*0.004+40000,0.1,3)+1)*125.0/2.0;
//        int temperature_category=getTemperature(temperature);
//        double precipitation=getPrecipitation(x,y);
        TileInstantiation tile_being_built(0,0,0);
        double chance = generateNoiseXY(x,y);
        
        tile_being_built.area_type=AREA_GRASSLANDS;
        if (chance > 0.95) {
            tile_being_built.tile_number=TILE_BEACH_SAND;
        } else {
            tile_being_built.tile_number=TILE_GRASS;
        }
        
        if ((int)x % ((int)width-5) < 5 || (int)y % ((int)height-5) < 5) {
            tile_being_built.tile_number=TILE_WATER;
        }
      
       return tile_being_built;
   }
};

#endif