#ifndef CPLAYER_H
#define CPLAYER_H
#include <map>
#include "CEventManager.h"
#include "IResource.h"
//struct Stats {
//  int hp;
//  int mana;
//  int stam;
//  int armor;
//  int dmg_min;
//  int dmg_max;
//  int power;
//  float dr;
//  float dodge;
//  float hit;
//  float regen;
//  float spd;
//
//};;
class CAnimationSheet : public CSpriteSheet {

private:
    Animation Idle[4];
    Animation Walk[4];
    Animation Attack[4];
    Animation Use[4];
public:
    void setAnimation(int AnimationType, int Direction, std::vector<int> frame_order, float time) {
        switch(AnimationType) {
        case ANIM_IDLE:
        case ANIM_WALK:
        case ANIM_ATTACK:
        case ANIM_CAST:
        case ANIM_USE:
        default:
            std::clog << "Error: No animation type exists for this case." << std::endl;
        };
    }

};
//class CAnimationManager {
//
//
//};
//class CMapTarget {
//  CSpriteSheet* sheet;
//  CMapTarget() {
//  }
//  virtual void setAnimation(int animationType, int direction) {
//  }
//
//};
//class CItem : public CMapTarget {
//  CItem() {
//  }
//};
//class CLiving : public CMapTarget, public IEventable {
//  Stats stats;
//  CLiving(float x, float y, float w, float h) {
//
//  }
//};
//class CPlayer : public CUnit {
//  CPlayer() {
//
//  }
//
//};

#endif
