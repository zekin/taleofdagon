#ifndef UNIT_H
#define UNIT_H

#include "CEventManager.h"
#include "IResource.h"
#include "CRenderable.h"
#include "CCamera.h"
#include "stdlib.h"
#include <iostream>
#include "logging.h"
#include "IMap.h"
#include "IUnit.h"
#include "CLocator.h"
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


class CUnit : public IUnit {
protected:
    CSpriteSheet* hair;
    Armor* armor;
    CSpriteSheet* shadow;
    XYZ skin_color;
    XYZ hair_color;
    FrameList* idle[4];
    FrameList* walk[4];
    FrameList* attack[4];
    FrameList* current_animation; //currently playing animation frame list
    CTimer ai_timer;


    virtual void init() {
        CResourceManager * resources = CResourceManager::getInstance();
        sheet=resources->getSpriteSheet("./graphics/sprites/walk_base.png",32,32);
        hair=resources->getSpriteSheet("./graphics/sprites/walk_hair.png",32,32);
        shadow=resources->getSpriteSheet("./graphics/sprites/walk_shadow.png");
        /* get list of human animations */
        idle[DIRECTION_NORTH]=resources->getFrameList("IDLE_NORTH");
        idle[DIRECTION_EAST]=resources->getFrameList("IDLE_EAST");
        idle[DIRECTION_SOUTH]=resources->getFrameList("IDLE_SOUTH");
        idle[DIRECTION_WEST]=resources->getFrameList("IDLE_WEST");

        walk[DIRECTION_NORTH]=resources->getFrameList("WALK_NORTH");
        walk[DIRECTION_EAST]=resources->getFrameList("WALK_EAST");
        walk[DIRECTION_SOUTH]=resources->getFrameList("WALK_SOUTH");
        walk[DIRECTION_WEST]=resources->getFrameList("WALK_WEST");

        attack[DIRECTION_NORTH]=resources->getFrameList("ATTACK_NORTH");
        attack[DIRECTION_EAST]=resources->getFrameList( "ATTACK_EAST");
        attack[DIRECTION_SOUTH]=resources->getFrameList("ATTACK_SOUTH");
        attack[DIRECTION_WEST]=resources->getFrameList( "ATTACK_WEST");
        current_animation=walk[DIRECTION_NORTH];
    }
public:
    //bad stuff we're doing because we didn't design this part correctly at the last minute
    int ai_state;
    float speed;
    int direction;
    virtual void move() {
        IMap* map = CLocator::getMap();
        CClock* clock = CClock::getInstance();

        float desiredMoveY=0;
        float desiredMoveX=0;
        /* this is a bad move function, this needs a LOT of work, but since units and movement were done completely on monday its ok */
        switch(direction) {
        case DIRECTION_NORTH:
            current_animation=walk[DIRECTION_NORTH];
            desiredMoveX = this->x;
            desiredMoveY = this->y - clock->deltaT() * speed;
            break;
        case DIRECTION_EAST:
            current_animation=walk[DIRECTION_EAST];
            desiredMoveX = this->x + clock->deltaT() * speed;
            desiredMoveY = this->y;
            break;
        case DIRECTION_SOUTH:
            desiredMoveX = this->x + clock->deltaT() * speed;
            current_animation=walk[DIRECTION_SOUTH];
            desiredMoveX = this->x;
            desiredMoveY = this->y + clock->deltaT() * speed;
            break;
        case DIRECTION_WEST:
            current_animation=walk[DIRECTION_WEST];
            desiredMoveX = this->x - clock->deltaT() * speed;
            desiredMoveY = this->y;
            break;
        }

        if (map->collide(this, round(desiredMoveX),round(desiredMoveY)) == false) {
            this->x = desiredMoveX;
            this->y = desiredMoveY;
        }
    }

    virtual void AI() {
        if (ai_timer.finished()==0) {
            return;
        } else {
            if (ai_state==AI_WALKING) {
                direction=rand() % 4;
                current_animation=walk[direction];
            }
            if (ai_state==AI_STAND_AROUND) {
                current_animation=idle[direction];
            }
        }
    }

