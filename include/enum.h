#ifndef ENUM_H
#define ENUM_H

#include "Enum/constants.h"
#include <iostream>
#include <string>
#include "math.h"
#include "zekin/noise.h"
#include <SDL/SDL.h>
#include "logging.h"
#include <iostream>
#include <vector>

#ifdef _MSC_VER
#include <float.h>  // for _isnan() on VC++
#define isnan(x) _isnan(x)  // VC++ uses _isnan() instead of isnan()
//#else
//#include <math.h>  // for isnan() everywhere else
#endif
struct XY {
    double x;
    double y;
    XY() : x(0), y(0) {}
    XY(double x,double y) : x(x), y(y) {}
    int dir() {
        if (x >= abs(y)) return DIRECTION_EAST;
        else if (-y >= abs(x)) return DIRECTION_NORTH;
        else if (-x >= abs(y)) return DIRECTION_WEST;
        else if (y >= abs(x)) return DIRECTION_SOUTH;
        else {
            ERROR(LOG) << "Direction at " << x << "," << y << "doesn't meet any criteria";
            return -1;
        }
    }
};
struct XYZ {
    double x;
    double y;
    double z;
    XYZ() : x(0), y(0) {}
    XYZ(double x, double y, double z) : x(x), y(y), z(z) {}

};

struct Rect {
    XY tl,
       tr,
       bl,
       br;
    Rect(XY tl, XY tr, XY bl, XY br) : tl(tl), tr(tr), bl(bl), br(br) { }
};

