#ifndef CCAMERA_H
#define CCAMERA_H
#include "CEventManager.h"
#include "enum.h"
#include <SDL/SDL_opengl.h>
class IRenderable {
public:
  IRenderable() : x(0), y(0), z(0) {}
  IRenderable(float x, float y, float z) : x(x), y(y), z(z) { }
  float x;
  float y;
  float z;
};
class CCameraTarget : public IRenderable {
public:
  CCameraTarget() {} 
  CCameraTarget(float x, float y, float z) : IRenderable(x,y,z) {}
};
class CCamera : public IEventable {
private:
  IRenderable* camera_target;
  CCameraTarget camera_target_default;
//  int directions[4];
  bool moving[4];
public:
  CCamera() : camera_target(0) { 
    for (int i=0; i<4; i++) { moving[i]=0; }
    camera_target_default=CCameraTarget(0,0,0);
    CEventManager::getInstance()->subscribe(0,this);
  }
  void setTarget(IRenderable* target) {
  }
  void setTarget(int x, int y, int z) {
    /* TODO: do map bounds checking i guess */
    std::clog << "Setting camera to (x" << x << ",y" << y << ",z" << z << ")" << std::endl;
    camera_target_default.x=x;
    camera_target_default.y=y;
    camera_target_default.z=z;
  }
  void updateTarget() {
    if (moving[DIRECTION_NORTH])
      camera_target_default.y+=5.0/25.0;
    if (moving[DIRECTION_EAST])
      camera_target_default.x+=5.0/25.0;
    if (moving[DIRECTION_SOUTH])
      camera_target_default.y-=5.0/25.0;
    if (moving[DIRECTION_WEST])
      camera_target_default.x-=5.0/25.0;
  }
  XY getXY() {
    return XY((double)camera_target_default.x, (double)camera_target_default.y);
  }
  void setupCamera() {
    glLoadIdentity();
//    glRotatef(-45,0,1,1);
    glRotatef(-45,1,0,0);
    glTranslatef(0,0,-10);
//    glTranslatef(-camera_target_default.x,-camera_target_default.y,-camera_target_default.z);
  }
  virtual void notify(Event* e) {
    if (e->type==EVENT_CAMERA_TARGET) {
      if (e->a==0) {
        this->setTarget(e->x,e->y,e->z);
        camera_target=&camera_target_default;
      } else {
        std::clog << "Recieved camera target at (x,y,z)" << std::endl;
        camera_target=(IRenderable*)(e->a);
      }
    } else if (e->type==EVENT_CAMERA_MOVE_START) {
      if (0 <= e->a && e->a <= 3) {
        moving[e->a]=true;
      } else {
        std::cerr << "Error: Incorrect camera start move direction received" << std::endl;
      }
    } else if (e->type==EVENT_CAMERA_MOVE_END) {
      if (0 <= e->a && e->a <= 3) {
        moving[e->a]=false;
      } else {
        std::cerr << "Error: Incorrect camera end move direction received" << std::endl;
      }
    } else if (e->type==EVENT_RENDER_FRAME) {
      updateTarget();
    }
  }
  static CCamera* getInstance() {
    static CCamera* instance=0;
    if (instance==0) {
      instance = new CCamera();
    }
    return instance;
  }
};
#endif