#ifndef UNIT_H
#define UNIT_H

#include "CEventManager.h"
#include "IResource.h"
#include "CRenderable.h"
#include "CCamera.h"
#include "stdlib.h"
#include <iostream>
#include "logging.h"
enum {
  ARMOR_CLOTHES,
  ARMOR_SCALEMAIL,
  ARMOR_CHAINMAIL,
  ARMOR_PLATEMAIL
};
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
  AI_WALKING
};
class CUnitRenderable : public CRenderable {
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
  
  void init() {
    sheet=CResourceManager::getInstance()->getSpriteSheet("./graphics/sprites/walk_base.png",32,32);
    hair=CResourceManager::getInstance()->getSpriteSheet("./graphics/sprites/walk_hair.png",32,32);
    shadow=CResourceManager::getInstance()->getSpriteSheet("./graphics/sprites/walk_shadow.png");   
    /* get list of human animations */
    idle[DIRECTION_NORTH]=CResourceManager::getInstance()->getFrameList("IDLE_NORTH");
    idle[DIRECTION_EAST]=CResourceManager::getInstance()->getFrameList("IDLE_EAST");   
    idle[DIRECTION_SOUTH]=CResourceManager::getInstance()->getFrameList("IDLE_SOUTH");
    idle[DIRECTION_WEST]=CResourceManager::getInstance()->getFrameList("IDLE_WEST");   
    
    walk[DIRECTION_NORTH]=CResourceManager::getInstance()->getFrameList("WALK_NORTH");
    walk[DIRECTION_EAST]=CResourceManager::getInstance()->getFrameList("WALK_EAST");   
    walk[DIRECTION_SOUTH]=CResourceManager::getInstance()->getFrameList("WALK_SOUTH");
    walk[DIRECTION_WEST]=CResourceManager::getInstance()->getFrameList("WALK_WEST");   
    
    attack[DIRECTION_NORTH]=CResourceManager::getInstance()->getFrameList("ATTACK_NORTH");
    attack[DIRECTION_EAST]=CResourceManager::getInstance()->getFrameList( "ATTACK_EAST");   
    attack[DIRECTION_SOUTH]=CResourceManager::getInstance()->getFrameList("ATTACK_SOUTH");
    attack[DIRECTION_WEST]=CResourceManager::getInstance()->getFrameList( "ATTACK_WEST");      
    current_animation=walk[DIRECTION_NORTH];
  }
public: 
  //bad stuff we're doing because we didn't design this part correctly at the last minute
  int ai_state;
  float speed;
  int direction;
  void Move() {
	  /* this is a bad move function, this needs a LOT of work, but since units and movement were done completely on monday its ok */
    switch(direction) {
      case DIRECTION_NORTH:
        current_animation=walk[DIRECTION_NORTH];
        this->y-=CClock::getInstance()->deltaT()*speed;
        break;
      case DIRECTION_EAST:
        current_animation=walk[DIRECTION_EAST];       
        this->x+=CClock::getInstance()->deltaT()*speed;
        break;
      case DIRECTION_SOUTH:
        current_animation=walk[DIRECTION_SOUTH];              
        this->y+=CClock::getInstance()->deltaT()*speed;
        break;
      case DIRECTION_WEST:
        current_animation=walk[DIRECTION_WEST];                     
        this->x-=CClock::getInstance()->deltaT()*speed;
        break;
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
  CUnitRenderable(float x, float y, float z) : CRenderable(x,y,z),
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
  CUnitRenderable(float x, float y, float z, XYZ skin_color, XYZ hair_color) : 
  CRenderable(x,y,z), 
  skin_color(skin_color), 
  hair_color(hair_color), 
  armor(0),
  ai_timer(5.0,true),
  ai_state(AI_WALKING),
  speed(0.2),
  direction(DIRECTION_NORTH) 
  { 
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
      glTexCoord2f(0,1); glVertex3f(-0.5,-0.5,0); //glVertex3f(-1.0,-1.0,0);
      glTexCoord2f(0,0); glVertex3f(-0.5,0.5,0.0); //glVertex3f(1.0,-1.0,0);
      glTexCoord2f(1,0); glVertex3f(0.5,0.5,0.0); //glVertex3f(1.0,1.0,0);
      glTexCoord2f(1,1); glVertex3f(0.5,-0.5,0.0); //glVertex3f(-1.0,1.0,0);
      glEnd();
    }
    
    glRotatef(-45,1,0,0);
    
   
    XYZ day_color=CCamera::getInstance()->timeOfDay(true);
    glColor3f(skin_color.x*day_color.x,skin_color.y*day_color.y,skin_color.z*day_color.z);
    
  
    glBindTexture(GL_TEXTURE_2D, sheet->getTextureID(current_animation->getFrame(CClock::getInstance()->getTime())));
    glBegin(GL_QUADS);
    glTexCoord2f(0,1); glVertex3f(-1.0,0.0,0); //glVertex3f(-1.0,-1.0,0);
    glTexCoord2f(0,0); glVertex3f(-1.0,0,2.0); //glVertex3f(1.0,-1.0,0);
    glTexCoord2f(1,0); glVertex3f(1.0,0,2.0); //glVertex3f(1.0,1.0,0);
    glTexCoord2f(1,1); glVertex3f(1.0,0,0.0); //glVertex3f(-1.0,1.0,0);
    glEnd();
    
    if (armor) {
      if (armor->sheet) {
        glColor3f(armor->color.x*day_color.x,armor->color.y*day_color.y,armor->color.z*day_color.z); 
        glBindTexture(GL_TEXTURE_2D, armor->sheet->getTextureID(current_animation->getFrame(CClock::getInstance()->getTime())));
        glBegin(GL_QUADS);
        glTexCoord2f(0,1); glVertex3f(-1.0,0.0,0); //glVertex3f(-1.0,-1.0,0);
        glTexCoord2f(0,0); glVertex3f(-1.0,0,2.0); //glVertex3f(1.0,-1.0,0);
        glTexCoord2f(1,0); glVertex3f(1.0,0,2.0); //glVertex3f(1.0,1.0,0);
        glTexCoord2f(1,1); glVertex3f(1.0,0,0.0); //glVertex3f(-1.0,1.0,0);
        glEnd();
      }
    }
    
    glColor3f(hair_color.x*day_color.x,hair_color.y*day_color.y,hair_color.z*day_color.z);
    glBindTexture(GL_TEXTURE_2D, hair->getTextureID(current_animation->getFrame(CClock::getInstance()->getTime())));
    glBegin(GL_QUADS);
    glTexCoord2f(0,1); glVertex3f(-1.0,0.0,0); //glVertex3f(-1.0,-1.0,0);
    glTexCoord2f(0,0); glVertex3f(-1.0,0,2.0); //glVertex3f(1.0,-1.0,0);
    glTexCoord2f(1,0); glVertex3f(1.0,0,2.0); //glVertex3f(1.0,1.0,0);
    glTexCoord2f(1,1); glVertex3f(1.0,0,0.0); //glVertex3f(-1.0,1.0,0);
    glEnd();
    glRotatef(45,1,0,0);      
    glPopMatrix();
    glDisable(GL_DEPTH_TEST);
    glColor3f(day_color.x,day_color.y,day_color.z);
  }
};
class CUnitVillager : public CUnitRenderable {
public:
  CUnitVillager(float x, float y, float z) : CUnitRenderable(x,y,z) {
    speed=0.8;
    armor=new Clothes(XYZ((float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX));
  }
};
class CUnitFighter : public CUnitRenderable {
public:
  CUnitFighter(float x, float y, float z) : CUnitRenderable(x,y,z) {
    speed=0.6;
    armor=new ScaleMail();
  } 
};
class CUnitKnight : public CUnitRenderable {
public:
  CUnitKnight(float x, float y, float z) : CUnitRenderable(x,y,z) {
    speed=0.4;
    armor=new ChainMail();
  }  
};
class CPlayerRenderable : public CUnitRenderable {
public:
   virtual void AI() {
      if (ai_state==AI_STAND_AROUND) {
        current_animation=idle[direction];
      }
     
  }
  
