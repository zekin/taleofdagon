#ifndef CGUI_H
#define CGUI_H

#include "CEventManager.h"
#include "IResource.h"
#include "enum.h"
#include "CMap.h"
#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>

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
class CLabel : public IWidget {
  CSpriteSheet image;
public:
  CLabel(float x, float y, float width, float height, std::string path) : IWidget(x,y,width,height), image(path) {
  }
  virtual void render() {
    renderQuad(image.getTextureID(0));
  }
  virtual void notify(Event* e) {
  }
  
};
class CButtonStartGame : public IWidget {
  bool activated;
  CButtonStartGame(float x, float y, float width, float height) : IWidget(x,y,width,height), activated(false) { 
  }
  virtual void renderActive() {
  }
  virtual void renderInactive() {
  }
  virtual void render() {
    if (activated) {
      renderActive();
    } else {
      renderInactive();
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
  void add(IWidget* member) {
  }
  virtual void render() {
    for (int i=0; i<widgets.size(); i++) {
      widgets.at(i)->render();
    }
  }
};

class CGUIMainMenu : public CGUI {
private:
  CSpriteSheet tex;
  CMap map;
  CLabel lbl;
public:
  CGUIMainMenu() : CGUI(0.2,0.2,0.60,0.80), tex("./graphics/ui.png"), lbl(0.0,0.05,1.0,0.14128,"./graphics/logo.png"), map(100,100) {
    CEventManager::getInstance()->subscribe(0,this);
    widgets.push_back(&lbl);
//    this->add(CButton());
  }
  virtual void notify(Event* e) {
    static SDL_Surface* screen=0;
    if (e->type==EVENT_GAME_STARTED) {
      screen=(SDL_Surface*)e->a;
      glClearColor(0.0,0.0,0.0,0.0);
    }
    if (e->type==EVENT_CAMERA_MOVE_START) {
      if (e->a==DIRECTION_NORTH) {
      }
      if (e->a==DIRECTION_SOUTH) {
      }
    }
    if (e->type==EVENT_RENDER_FRAME) {
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      float extra_sides=((float)screen->w/(float)screen->h - 1.0)/2.0;
      glOrtho(-extra_sides,1.0+extra_sides,1.0,0.0,0,-1);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glEnable(GL_BLEND);    
      glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
      glBlendEquation(GL_FUNC_ADD);
      
//      lbl.render();
      renderQuad(tex.getTextureID(0));
      
      render();
      glDisable(GL_BLEND);
      
      
    }
  }
};

#endif


