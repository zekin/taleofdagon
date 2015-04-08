#ifndef ENUM_H
#define ENUM_H
#include <iostream>
#include <string>
#include "math.h"
#include "zekin/noise.h"
#include <SDL/SDL.h>
#include "logging.h"
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
enum {
    DIRECTION_NORTH=0,
    DIRECTION_EAST=1,
    DIRECTION_SOUTH=2,
    DIRECTION_WEST=3
};
enum {
    MUSIC_INTRO=1000,
    MUSIC_TOWN,
    MUSIC_WANDERING,
    MUSIC_DESERT,
    MUSIC_FOREST,
    MUSIC_COMBAT1,
    MUSIC_COMBAT2,
    MUSIC_COMBAT3,
    MUSIC_FINALBATTLE,
    MUSIC_CREDITS
};
enum {
    SOUND_UI_CLICK=1,
    SOUND_UI_SELECT=2
};

enum {
    EVENT_GAME_STARTED=11000,
    EVENT_GAME_ENDED,
    EVENT_BUTTON_CLICKED,
    EVENT_RENDER_FRAME,
    EVENT_KEYPRESS_DOWN,
    EVENT_KEYPRESS_UP,
    EVENT_SWITCH_STATES,
    EVENT_PLAY_MUSIC,
    EVENT_PLAY_SOUND,
    EVENT_CAMERA_TARGET,
    EVENT_CAMERA_MOVE_START,
    EVENT_CAMERA_MOVE_END,
    EVENT_COMMAND_USE_START,
    EVENT_COMMAND_USE_END,
    EVENT_COMMAND_CANCEL,
    EVENT_COMMAND_MAP,
    EVENT_COMMAND_CHARACTER_SCREEN,
    EVENT_ADD_CONSOLE,
    EVENT_SET_SPEED,
    EVENT_WINDOW_BUTTON_CLICKED,
    EVENT_BUTTON_PRESSED_DOWN,
    EVENT_BUTTON_PRESSED_RELEASE,
    EVENT_BUTTON_SELECTED,
    EVENT_COMBAT_STARTED,
    EVENT_COMBAT_ENDED,
    EVENT_UNIT_CREATED,
    EVENT_UNIT_ATTACKS_DIRECTION,  //Unit Pointer, AttackNumber {1,2,3,4}, Direction
    EVENT_UNIT_USES_DIRECTION,     //Unit Pointer, Item Pointer, Direction
    EVENT_UNIT_USES_ON_SELF,       //Unit Pointer, Item Pointer
    EVENT_UNIT_CASTS_DIRECTION,    //Unit Pointer, Spell, Direction
    EVENT_UNIT_CASTS_ON_SELF,      //Unit Pointer, Spell
    EVENT_UNIT_HIT_UNIT_WITH,      //
    EVENT_PROJECTILE_HITS_UNIT,    //
    EVENT_UNIT_LEVELS_UP,         //Skill UP
    EVENT_BUTTON_INCREASE_SKILL, //skill number
    EVENT_BUTTON_INCREASE_STAT,  //stat number
    EVENT_BUTTON_DECREASE_SKILL, //skill number
    EVENT_BUTTON_DECREASE_STAT,   //stat number
    EVENT_INITIALIZE,
    EVENT_CLEANUP,
    EVENT_SPAWN_DIALOG  //DIALOG_NUMBER

};


enum {
    STATE_INTRO=1,
    STATE_MENU,
    STATE_CHARACTER_SELECT,
    STATE_GAME
};
enum CLASSIFIER {
    SEA,
    GROUND,
    CLIFF
};

enum {
    SKILL_AXES,
    SKILL_DODGE,
    SKILL_HEALTH,
    SKILL_HIDE,
    SKILL_MACES,
    SKILL_MAGERY,
    SKILL_MEDITATE,
    SKILL_SWORDS
};
enum {
    STAT_STR,
    STAT_DEX,
    STAT_INT
};
enum RESOURCE_TYPE {
    RESOURCE_SPRITESHEET,
    RESOURCE_MAPSHEET,
    RESOURCE_MUSIC,
    RESOURCE_SOUND
};

