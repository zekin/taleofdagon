#ifndef CGUI_H
#define CGUI_H

#include "CEventManager.h"
#include "IResource.h"
#include "enum.h"
#include "CMap.h"
#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>

enum UI_TYPE {
  UI_TYPE_LABEL,
  UI_TYPE_BUTTON
};
class IWidget : public IEventable {
protected:
  IWidget* parent;
  CSpriteSheet* panel_texture;
  float x;
  float y;
  float w;
  float h;
  UI_TYPE uitype;
  void setUIType(UI_TYPE type) {
    uitype=type;
  }
  UI_TYPE getUIType() {
    return uitype;
  }
public:
  IWidget(float x, float y, float w, float h) : x(x), y(y), w(w), h(h), parent(0), uitype(UI_TYPE_LABEL) { }
  IWidget(float x, float y, float w, float h, IWidget* parent) : x(x), y(y), w(w), h(h), parent(parent), uitype(UI_TYPE_LABEL) { }
  virtual void render()=0;
  float getX() {
    return x;
  }
  float getY() {
    return y;
  }
  float getW() {
    return w;
  }
  float getH() {
    return h;
  }
  void renderQuad(int texture) {
    /* TODO: Check bounds for 0 to 1 */
    if (parent) {
      float real_x=parent->getX()+x*parent->getW();
      float real_y=parent->getY()+y*parent->getH();
      float real_w=parent->getW()*w;
      float real_h=parent->getH()*h;
      glBindTexture(GL_TEXTURE_2D, texture);
      glBegin(GL_QUADS);
      glTexCoord2f(0.0,0.0); glVertex3f(real_x,real_y,0);
      glTexCoord2f(1.0,0.0); glVertex3f(real_x+real_w,real_y,0);
      glTexCoord2f(1.0,1.0); glVertex3f(real_x+real_w,real_y+real_h,0);
      glTexCoord2f(0.0,1.0); glVertex3f(real_x,real_y+real_h,0);
      glEnd();
      return;
    } else { 
      glBindTexture(GL_TEXTURE_2D, texture);
      glBegin(GL_QUADS);
      glTexCoord2f(0.0,0.0); glVertex3f(x,y,0);
      glTexCoord2f(1.0,0.0); glVertex3f(x+w,y,0);
      glTexCoord2f(1.0,1.0); glVertex3f(x+w,y+h,0);
      glTexCoord2f(0.0,1.0); glVertex3f(x,y+h,0);
      glEnd();
      return;
    }
  }
};
class CLabel : public IWidget {
public:
  CLabel(float x, float y, float width, float height, std::string path) : IWidget(x,y,width,height) {
    panel_texture=CResourceManager::getInstance()->getSpriteSheet(path);
  }
  CLabel(float x, float y, float width, float height, std::string path, IWidget* parent) : IWidget(x,y,width,height, parent) {
    panel_texture=CResourceManager::getInstance()->getSpriteSheet(path);
    std::clog << "Created texture, going ahead. " << std::endl;
  }
  virtual void render() {
    renderQuad(panel_texture->getTextureID(0));
  }
  virtual void notify(Event* e) {
  }
};
class CButton : public IWidget {
private:
  bool selected;
  bool pressed;
protected:
  enum BUTTON_ENUMS {
    TEXTURE_BUTTON_NOT_SELECTED=0,
    TEXTURE_BUTTON_SELECTED,
    TEXTURE_BUTTON_PRESSED
  };
public:
  CButton(float x, float y, float width, float height, IWidget* parent) : IWidget(x,y,width,height,parent), pressed(false), selected(false) {
    panel_texture=CResourceManager::getInstance()->getSpriteSheet("./graphics/ui/ui_default.png",64,16);
    std::clog << "Created button" << std::endl;
    if (panel_texture==NULL)
      std::cerr << "Error: Could not retrieve default button image, will not be able to render button" << std::endl;
    setUIType(UI_TYPE_BUTTON);
  }
  virtual void render() {
    if (panel_texture==NULL) 
      return;
    if (selected)
      renderQuad(panel_texture->getTextureID(TEXTURE_BUTTON_SELECTED));     
    else if (pressed)
      renderQuad(panel_texture->getTextureID(TEXTURE_BUTTON_PRESSED));
    else 
      renderQuad(panel_texture->getTextureID(TEXTURE_BUTTON_NOT_SELECTED));
  }
  virtual void press()=0; 
  virtual void notify(Event* e) {
    if (e->type==EVENT_BUTTON_PRESSED_DOWN) {
      pressed=true;
    }
    else if (e->type==EVENT_BUTTON_PRESSED_RELEASE) {
      press();
      pressed=false;
    }
    else if (e->type==EVENT_BUTTON_SELECTED) {
      if ((void*)e->a==this) {
        CEventManager::getInstance()->notify(Event(EVENT_PLAY_SOUND,0));
        selected=true;
      }
      else {
        selected=false;
      }
    }
  }
};
class CButtonStartGame : public CButton {
public:
  CButtonStartGame(float x, float y, float w, float h, IWidget* parent) : CButton(x,y,w,h,parent) {
    panel_texture=CResourceManager::getInstance()->getSpriteSheet("./graphics/ui/ui_mainmenu_start.png", 64, 16);
  }
  virtual void press() {
    CEventManager::getInstance()->notify(Event(EVENT_GAME_STARTED));
  }
};
class CButtonOptions : public CButton {
public:
  CButtonOptions(float x, float y, float w, float h, IWidget* parent) : CButton(x,y,w,h,parent) {
    panel_texture=CResourceManager::getInstance()->getSpriteSheet("./graphics/ui/ui_mainmenu_options.png", 64, 16);
  }
  virtual void press() {
    CEventManager::getInstance()->notify(Event(EVENT_GAME_STARTED));
  }
};
class CButtonExitGame : public CButton {
public:
  CButtonExitGame(float x, float y, float w, float h, IWidget* parent) : CButton(x,y,w,h,parent) {
    panel_texture=CResourceManager::getInstance()->getSpriteSheet("./graphics/ui/ui_mainmenu_exit.png", 64, 16);
  }
  virtual void press() {
    CEventManager::getInstance()->notify(Event(EVENT_GAME_STARTED));
  }
};
class CGUI : public IWidget {
protected:
  std::vector<IWidget*> widgets;
  int selected;
public:
  CGUI(float x, float y, float gui_width, float gui_height) : IWidget(x,y,gui_width,gui_height) { 
  }
  void notify_members(Event e) {
    for (int i=0;i<widgets.size(); i++) {
      widgets[i]->notify(&e);
    }
  }
  IWidget* getPrevButton() {
    int i=(selected-1)%widgets.size();
    
    if (i<0) {
      i=widgets.size()-i;
    }
    for (i; i<widgets.size(); i++) {
    }
  }
  IWidget* getNextButton() {
    for (int i=(selected+1)%widgets.size(); i<widgets.size(); i++) {
    }
  }
  void add(IWidget* member) {
  }
  virtual void render() {
    renderQuad(panel_texture->getTextureID(0));
//    glTranslatef(0,0,0.1);
//    glDisable(GL_BLEND);
//    glEnable(GL_DEPTH_TEST);
    for (int i=0; i<widgets.size(); i++) {
      widgets.at(i)->render();
    }
  }
};