    virtual bool isMoving() {
        if (ai_state == AI_WALKING) 
            return true;
        
        return false;
    }
    CUnit(float x, float y, float z) : IUnit(x,y,z),
        //create redder tones for skin
//    skin_color(0.5+(float)rand()/(RAND_MAX), 0.5+(float)rand()/(RAND_MAX), 0.5+(float)rand()/(RAND_MAX)),
        skin_color(0,0,0),
        //create random hair colors
        hair_color((float)rand()/(RAND_MAX), (float)rand()/(RAND_MAX), (float)rand()/(RAND_MAX)),
        armor(0),
        ai_timer(5.0,true),
        ai_state(AI_WALKING),
        speed(0.2),
        direction(DIRECTION_NORTH) {
        type=RENDERABLE_UNIT;
        int skin_type=rand()%5;
        if (skin_type==SKIN_WHITE) {
            skin_color.x=0.9+((float)rand()/RAND_MAX)*0.10;
            skin_color.y=0.85+((float)rand()/RAND_MAX)*0.15;
            skin_color.z=0.82+((float)rand()/RAND_MAX)*0.18;
        } else if (skin_type==SKIN_RED) {
            skin_color.x=0.9+((float)rand()/RAND_MAX)*0.10;
            skin_color.y=0.65+((float)rand()/RAND_MAX)*0.20;
            skin_color.z=0.65+((float)rand()/RAND_MAX)*0.20;
        } else if (skin_type==SKIN_YELLOW) {
            skin_color.x=0.9+((float)rand()/RAND_MAX)*0.10;
            skin_color.y=0.9+((float)rand()/RAND_MAX)*0.10;
            skin_color.z=0.7+((float)rand()/RAND_MAX)*0.20;
        } else if (skin_type==SKIN_BLACK) {
            skin_color.x=0.5+((float)rand()/RAND_MAX)*0.10;
            skin_color.y=0.5+((float)rand()/RAND_MAX)*0.10;
            skin_color.z=0.5+((float)rand()/RAND_MAX)*0.10;
        } else if (skin_type==SKIN_BROWN) {
            skin_color.x=0.7+((float)rand()/RAND_MAX)*0.10;
            skin_color.y=0.7+((float)rand()/RAND_MAX)*0.10;
            skin_color.z=0.7+((float)rand()/RAND_MAX)*0.10;
        } else {
            INFO(LOG) << "Wrong skin type detected.";
        }
        init();
    }

    CUnit(float x, float y, float z, XYZ skin_color, XYZ hair_color) :
        IUnit(x,y,z),
        skin_color(skin_color),
        hair_color(hair_color),
        armor(0),
        ai_timer(5.0,true),
        ai_state(AI_WALKING),
        speed(0.2),
        direction(DIRECTION_NORTH) {
        type=RENDERABLE_UNIT;
        //we know what colors we want here for our constructor, not generated by the map i suppose
        init();
    }


    virtual void render(float pos_x, float pos_y, float pos_frac_x, float pos_frac_y, int render_tiles_view) {
        float offset_x=x-pos_x+pos_frac_x+render_tiles_view;
        float offset_y=pos_y-y+pos_frac_y+render_tiles_view;
        glEnable(GL_DEPTH_TEST);
        glPushMatrix();
        glTranslatef(offset_x,offset_y,0);
        if (shadow) {
            glBindTexture(GL_TEXTURE_2D, shadow->getTextureID(0));
            glBegin(GL_QUADS);
            glTexCoord2f(0,1);
            glVertex3f(-0.5,-0.5,0); //glVertex3f(-1.0,-1.0,0);
            glTexCoord2f(0,0);
            glVertex3f(-0.5,0.5,0.0); //glVertex3f(1.0,-1.0,0);
            glTexCoord2f(1,0);
            glVertex3f(0.5,0.5,0.0); //glVertex3f(1.0,1.0,0);
            glTexCoord2f(1,1);
            glVertex3f(0.5,-0.5,0.0); //glVertex3f(-1.0,1.0,0);
            glEnd();
        }

        glRotatef(-45,1,0,0);


        XYZ day_color=CCamera::getInstance()->timeOfDay(true);
        glColor3f(skin_color.x*day_color.x,skin_color.y*day_color.y,skin_color.z*day_color.z);


        glBindTexture(GL_TEXTURE_2D, sheet->getTextureID(current_animation->getFrame(CClock::getInstance()->getTime())));
        glBegin(GL_QUADS);
        glTexCoord2f(0,1);
        glVertex3f(-1.0,0.0,0); //glVertex3f(-1.0,-1.0,0);
        glTexCoord2f(0,0);
        glVertex3f(-1.0,0,2.0); //glVertex3f(1.0,-1.0,0);
        glTexCoord2f(1,0);
        glVertex3f(1.0,0,2.0); //glVertex3f(1.0,1.0,0);
        glTexCoord2f(1,1);
        glVertex3f(1.0,0,0.0); //glVertex3f(-1.0,1.0,0);
        glEnd();

        if (armor) {
            if (armor->sheet) {
                glColor3f(armor->color.x*day_color.x,armor->color.y*day_color.y,armor->color.z*day_color.z);
                glBindTexture(GL_TEXTURE_2D, armor->sheet->getTextureID(current_animation->getFrame(CClock::getInstance()->getTime())));
                glBegin(GL_QUADS);
                glTexCoord2f(0,1);
                glVertex3f(-1.0,0.0,0); //glVertex3f(-1.0,-1.0,0);
                glTexCoord2f(0,0);
                glVertex3f(-1.0,0,2.0); //glVertex3f(1.0,-1.0,0);
                glTexCoord2f(1,0);
                glVertex3f(1.0,0,2.0); //glVertex3f(1.0,1.0,0);
                glTexCoord2f(1,1);
                glVertex3f(1.0,0,0.0); //glVertex3f(-1.0,1.0,0);
                glEnd();
            }
        }

        glColor3f(hair_color.x*day_color.x,hair_color.y*day_color.y,hair_color.z*day_color.z);
        glBindTexture(GL_TEXTURE_2D, hair->getTextureID(current_animation->getFrame(CClock::getInstance()->getTime())));
        glBegin(GL_QUADS);
        glTexCoord2f(0,1);
        glVertex3f(-1.0,0.0,0); //glVertex3f(-1.0,-1.0,0);
        glTexCoord2f(0,0);
        glVertex3f(-1.0,0,2.0); //glVertex3f(1.0,-1.0,0);
        glTexCoord2f(1,0);
        glVertex3f(1.0,0,2.0); //glVertex3f(1.0,1.0,0);
        glTexCoord2f(1,1);
        glVertex3f(1.0,0,0.0); //glVertex3f(-1.0,1.0,0);
        glEnd();
        glRotatef(45,1,0,0);
        glPopMatrix();
        glDisable(GL_DEPTH_TEST);
        glColor3f(day_color.x,day_color.y,day_color.z);
    }
};


