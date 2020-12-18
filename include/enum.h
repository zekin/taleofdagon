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
#include "struct.h"

#ifdef _MSC_VER
#include <float.h>  // for _isnan() on VC++
#define isnan(x) _isnan(x)  // VC++ uses _isnan() instead of isnan()
//#else
//#include <math.h>  // for isnan() everywhere else
#endif
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

}
#endif
