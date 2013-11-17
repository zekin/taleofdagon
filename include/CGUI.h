#ifndef CGUI_H
#define CGUI_H

#include "CEventManager.h"
#include "IResource.h"
#include "enum.h"
#include <SDL/SDL_opengl.h>
class IWidget : public IEventable {
protected:
  float x;
  float y;
  float w;
  float h;
public:
  IWidget() { }
  IWidget(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) { }
  virtual void render()=0;
  void renderQuad(int texture) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0,0.0); glVertex3f(x,y,0);
    glTexCoord2f(1.0,0.0); glVertex3f(x+w,y,0);
    glTexCoord2f(1.0,1.0); glVertex3f(x+w,y+h,0);
    glTexCoord2f(0.0,1.0); glVertex3f(x,y+h,0);
    glEnd();
  }
};

class CButton : public IWidget {
private:  
public:
  CButton() {
  }
  virtual void notify(Event* e) {
    if (e->type == EVENT_KEYPRESS_DOWN) {
        CEventManager::getInstance()->notify(Event());
    }
  }
};

class CGUI : public IWidget {
protected:
  std::vector<IWidget*> widgets;
public:
  CGUI(float x, float y, float gui_width, float gui_height) : IWidget(x,y,gui_width,gui_height)  { }
  void notify_members(Event* e) {
    for (int i=0;i<widgets.size(); i++) {
      std::clog << "Notifying member " << std::endl;
    }
  }
};
class CGUIMainMenu : public CGUI {
private:
  CSpriteSheet tex;
public:
  CGUIMainMenu() : CGUI(0.35,0.3,0.3,0.5), tex("./graphics/ui.png",300,500) {
    CEventManager::getInstance()->subscribe(0,this);
  }
  virtual void render() { }
  virtual void notify(Event* e) {
    if (e->type==EVENT_CAMERA_MOVE_START) {
      if (e->a==DIRECTION_NORTH) {
      }
      if (e->a==DIRECTION_SOUTH) {
      }
    }
    if (e->type==EVENT_RENDER_FRAME) {
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(0.0,1.0,1.0,0.0,0,-1);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      this->renderQuad(tex.getTextureID(0));
    }
  }
};

#endif


