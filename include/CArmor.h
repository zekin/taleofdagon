#ifndef CARMOR_H
#define CARMOR_H

#include "constants.h"
#include "struct.h"
#include "IResource.h"

class Armor {
public:
    int armor_type;
    int armor_value;
    XYZ color;
    CSpriteSheet* sheet;
    Armor(XYZ color) : color(color), sheet(0), armor_type(0), armor_value(0) {}
};

class Clothes : public Armor {
public:
    Clothes(XYZ color) : Armor(color) {
        sheet=CResourceManager::getInstance()->getSpriteSheet("./graphics/sprites/walk_clothes.png",32,32);
        armor_type=ARMOR_CLOTHES;
        armor_value=1;
    }
};

class ScaleMail : public Armor {
public:
    ScaleMail() : Armor(XYZ(0.5,0.4,0.6)) {
        sheet=CResourceManager::getInstance()->getSpriteSheet("./graphics/sprites/walk_armor.png",32,32);
        armor_type=ARMOR_SCALEMAIL;
        armor_value=5;
    }
};

class ChainMail : public Armor {
public:
    ChainMail() : Armor(XYZ(0.8,0.8,0.9)) {
        sheet=CResourceManager::getInstance()->getSpriteSheet("./graphics/sprites/walk_armor.png",32,32);
        armor_type=ARMOR_SCALEMAIL;
        armor_value=10;
    }
};

class PlateMail : public Armor {
public:
    PlateMail() : Armor(XYZ(0.9,0.9,0.5)) {
        sheet=CResourceManager::getInstance()->getSpriteSheet("./graphics/sprites/walk_armor.png",32,32);
        armor_type=ARMOR_PLATEMAIL;
        armor_value=20;
    }
};



#endif