enum {
    PORTRAIT_DAGON,
    PORTRAIT_WHISKERS,
    PORTRAIT_TOWN_FEMALE1,
    PORTRAIT_TOWN_FEMALE2,
    PORTRAIT_TOWN_MALE1,
    PORTRAIT_TOWN_MALE2,
    PORTRAIT_TOWN_SHOPKEEPER1,
    PORTRAIT_TOWN_SHOPKEEPER2,
};
enum {
    DIALOG_1_START_DAGON,
    DIALOG_2_START_WHISKERS,
    DIALOG_3_START_DAGON,
    DIALOG_4_START_WHISKERS,
    DIALOG_5_START_DAGON,
    /* planned to have a lot more dialogue in the game, based on events triggering such as a unit death for the first time
     * or numerous other occassions
     */
    DIALOG_6_END_DAGON,
    DIALOG_7_END_KILLER,
    DIALOG_8_END_CAT,
    DIALOG_9_END_DAGON,
    DIALOG_10_END_CAT,
    DIALOG_11_END_KILLER
};
static char* dialogue[] = {
    "~6Dagon looks down at his starving cat. ~0\"Whiskers! What are you doing over there? We have work to do.\"",
    "~0\"Mrrrowl!\"",
    "~0\"We have to work together! We need to find fire! Big swords, things that go boom! Your claws! ANYTHING to get this guy!\" ~6Dagon flails his arms excitedly and makes explosion sounds. ",
    "~0\"Mrrrrrooowll..?\" ~6The cat looks up at Dagon pleadingly.",
    "~0\"Oh okay fine, I guess we can try to find a meal too. We can try that city Valleyholme just north of here.\""
};

enum {
    TEMP_COLD,
    TEMP_WARM,
    TEMP_HOT
};
enum {
    ELEVATION_SEA,
    ELEVATION_ABOVE_SEA,
    ELEVATION_LAND,
    ELEVATION_HIGH_MOUNTAIN
};

enum {
//  TILE_GROUND_START=0,
    TILE_BEACH_SAND=0,
    TILE_GRASS,
    TILE_MOUNTAIN,
    TILE_SAND=3,
    TILE_CRACKED_GROUND,
    TILE_RIGID_MOUNTAIN,
    TILE_CRACKED_ICE=6,
    TILE_SNOW,
    TILE_ICE_MOUNTAIN,
    TILE_FROZEN_WATER=9,
    TILE_WATER,
    TILE_CITY_GROUND=15,
    TILE_CITY_WALL,
    TILE_CITY_FOOTHOLD,
    TILE_CITY_HOUSE_WALL=18,
    TILE_CITY_HOUSE_WINDOW,
    TILE_CITY_HOUSE_WALL2,
    TILE_CITY_HOUSE_WALL3=21,
    TILE_CITY_HOUSE_FLOOR,
    TILE_CITY_CONCRETE,
    TILE_CITY_STONE_WALL=24,
    TILE_CITY_MARBLE_FLOOR,
    TILE_CITY_HOUSE_WOOD,
    TILE_CITY_HOUSE_WOOD2=27

};

enum {
    MAP_OBJECT_CACTUS_S=1,
    MAP_OBJECT_CACTUS_L=2,
    MAP_OBJECT_EVERGREEN_L=4,
    MAP_OBJECT_EVERGREEN_S=5,
    MAP_OBJECT_TREE=6
};
enum {
    UNIT_TYPE_VILLAGER=100,
    UNIT_TYPE_FIGHTER,
    UNIT_TYPE_KNIGHT
};

enum {
    PRECIPITATION_DRY,
    PRECIPITATION_WET
};
enum {
    AREA_GRASSLANDS,
    AREA_FOREST,
    AREA_DESERT,
    AREA_SNOWLANDS,
    AREA_CITY
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
                    break; //FROZEN WATER CHUNKS
                case TEMP_WARM:
                    color=0xE3716D;
                    break;
                case TEMP_HOT:
                    color=0xE3716D;
                    break; //WATER IS WATER MAN, HOT WATER DOESNT CHANGE IT
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

void buildTownWalls(TileInstantiation* tile_being_built, int x1, int y1, int x2, int y2, int posx, int posy) {
    if (!tile_being_built) {
        INFO(LOG) << "Tile being built called with no tile in BuildTownWalls ? ";
        return;
    }
    //there's a better way to check if a value is within the rectangle edges or not, but my brain isnt working right now
    if ( (x1 <= posx && posx <= x2 && posy==y1) ||
            (x1 <= posx && posx <= x2 && posy==y2) ||
            (y1 <= posy && posy <= y2 && posx==x1) ||
            (y1 <= posy && posy <= y2 && posx==x2) ) {
        tile_being_built->tile_number=TILE_CITY_WALL;

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

    /* now check for hardcoded static objects */ //Building the first town (Valleyholme?)
    fillRectWith(&tile_being_built,TILE_CITY_GROUND,AREA_CITY,596,1414,710,1533,x,y);
    buildTownWalls(&tile_being_built,596,1414,710,1533,x,y);
    fillRectWith(&tile_being_built,TILE_CITY_STONE_WALL,AREA_CITY,594,1470,712,1472,x,y);
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
                color=0x706060;
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