class CPlayerRenderable : public CUnit {
public:
    enum {
        ABILITY_SPAWN_DEBUG
    };
    virtual void AI() {
        if (ai_state==AI_STAND_AROUND) {
            current_animation=idle[direction];
        }
        
        if (ai_state==AI_ATTACK) {
            ability(ABILITY_SPAWN_DEBUG);
            current_animation=attack[direction];
        }
    }
    
    virtual void ability(int ability) {
        IUnitFactory* factory = CLocator::getUnitFactory();
        IMap* map = CLocator::getMap();
        
        int spawnLocationX = x;
        int spawnLocationY = y;
        const int spawnRadius = 3;
        
        switch ( direction ) {
            case DIRECTION_SOUTH:
                spawnLocationY+=spawnRadius;
                break;
            case DIRECTION_NORTH:
                spawnLocationY-=spawnRadius;
                break;
            case DIRECTION_EAST:
                spawnLocationX+=spawnRadius;
                break;
            case DIRECTION_WEST:
                spawnLocationX-=spawnRadius;
                break;
        }
        
        switch ( ability ) {
            case ABILITY_SPAWN_DEBUG:
                if ( map->collide(NULL, spawnLocationX, spawnLocationY) == false ) {
                    factory->createUnit(IUnitFactory::UNIT_FIGHTER, spawnLocationX, spawnLocationY);
                }
                
                ai_state=AI_STAND_AROUND;
                break;
        }
    }

    CPlayerRenderable(float x, float y, float z) : CUnit(x,y,z,XYZ(1.0,1.0,1.0),XYZ(1.0,1.0,0.3)) {
        speed=4.0;
        armor=new Clothes(XYZ(0.8,0.6,0.6));
        ai_state=AI_STAND_AROUND;
        direction=DIRECTION_SOUTH;
        
    }
    static CPlayerRenderable* getInstance() {
        static CPlayerRenderable* instance=0;
        if (instance==0) {
            instance=new CPlayerRenderable(685,1550,0);
        }
        return instance;
    }
};

class CMapObjectRenderable : public CRenderable {
private:
    int obj_number;
public:
    CMapObjectRenderable(int type, float x, float y, float z) : CRenderable(x,y,z), obj_number(type) {
        sheet=CResourceManager::getInstance()->getSpriteSheet("./graphics/map_object.png",32,32,false);
    }

    virtual void render(float pos_x, float pos_y, float pos_frac_x, float pos_frac_y, int render_tiles_view) {
        float offset_x=x-pos_x+pos_frac_x+render_tiles_view;
        float offset_y=pos_y-y+pos_frac_y+render_tiles_view;
        glEnable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, sheet->getTextureID(obj_number));
        glPushMatrix();
        glTranslatef(offset_x,offset_y,0);
        glRotatef(-45,1,0,0);
        glBegin(GL_QUADS);
        glTexCoord2f(0,1);
        glVertex3f(-1.0,0.0,0); //glVertex3f(-1.0,-1.0,0);
        glTexCoord2f(0,0);
        glVertex3f(-1.0,0,2.0); //glVertex3f(1.0,-1.0,0);
        glTexCoord2f(1,0);
        glVertex3f(1.0,0,2.0); //glVertex3f(1.0,1.0,0);
        glTexCoord2f(1,1);
        glVertex3f(1.0,0,0.0); //glVertex3f(-1.0,1.0,0);
        glEnd();
        glRotatef(45,1,0,0);
        glPopMatrix();
        glDisable(GL_DEPTH_TEST);
    }
};


#endif
