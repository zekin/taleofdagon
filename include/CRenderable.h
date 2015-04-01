#ifndef CRENDERABLE_H
#define CRENDERABLE_H
#include "enum.h"
#include "CClock.h"
#include "CEventManager.h"
#include "IResource.h"

class CMap;
enum {
  RENDERABLE_MAP_OBJECT,
  RENDERABLE_UNIT
};
class CRenderable {
protected:
  CSpriteSheet* sheet; 
public:
  int type;
  CRenderable() : x(0), y(0), z(0), type(0), sheet(0) {}
  CRenderable(float x, float y, float z) : x(x), y(y), z(z), type(0), sheet(0) {}
  /* don't ask about the choice of the interface */
  virtual void render(float pos_x, float pos_y, float pos_frac_x, float pos_frac_y, int render_tiles_view) {}
  float x;
  float y;
  float z;
};


class CIntroCamera : public CRenderable {
private:
  CClock* clock;
public:
  CIntroCamera() : CRenderable(800,1480,0) {
    clock=CClock::getInstance();
  }
  static CIntroCamera* getInstance() {
    static CIntroCamera* instance=0;
    if (instance==0) {
      instance = new CIntroCamera();
    }
    return instance;
  }
  
  void update() {
    static double time_last=0;
    
//    std::clog << CClock::getInstance()->getTime()-time_last;
//    time_last=CClock::getInstance()->getTime();
    this->y-=clock->deltaT()/2.0;//(CClock::getInstance()->getTime()-time_last)/2.0;
    
//    time_last=CClock::getInstance()->getTime();
  }
};
#endif