struct TileInstantiation {
    int tile_number;
    int object_type;
    int unit_type;
    int area_type;
    TileInstantiation(int tile_number, int area_type, int object_type=0, int unit_type=0) :
        tile_number(tile_number),
        object_type(object_type),
        unit_type(unit_type),
        area_type(area_type) {}
};
namespace Globals {
static const int chunk_size=16;
static bool tile_call_initialized=false;
static std::vector<unsigned int> tile_call;
double sphere(double radius, double x, double y) {
    double value=sqrt(radius*radius-(x-radius)*(x-radius)-(y-radius)*(y-radius));
    if (isnan(value))
        value=0;
    return value;
}
/* noise weight : how much more noise is in */
/* sphere weight : how round the island is */
/* initial frequency : */
double generateIslandHeight(double radius, int x, int y, double noise_weight, double sphere_weight, double initial_frequency, double persistance, double passes) {
    double value = ((sphere(radius,x,y)/radius)-0.22)*sphere_weight +
                   ((fbmXY( ((double)x)*initial_frequency, ((double)y)*initial_frequency,persistance,passes)+1)/2)*noise_weight;
    if (isnan(value)) {
        value=0;
    }
    /* generates more interesting and varied inner landscape */
    value-=(sphere(radius*0.45,x-radius*0.85,y-radius/2)/radius)*0.5;

    if (isnan(value)) {
        value=0;
    }

    return value;
}

int getTileHeight(float width, float height, float x, float y) {
    const double radius=(width > height)?height/2.0:width/2.0;
    double height_index=generateIslandHeight(radius,x,y,0.65,0.35,0.032,3.4,5);
    if (0.0 <= height_index && height_index <= 0.48)
        return ELEVATION_SEA;
    else if (0.48 < height_index && height_index <= 0.53)
        return ELEVATION_ABOVE_SEA;
    else if (0.5 < height_index && height_index <= 0.615)
        return ELEVATION_LAND;
    else if (0.615 < height_index && height_index <= 1.00)
        return ELEVATION_HIGH_MOUNTAIN;
    else
        return ELEVATION_HIGH_MOUNTAIN;
}

int getTemperature(double temperature) {
    if (0.0 <= temperature && temperature < 48.0) {
        return TEMP_COLD;
    } else if (48.0 <= temperature && temperature <= 80.0) {
        return TEMP_WARM;
    } else if (80.0 <= temperature && temperature <= 125.0) {
        return TEMP_HOT;
    } else {
        return TEMP_COLD;
    }
}


void generateWorldMapImage() {
    SDL_Surface * temporary_surface = SDL_CreateRGBSurface(0,2000,2000,32,0xFF,0xFF00,0xFF0000,0);
    for (int i=0; i<2000; i++) {
        for (int j=0; j<2000; j++) {
            int height=Globals::getTileHeight(2000,2000,j,i);
            int color=0;
            double temperature=(fbmXY(j*0.004+28000,i*0.004+40000,0.1,3)+1)*125.0/2.0;
            int temperature_category=getTemperature(temperature);

            switch(height) {
            case ELEVATION_SEA:
                switch(temperature_category) {
                case TEMP_COLD:
                    color=0xB38B89;
                    break; // Frozen water chunks
                case TEMP_WARM:
                    color=0xE3716D;
                    break;
                case TEMP_HOT:
                    color=0xE3716D;
                    break; // Hot water ?
                }
                break;
            case ELEVATION_ABOVE_SEA:
                switch(temperature_category) {
                case TEMP_COLD:
                    color=0xE8EBD1;
                    break;
                case TEMP_WARM:
                    color=0xB3DFE3;
                    break;
                case TEMP_HOT:
                    color=0x87C1E0;
                    break;
                }
                break;
            case ELEVATION_LAND:
                switch(temperature_category) {
                case TEMP_COLD:
                    color=0xFAEDD9;
                    break; //COLD
                case TEMP_WARM:
                    color=0x89E0B0;
                    break; //WARM GRASS/TEMPERATE, FOREST?
                case TEMP_HOT:
                    color=0x5FABD4;
                    break; //HOT DESERT SAND
                }
                break;
            case ELEVATION_HIGH_MOUNTAIN:
                switch(temperature_category) {
                case TEMP_COLD:
                    color=0x606060;
                    break; //ICE MOUNTAIN / TUNDRA
                case TEMP_WARM:
                    color=0x606060;
                    break; //REGULAR MOUNTAIN
                case TEMP_HOT:
                    color=0x606060;
                    break; //RUGGED CLIFFY MOUNTAIN
                }
                break;
            default:
                color=0x0;
                break;
            }

            ((unsigned int*)temporary_surface->pixels)[i*2000+j]=color;
        }
    }

    SDL_SaveBMP_RW(temporary_surface, SDL_RWFromFile("WorldMap_Uncompressed.bmp", "wb"), 1) ;
}
int getPrecipitation(int x, int y) {
    double precipitation=(fbmXY(x*0.004+40000,y*0.004+80000,0.1,3)+1)/2.0;
    if (precipitation < 0.55) {
        return PRECIPITATION_DRY;
    } else {
        return PRECIPITATION_WET;
    }
}

void buildTownWalls(TileInstantiation* tile_being_built, int tileType, int x1, int y1, int x2, int y2, int posx, int posy) {
    if (!tile_being_built) {
        INFO(LOG) << "Tile being built called with no tile in BuildTownWalls ? ";
        return;
    }
    //there's a better way to check if a value is within the rectangle edges or not, but my brain isnt working right now
    if ( (x1 <= posx && posx <= x2 && posy==y1) ||
            (x1 <= posx && posx <= x2 && posy==y2) ||
            (y1 <= posy && posy <= y2 && posx==x1) ||
            (y1 <= posy && posy <= y2 && posx==x2) ) {
        tile_being_built->tile_number=tileType;

    }
}


void fillRectWith(TileInstantiation* tile_being_built, int tile_type, int area_type, int x1, int y1, int x2, int y2, int posx, int posy) {
    if (!tile_being_built) {
        INFO(LOG) << "Tile being built called with no tile in FillRectWith ? ";
        return;
    }
    if (x1 <= posx && posx <= x2 && y1 <= posy && posy <= y2) {
        tile_being_built->tile_number=tile_type;
        tile_being_built->area_type=area_type;
    }
}
void fillRectWithObjects(TileInstantiation* tile_being_built, int object_type, int x1, int y1, int x2, int y2, int posx, int posy) {
    if (!tile_being_built) {
        INFO(LOG) << "Tile being built called with no tile in FillRectWith ? ";
        return;
    }
    if (x1 <= posx && posx <= x2 && y1 <= posy && posy <= y2) {
        tile_being_built->object_type=object_type;
    }
}
void buildHouse(TileInstantiation* tile_being_built, int x1, int y1, int x2, int y2, int posx, int posy) {
    if (!tile_being_built) {
        INFO(LOG) << "Tile being built called with no tile in BuildTownWalls ? ";
        return;
    }
    
    int doorLocationY = y2;
    int doorLocationXCenter = x1 + (x2 - x1) / 2;
    int doorLocationX1 = doorLocationXCenter - 1;
    int doorLocationX2 = doorLocationXCenter + 1;
    
    fillRectWith(tile_being_built, TILE_CITY_HOUSE_FLOOR, AREA_CITY, x1, y1, x2, y2, posx, posy);
    //there's a better way to check if a value is within the rectangle edges or not, but my brain isnt working right now
    buildTownWalls(tile_being_built, TILE_CITY_HOUSE_WALL, x1, y1, x2, y2, posx, posy);
    fillRectWith(tile_being_built, TILE_CITY_HOUSE_FLOOR, AREA_CITY, doorLocationX1,doorLocationY,doorLocationX2,doorLocationY, posx, posy);
    fillRectWithObjects(tile_being_built, MAP_OBJECT_TABLE, x1+1, y1+3, x2-1, y1+3, posx, posy);
}

TileInstantiation getTileAt(float width, float height, float x, float y) {
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
void generateWorldImage() {
    SDL_Surface * surf = SDL_CreateRGBSurface(0,2000,2000,32,0xFF,0xFF00,0xFF0000,0);
    for (int i=0; i<2000; i++) {
        for (int j=0; j<2000; j++) {
            TileInstantiation tile=getTileAt(2000,2000,j,i);
//        int tile=getTileAt(2000, 2000, j, i);
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



            ((unsigned int*)surf->pixels)[i*2000+j]=color;
        }
    }

    SDL_SaveBMP_RW(surf, SDL_RWFromFile("WorldMap_Uncompressed.bmp", "wb"), 1) ;
}

}
#endif
