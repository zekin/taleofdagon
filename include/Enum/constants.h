#ifndef CONSTANTS_H
#define CONSTANTS_H
enum {
    GAMEFLAGS_FULLSCREEN=0x1,
    GAMEFLAGS_DEBUG=0x2
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
enum UI_TYPE {
    UI_TYPE_LABEL,
    UI_TYPE_BUTTON,
    UI_TYPE_TEXT
};
enum {
    ANIM_IDLE,
    ANIM_WALK,
    ANIM_ATTACK,
    ANIM_CAST,
    ANIM_USE
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
    MAP_OBJECT_CACTUS_S=7,
    MAP_OBJECT_CACTUS_L=1,
    MAP_OBJECT_ROCK=2,
    MAP_OBJECT_TABLE=3,
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

enum {
    SKIN_WHITE,
    SKIN_BLACK,
    SKIN_YELLOW,
    SKIN_RED,
    SKIN_BROWN
};

enum {
    AI_PLAYER,
    AI_STAND_AROUND,
    AI_WALKING,
    AI_ATTACK
};

enum {
    ARMOR_CLOTHES,
    ARMOR_SCALEMAIL,
    ARMOR_CHAINMAIL,
    ARMOR_PLATEMAIL
};

#endif