  CPlayerRenderable(float x, float y, float z) : CUnitRenderable(x,y,z,XYZ(1.0,1.0,1.0),XYZ(1.0,1.0,0.3)) {
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
    glTexCoord2f(0,1); glVertex3f(-1.0,0.0,0); //glVertex3f(-1.0,-1.0,0);
    glTexCoord2f(0,0); glVertex3f(-1.0,0,2.0); //glVertex3f(1.0,-1.0,0);
    glTexCoord2f(1,0); glVertex3f(1.0,0,2.0); //glVertex3f(1.0,1.0,0);
    glTexCoord2f(1,1); glVertex3f(1.0,0,0.0); //glVertex3f(-1.0,1.0,0);
    glEnd();
    glRotatef(45,1,0,0);      
    glPopMatrix();
    glDisable(GL_DEPTH_TEST);
  }
};

/* this will be for later, projectiles and whatnot, possibly for units and friction */
/*class CMoving : public CRenderable {
  
  CMoving(float x, float y, float z, XY accel, XY accel_max) {
  }
  void applyForce(XY force) {
    
  }
  void update() {
    
  }
};*/

class CUnitManager : public IEventable {
  std::vector<CUnitRenderable*> unit_list;
  CUnitManager() {}
    
public:
  virtual void notify(Event* e) {
    if (e->type==EVENT_UNIT_CREATED) {
      unit_list.push_back((CUnitRenderable*)e->a);
    }
  }
  
};
#endif