class CGUIMainMenu : public CGUI {
private:
  CLabel logo;
  CButtonStartGame btn_start_game;
  CButtonOptions btn_options; 
  CButtonExitGame btn_exit_game;
  CMap map;
public:
  CGUIMainMenu() : 
    CGUI(0.2,0.2,0.60,0.80), 
    logo(0.0,0.05,1.0,0.14128,"./graphics/logo.png"),
    btn_start_game(0.30,0.25,0.4,0.1,this),
    btn_options(0.30,0.40,0.4,0.1,this),
    btn_exit_game(0.30,0.55,0.4,0.1,this),
    map(50,50) {
    CEventManager::getInstance()->subscribe(0,this);
    panel_texture=CResourceManager::getInstance()->getSpriteSheet("./graphics/ui.png");
    widgets.push_back(&btn_start_game);
    widgets.push_back(&btn_options);
    widgets.push_back(&btn_exit_game);
    notify_members(Event(EVENT_BUTTON_SELECTED,(long)&btn_start_game));
  }
  virtual void notify(Event* e) {
    static SDL_Surface* screen=0;
    if (e->type==EVENT_GAME_STARTED) {
      screen=(SDL_Surface*)e->a;
    }
    if (e->type==EVENT_CAMERA_MOVE_START) {
      if (e->a==DIRECTION_NORTH) {
        notify_members(Event(EVENT_BUTTON_SELECTED,0));
      }
      if (e->a==DIRECTION_SOUTH) {
      }
    }
    if (e->type==EVENT_RENDER_FRAME) {
      CCamera::getInstance()->startGUI();     
      logo.render();
      render();
      CCamera::getInstance()->endGUI();
    }
  }
};

#endif


