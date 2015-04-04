#ifndef CGUI_H
#define CGUI_H

#include "CEventManager.h"
#include "IResource.h"
#include "enum.h"
#include "CMap.h"
//#include "CGameRules.h"

#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>
#include <sstream>
#include <string.h>
#include "logging.h"


enum UI_TYPE {
    UI_TYPE_LABEL,
    UI_TYPE_BUTTON,
    UI_TYPE_TEXT
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
public:

    void setUIType(UI_TYPE type) {
        uitype=type;
    }
    UI_TYPE getUIType() {
        return uitype;
    }
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
            glTexCoord2f(0.0,0.0);
            glVertex3f(real_x,real_y,0);
            glTexCoord2f(1.0,0.0);
            glVertex3f(real_x+real_w,real_y,0);
            glTexCoord2f(1.0,1.0);
            glVertex3f(real_x+real_w,real_y+real_h,0);
            glTexCoord2f(0.0,1.0);
            glVertex3f(real_x,real_y+real_h,0);
            glEnd();
            return;
        } else {
            glBindTexture(GL_TEXTURE_2D, texture);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0,0.0);
            glVertex3f(x,y,0);
            glTexCoord2f(1.0,0.0);
            glVertex3f(x+w,y,0);
            glTexCoord2f(1.0,1.0);
            glVertex3f(x+w,y+h,0);
            glTexCoord2f(0.0,1.0);
            glVertex3f(x,y+h,0);
            glEnd();
            return;
        }
    }
};
class CLabel : public IWidget {
public:
    CLabel(float x, float y, float width, float height, std::string path, bool linear=false) : IWidget(x,y,width,height) {
        panel_texture=CResourceManager::getInstance()->getSpriteSheet(path,linear);
    }
    CLabel(float x, float y, float width, float height, std::string path, IWidget* parent, bool linear=false) : IWidget(x,y,width,height,parent) {
        panel_texture=CResourceManager::getInstance()->getSpriteSheet(path,linear);
        INFO(LOG) << "Created texture, going ahead. ";
    }
    virtual void render() {
        renderQuad(panel_texture->getTextureID(0));
    }
    void update() {
    }
    virtual void notify(Event* e) {
    }
};
class CText : public IWidget {
private:
    CSpriteSheet* font_texture;
    std::string text;
    float size;
    float x;
    float y;
public:
    CText(float x, float y, float size, std::string text) : IWidget(x,y,1.0,1.0), text(text), size(size), x(x), y(y) {
        setUIType(UI_TYPE_TEXT);
    }
    virtual void render() {
        CFontOutputManager::getInstance()->write_text((char*)text.c_str(),x,y,size);
    }
    virtual void setText(std::string text) {
        this->text=text;
    }
    virtual void setValue(int value) {
        std::stringstream string_stream;
        string_stream << value;
        setText(string_stream.str());
    }
    virtual void setValue(float value) {
    }
    virtual void notify(Event* e) {
    }
};
//class CSubStatPanel : public CLabel {
//private:
//  CText txtHP,
//        txtMana,
//        txtStam,
//        txtDR,
//        txtDodge,
//        txtArmor,
//        txtPower,
//        txtRegen,
//        txtHit,
//        txtSPD,
//        txtDMGBonus,
//        txtWeaponName,
//        txtDMGMin,
//        txtDMGMax;
//public:
//  CSubStatPanel(float x, float y, float w, float h) :
//    txtArmor
//  {
//    panel_texture=CResourceManager::getInstance()->getSpriteSheet("./graphics/ui/ui_characterselect_substats_panel.png", 16, 16);
//  }
//  virtual void render() {
//    renderQuad(panel_texture->getTextureID(0));
//
//  }
//  virtual void notify(Event* e) {
//  }
//
//
//};

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
        INFO(LOG) << "Created button";
        if (panel_texture==NULL)
            ERROR(LOG) << "Error: Could not retrieve default button image, will not be able to render button";
        setUIType(UI_TYPE_BUTTON);
    }
    void stopPressing() {
        pressed=false;
    }
    void setSelected() {
        selected=true;
    }
    virtual void render() {
        if (panel_texture==NULL)
            return;
        if (pressed)
            renderQuad(panel_texture->getTextureID(TEXTURE_BUTTON_PRESSED));
        else if (selected)
            renderQuad(panel_texture->getTextureID(TEXTURE_BUTTON_SELECTED));
        else
            renderQuad(panel_texture->getTextureID(TEXTURE_BUTTON_NOT_SELECTED));
    }
    virtual void press()=0;
    virtual void notify(Event* e) {
        if (e->type==EVENT_BUTTON_PRESSED_DOWN) {
            if (e->a==(long)this)
                pressed=true;
        }
        else if (e->type==EVENT_BUTTON_PRESSED_RELEASE) {
            if (e->a==(long)this && pressed) {
                press();
                pressed=false;
                CEventManager::getInstance()->notify(Event(EVENT_PLAY_SOUND,SOUND_UI_CLICK));
            }
        }
        else if (e->type==EVENT_BUTTON_SELECTED) {
            if ((void*)e->a==this) {
                CEventManager::getInstance()->notify(Event(EVENT_PLAY_SOUND,SOUND_UI_SELECT));
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
        CEventManager::getInstance()->notify(Event(EVENT_SWITCH_STATES,STATE_CHARACTER_SELECT));
    }
};
class CButtonOptions : public CButton {
public:
    CButtonOptions(float x, float y, float w, float h, IWidget* parent) : CButton(x,y,w,h,parent) {
        panel_texture=CResourceManager::getInstance()->getSpriteSheet("./graphics/ui/ui_mainmenu_options.png", 64, 16);
    }
    virtual void press() {
    }
};
class CButtonExitGame : public CButton {
public:
    CButtonExitGame(float x, float y, float w, float h, IWidget* parent) : CButton(x,y,w,h,parent) {
        panel_texture=CResourceManager::getInstance()->getSpriteSheet("./graphics/ui/ui_mainmenu_exit.png", 64, 16);
    }
    virtual void press() {
        SDL_Event quit_event;
        quit_event.type=SDL_QUIT;
        SDL_PushEvent(&quit_event);
    }
};
class CButtonSkillRaise : public CButton {
private:
    int skill_number;
public:
    CButtonSkillRaise(int skill_number, float x, float y, float w, float h, IWidget* parent) : CButton(x,y,w,h,parent), skill_number(skill_number) {
        panel_texture=CResourceManager::getInstance()->getSpriteSheet("./graphics/ui/ui_plus.png", 16, 16);
    }
    virtual void press() {
        Event increase_skill(EVENT_BUTTON_INCREASE_SKILL,skill_number);
        parent->notify(&increase_skill);
    }
};
class CButtonSkillLower : public CButton {
private:
    int skill_number;

public:
    CButtonSkillLower(int skillnumber, float x, float y, float w, float h, IWidget* parent) : CButton(x,y,w,h,parent), skill_number(skillnumber) {
        panel_texture=CResourceManager::getInstance()->getSpriteSheet("./graphics/ui/ui_minus.png", 16, 16);
    }
    virtual void press() {
        Event decrease_skill(EVENT_BUTTON_DECREASE_SKILL,skill_number);
        parent->notify(&decrease_skill);
    }
};
class CButtonStatRaise : public CButton {
private:
    int stat_number;
public:
    CButtonStatRaise(int statnumber, float x, float y, float w, float h, IWidget* parent) : CButton(x,y,w,h,parent), stat_number(statnumber) {
        panel_texture=CResourceManager::getInstance()->getSpriteSheet("./graphics/ui/ui_plus.png", 16, 16);
    }
    virtual void press() {
        Event increase_stat(EVENT_BUTTON_INCREASE_STAT,stat_number);
        parent->notify(&increase_stat);
    }
};

class CButtonStatLower : public CButton {
private:
    int stat_number;
public:
    CButtonStatLower(int stat_number, float x, float y, float w, float h, IWidget* parent) : CButton(x,y,w,h,parent), stat_number(stat_number) {
        panel_texture=CResourceManager::getInstance()->getSpriteSheet("./graphics/ui/ui_minus.png", 16, 16);
    }
    virtual void press() {
        Event decrease_stat(EVENT_BUTTON_DECREASE_STAT,stat_number);
        parent->notify(&decrease_stat);
    }
};
class CButtonFinishCharacter : public CButton {
public:
    CButtonFinishCharacter(float x, float y, float w, float h, IWidget* parent) : CButton(x,y,w,h,parent) {
        panel_texture=CResourceManager::getInstance()->getSpriteSheet("./graphics/ui/ui_mainmenu_start.png", 64, 16);
    }
    virtual void press() {
        CEventManager::getInstance()->notify(Event(EVENT_SWITCH_STATES,STATE_GAME));
    }
};
class CGUI : public IWidget {
protected:
    std::vector<IWidget*> widgets;
    int selected;
public:
    CGUI(float x, float y, float gui_width, float gui_height) : IWidget(x,y,gui_width,gui_height), selected(0) {
        INFO(LOG) << "GUI Created";
    }

    void notifyMembers(Event e) {
        for (int i=0; i<widgets.size(); i++) {
            widgets[i]->notify(&e);
        }
    }
    int getPrevButton() {
        int index=selected;
        for (int i=selected-1; i>=0; i--) {
            if (widgets[i]->getUIType()==UI_TYPE_BUTTON) {
                index=i;
                break;
            }
        }
        return index;
    }
    int getNextButton() {
        int index=selected;
        for (int i=selected+1; i<widgets.size(); i++) {
            if (widgets[i]->getUIType()==UI_TYPE_BUTTON) {
                index=i;
                break;
            }
        }
        return index;
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
class CGameConsole : public IWidget {
protected:
    std::vector<std::string> console_text;
public:
    CGameConsole() : IWidget(0,0,1.0,1.0) {
        console_text.insert(console_text.begin(), "");
        console_text.insert(console_text.begin(), "");
        console_text.insert(console_text.begin(), "");
        console_text.insert(console_text.begin(), "");
        console_text.insert(console_text.begin(), "");
    }
    void newLine(std::string line) {
        console_text.insert(console_text.begin(), line);
        console_text.pop_back();
    }
    virtual void render() {
        CCamera::getInstance()->startGUI();
        for (int i=0; i<console_text.size(); i++) {
            CFontOutputManager::getInstance()->write_text((char*)console_text[i].c_str(), -.15, 0.97-i*0.03,0.015);
        }
        CCamera::getInstance()->endGUI();
    }
    virtual void notify(Event* e) {
        if (e->type==EVENT_RENDER_FRAME) {
            render();
        }
        if (e->type==EVENT_ADD_CONSOLE) {
            newLine(e->string);
        }
    }
};

/* add game ui elements, attach to player*/
class CGameUI : public CGUI {
private:
    CRenderable* player;
public:
    CGameUI(CRenderable* player) : CGUI(0,0,1,1), player(player) {


    }
    virtual void render() {

    }
};
class CGUIWorldMap : public CGUI {
    CRenderable* player;
public:
    CGUIWorldMap(CRenderable* player) : //initialize gui members
        CGUI(0.0,0.0,1.00,1.00),
        player(player) {

        panel_texture=CResourceManager::getInstance()->getSpriteSheet("./graphics/ui/ui_world_map.png", true);
    }

    void setPlayer(CRenderable* renderable) {
        player=renderable;
    }
    virtual void render() {
        const float size=0.0050;
        static char location[32]= {0};

        renderQuad(panel_texture->getTextureID(0));

        for (int i=0; i<widgets.size(); i++) {
            widgets.at(i)->render();
        }
//    glColor3f(0.5,0.4,0.3);
        if (player) {
            glBindTexture(GL_TEXTURE_2D, 0);
            glBegin(GL_QUADS);
            glColor3f(0,0,0);
            glVertex3f(player->x/2000.0-size,player->y/2000.0-size,0);
            glVertex3f(player->x/2000.0+size,player->y/2000.0-size,0);
            glVertex3f(player->x/2000.0+size,player->y/2000.0+size,0);
            glVertex3f(player->x/2000.0-size,player->y/2000.0+size,0);

            glColor3f(0.7,0.7,0.2);
            glVertex3f(-0.01,0  ,0);
            glVertex3f( 1.01,0  ,0);
            glColor3f(0.5,0.4,0.3);
            glVertex3f( 1.01,0.01,0);
            glVertex3f(-0.01,0.01,0);

            glColor3f(0.5,0.4,0.3);
            glVertex3f( 1.00,0.01,0);
            glColor3f(0.7,0.7,0.2);
            glVertex3f( 1.01,0  ,0);
            glVertex3f( 1.01,1.00,0);
            glColor3f(0.5,0.4,0.3);
            glVertex3f( 1.00,0.99,0);

            glColor3f(0.5,0.4,0.3);
            glVertex3f(-0.01,0.99,0);
            glVertex3f( 1.00,0.99,0);
            glColor3f(0.7,0.7,0.2);
            glVertex3f( 1.01,1.00,0);
            glVertex3f(-0.01,1.00,0);

            glColor3f(0.7,0.7,0.2);
            glVertex3f(-0.01,0   ,0);
            glColor3f(0.5,0.4,0.3);
            glVertex3f( 0.00,0.01,0);
            glVertex3f( 0.00,0.99,0);
            glColor3f(0.7,0.7,0.2);
            glVertex3f(-0.01,1.00,0);
            glEnd();
            sprintf(location,"~0(%d %d)", (int)player->x, (int)player->y);
            CFontOutputManager::getInstance()->write_text(location,0,0,0.02);
            CCamera::getInstance()->timeOfDay();
        }
    }
    virtual bool itsOKToContinue(Event* e) {
        /* events we respond to while we're in memory */
        if (e->type==EVENT_SWITCH_STATES) {
            if (e->a==STATE_INTRO) {
                enable(true);
            } else {
                enable(false);
            }
        }
        return enabled;
    }
    virtual void notify(Event* e) {
        if (e->type==EVENT_RENDER_FRAME) {
            CCamera::getInstance()->startGUI();
            render();
            CCamera::getInstance()->endGUI();
        }
        if (e->type==EVENT_CAMERA_TARGET) {
            player=(CRenderable*)e->a;
        }
    }
};
class CGUIMainMenu : public CGUI {
private:
    CLabel logo;
    CButtonStartGame btn_start_game;
    CButtonOptions btn_options;
    CButtonExitGame btn_exit_game;
//  CMap map;
    IEventable* parent;
public:
    CGUIMainMenu(IEventable* parent) : //initialize gui members
        CGUI(0.2,0.2,0.60,0.80),
        logo(0.0,0.05,1.0,0.14128,"./graphics/logo.png", true),
        btn_start_game(0.30,0.25,0.4,0.1,this),
        btn_options(0.30,0.40,0.4,0.1,this),
        btn_exit_game(0.30,0.55,0.4,0.1,this),
        parent(parent) {
        panel_texture=CResourceManager::getInstance()->getSpriteSheet("./graphics/ui/ui_mainmenu.png");
        widgets.push_back(&btn_start_game);
        ((CButton*)widgets[0])->setSelected();
        widgets.push_back(&btn_options);
        widgets.push_back(&btn_exit_game);
    }

    virtual bool itsOKToContinue(Event* e) {
        /* events we respond to while we're in memory */
        if (e->type==EVENT_SWITCH_STATES) {
            if (e->a==STATE_INTRO) {
                enable(true);
            } else {
                enable(false);
            }
        }
        return enabled;
    }
    virtual void notify(Event* e) {
//    if (!itsOKToContinue(e))
//      return;
        /* events we respond to while we're in memory */
        /* otherwise we leave */

        if (e->type==EVENT_CAMERA_MOVE_START) {
            if (e->a==DIRECTION_NORTH) {
                if (widgets[getPrevButton()] != widgets[selected]) {
                    notifyMembers(Event(EVENT_BUTTON_SELECTED,(long)widgets[getPrevButton()]));
                    if (widgets[selected]->getUIType()==UI_TYPE_BUTTON)
                        ((CButton*)widgets[selected])->stopPressing();
                    selected=getPrevButton();
                }
            }
            if (e->a==DIRECTION_SOUTH) {
                if (widgets[getNextButton()] != widgets[selected]) {
                    notifyMembers(Event(EVENT_BUTTON_SELECTED,(long)widgets[getNextButton()]));
                    if (widgets[selected]->getUIType()==UI_TYPE_BUTTON)
                        ((CButton*)widgets[selected])->stopPressing();
                    selected=getNextButton();
                }
            }
        } else if (e->type==EVENT_COMMAND_USE_START) {
            notifyMembers(Event(EVENT_BUTTON_PRESSED_DOWN,(long)widgets[selected]));
        } else if (e->type==EVENT_COMMAND_USE_END) {
            notifyMembers(Event(EVENT_BUTTON_PRESSED_RELEASE, (long)widgets[selected]));
        } else if (e->type==EVENT_RENDER_FRAME) {
            CCamera::getInstance()->startGUI();
            glDisable(GL_ALPHA);
            logo.render();
            render();
            glEnable(GL_ALPHA);
            CCamera::getInstance()->endGUI();
        }
    }
};
class CGUICharacterSelect : public CGUI {
private:
    CLabel label_skill_axes;
    CLabel label_skill_dodge;
    CLabel label_skill_health;
    CLabel label_skill_hide;
    CLabel label_skill_maces;
    CLabel label_skill_magery;
    CLabel label_skill_meditate;
    CLabel label_skill_swords;
    CLabel label_stat_strength;
    CLabel label_stat_dexterity;
    CLabel label_stat_intelligence;
    CText textbox_skill_axes;
    CText textbox_skill_dodge;
    CText textbox_skill_health;
    CText textbox_skill_hide;
    CText textbox_skill_maces;
    CText textbox_skill_magery;
    CText textbox_skill_meditate;
    CText textbox_skill_swords;
    CText textbox_stat_strength;
    CText textbox_stat_dexterity;
    CText textbox_stat_intelligence;
    CText textbox_skill_points_left;
    CText textbox_stat_points_left;
    CLabel label_dagon_portrait;
    CLabel label_top_panel;
//  CLabel lblBotPanel;
    CButtonFinishCharacter button_finish_character;
    int skill[8];
    int stat[3];
    int skill_points_left;
    int stat_points_left;
    const int minimum_stat_points;
    const int maximum_stat_points;
    const int maximum_skill_points;
    IEventable* parent;
public:
    CGUICharacterSelect(IEventable* parent) : //initialize gui members
        label_skill_axes(    0.15,0.2       ,0.2,0.05,"./graphics/ui/ui_skills_axes.png",this),
        label_skill_dodge(   0.15,0.2+1.0/20.0*1,0.2,0.05,"./graphics/ui/ui_skills_dodge.png",this),
        label_skill_health(  0.15,0.2+1.0/20.0*2,0.2,0.05,"./graphics/ui/ui_skills_health.png",this),
        label_skill_hide(    0.15,0.2+1.0/20.0*3,0.2,0.05,"./graphics/ui/ui_skills_hide.png",this),
        label_skill_maces(   0.15,0.2+1.0/20.0*4,0.2,0.05,"./graphics/ui/ui_skills_maces.png",this),
        label_skill_magery(  0.15,0.2+1.0/20.0*5,0.2,0.05,"./graphics/ui/ui_skills_magery.png",this),
        label_skill_meditate(0.15,0.2+1.0/20.0*6,0.2,0.05,"./graphics/ui/ui_skills_meditate.png",this),
        label_skill_swords(  0.15,0.2+1.0/20.0*7,0.2,0.05,"./graphics/ui/ui_skills_swords.png",this),
        label_stat_strength(      0.55,0.2+1.0/20.0*0,0.1,0.05,"./graphics/ui/ui_stat_str.png",this),
        label_stat_dexterity(      0.55,0.2+1.0/20.0*1,0.1,0.05,"./graphics/ui/ui_stat_dex.png",this),
        label_stat_intelligence(      0.55,0.2+1.0/20.0*2,0.1,0.05,"./graphics/ui/ui_stat_int.png",this),
        label_dagon_portrait(0.55,0.2+1.0/20.0*3,0.30,0.25,"./graphics/ui/ui_portrait_dagon.png",this),
        textbox_skill_axes(     0.45,0.2+1.0/20.0*0,0.025,"0"),
        textbox_skill_dodge(    0.45,0.2+1.0/20.0*1,0.025,"0"),
        textbox_skill_health(   0.45,0.2+1.0/20.0*2,0.025,"0"),
        textbox_skill_hide(     0.45,0.2+1.0/20.0*3,0.025,"0"),
        textbox_skill_maces(    0.45,0.2+1.0/20.0*4,0.025,"0"),
        textbox_skill_magery(   0.45,0.2+1.0/20.0*5,0.025,"0"),
        textbox_skill_meditate( 0.45,0.2+1.0/20.0*6,0.025,"0"),
        textbox_skill_swords(   0.45,0.2+1.0/20.0*7,0.025,"0"),
        textbox_stat_strength(   0.75,0.2+1.0/20.0*0,0.025,"10"),
        textbox_stat_dexterity( 0.75,0.2+1.0/20.0*1,0.025,"10"),
        textbox_stat_intelligence(   0.75,0.2+1.0/20.0*2,0.025,"10"),
        textbox_skill_points_left(   0.45,0.15,0.025,"72"),
        textbox_stat_points_left(   0.75,0.15,0.025,"20"),
        label_top_panel(0.15,0.15,0.7,0.05,"./graphics/ui/ui_characterselect_top_panel.png"),
//  lblBotPanel(0.15,0.2+1.0/20.0*8,0.70,0.25,"./graphics/ui/ui_characterselect_substats_panel.png"),
        button_finish_character(0.15,0.2+1.0/20.0*13,0.20,0.05,this),
//  skill({0}), need new C++ standard to do this, worked fine in linux
//  stat({10,10,10}),
        CGUI(0.0,0.0,1.0,1.0),
        skill_points_left(72),
        stat_points_left(20),
        maximum_skill_points(30),
        minimum_stat_points(10),
        maximum_stat_points(25),
        parent(parent)
    {
        /* dunno how to initialize besides using {0}, so i'll for loop it */
        for (int i=0; i<8; i++) {
            skill[i]=0;
        }
        for (int i=0; i<3; i++) {
            stat[i]=10;
        }
//    CEventManager::getInstance()->subscribe(0,this);
        panel_texture=CResourceManager::getInstance()->getSpriteSheet("./graphics/ui/ui_characterselectmenu.png");
        for (int i=SKILL_AXES; i<=SKILL_SWORDS; i++) {
            widgets.push_back(new CButtonSkillRaise(i,0.35,0.20+(float)i/20,0.05,0.05,this));
            widgets.push_back(new CButtonSkillLower(i,0.40,0.20+(float)i/20,0.05,0.05,this));
            widgets.push_back(new CLabel(0.45,0.20+(float)i/20,0.10,0.05,"./graphics/ui/ui_panel_input.png"));
        }
        for (int i=STAT_STR; i<= STAT_INT; i++) {

            widgets.push_back(new CButtonStatRaise(i,0.65,0.20+(float)i/20,0.05,0.05,this));
            widgets.push_back(new CButtonStatLower(i,0.70,0.20+(float)i/20,0.05,0.05,this));
            widgets.push_back(new CLabel(0.75,0.20+(float)i/20,0.10,0.05,"./graphics/ui/ui_panel_input.png"));
        }
        ((CButton*)widgets[0])->setSelected();
        widgets.push_back(&label_skill_axes);
        widgets.push_back(&label_skill_dodge);
        widgets.push_back(&label_skill_health);
        widgets.push_back(&label_skill_hide);
        widgets.push_back(&label_skill_maces);
        widgets.push_back(&label_skill_magery);
        widgets.push_back(&label_skill_meditate);
        widgets.push_back(&label_skill_swords);
        widgets.push_back(&label_stat_strength);
        widgets.push_back(&label_stat_dexterity);
        widgets.push_back(&label_stat_intelligence);
        widgets.push_back(&label_dagon_portrait);
        widgets.push_back(&textbox_skill_axes);
        widgets.push_back(&textbox_skill_dodge);
        widgets.push_back(&textbox_skill_health);
        widgets.push_back(&textbox_skill_hide);
        widgets.push_back(&textbox_skill_maces);
        widgets.push_back(&textbox_skill_magery);
        widgets.push_back(&textbox_skill_meditate);
        widgets.push_back(&textbox_skill_swords);
        widgets.push_back(&textbox_stat_strength);
        widgets.push_back(&textbox_stat_dexterity);
        widgets.push_back(&textbox_stat_intelligence);
        widgets.push_back(&label_top_panel);
        widgets.push_back(&textbox_skill_points_left);
        widgets.push_back(&textbox_stat_points_left);
//    widgets.push_back(&lblBotPanel);
        widgets.push_back(&button_finish_character);
    }
    //why didn't we use an array for the textObjects?
    //because if we were to use one, we would have to initialize each member somehow
    //i think it requires c++0x at least
    CText* getSkillObject(int skill_number) {
        switch(skill_number) {
        case SKILL_AXES:
            return &textbox_skill_axes;
        case SKILL_DODGE:
            return &textbox_skill_dodge;
        case SKILL_HEALTH:
            return &textbox_skill_health;
        case SKILL_HIDE:
            return &textbox_skill_hide;
        case SKILL_MACES:
            return &textbox_skill_maces;
        case SKILL_MAGERY:
            return &textbox_skill_magery;
        case SKILL_MEDITATE:
            return &textbox_skill_meditate;
        case SKILL_SWORDS:
            return &textbox_skill_swords;
        default:
            return 0;
        }
    }
    CText* getStatObject(int stat_number) {
        switch(stat_number) {
        case STAT_STR:
            return &textbox_stat_strength;
        case STAT_DEX:
            return &textbox_stat_dexterity;
        case STAT_INT:
            return &textbox_stat_intelligence;
        default:
            return 0;
        }
    }
    void lowerStat(int stat_number) {
        if (stat[stat_number]-1 >= minimum_stat_points) {
            stat[stat_number]--;
            stat_points_left++;
            textbox_stat_points_left.setValue(stat_points_left);
            getStatObject(stat_number)->setValue(stat[stat_number]);
        } else {
            INFO(LOG) << "Can't lower : " << stat_points_left << " left";
        }
    }
    void raiseStat(int stat_number) {
        if (stat[stat_number]+1 <= maximum_stat_points && stat_points_left > 0) {
            stat[stat_number]++;
            stat_points_left--;
            textbox_stat_points_left.setValue(stat_points_left);
            getStatObject(stat_number)->setValue(stat[stat_number]);
        } else {
            INFO(LOG) << "Can't raise higher : " << stat_points_left <<  " left";
        }
    }
    void lowerSkill(int skill_number) {
        if (skill[skill_number]-3 >= 0) {
            skill[skill_number]-=3;
            skill_points_left+=3;
            textbox_skill_points_left.setValue(skill_points_left);
            getSkillObject(skill_number)->setValue(skill[skill_number]);
        } else {
            INFO(LOG) << "Can't lower skill : " << skill_points_left << " skill points left";
        }
    }
    void raiseSkill(int skill_number) {
        if (skill[skill_number]+3 <= maximum_skill_points && skill_points_left > 0) {
            skill[skill_number]+=3;
            skill_points_left-=3;
            textbox_skill_points_left.setValue(skill_points_left);
            getSkillObject(skill_number)->setValue(skill[skill_number]);
        } else {
            INFO(LOG) << "Can't raise skill : " << skill_points_left << " skill points left";
        }
    }
    virtual bool itsOKToContinue(Event* e) {
        /* events we respond to while we're in memory */
        if (e->type==EVENT_SWITCH_STATES) {
            if (e->a==STATE_CHARACTER_SELECT) {
                enable(true);
            } else {
                enable(false);
            }
        }
        return enabled;
    }
    virtual void notify(Event* e) {
//    if (!itsOKToContinue(e)) {
//      return;
//    } else {
        /* events we respond to while we're in memory */
        /* otherwise we leave */
        if (e->type==EVENT_CAMERA_MOVE_START) {
            if (e->a==DIRECTION_NORTH) {
                if (widgets[getPrevButton()] != widgets[selected]) {
                    notifyMembers(Event(EVENT_BUTTON_SELECTED,(long)widgets[getPrevButton()]));
                    if (widgets[selected]->getUIType()==UI_TYPE_BUTTON)
                        ((CButton*)widgets[selected])->stopPressing();
                    selected=getPrevButton();
                }
            }
            if (e->a==DIRECTION_SOUTH) {
                if (widgets[getNextButton()] != widgets[selected]) {
                    notifyMembers(Event(EVENT_BUTTON_SELECTED,(long)widgets[getNextButton()]));
                    if (widgets[selected]->getUIType()==UI_TYPE_BUTTON)
                        ((CButton*)widgets[selected])->stopPressing();
                    selected=getNextButton();
                }
            }
        } else if (e->type==EVENT_COMMAND_USE_START) {
            notifyMembers(Event(EVENT_BUTTON_PRESSED_DOWN,(long)widgets[selected]));
        } else if (e->type==EVENT_COMMAND_USE_END) {
            notifyMembers(Event(EVENT_BUTTON_PRESSED_RELEASE, (long)widgets[selected]));
        } else if (e->type==EVENT_RENDER_FRAME) {
            CCamera::getInstance()->startGUI();
            render();
            CCamera::getInstance()->endGUI();
        } else if (e->type==EVENT_BUTTON_DECREASE_SKILL) {
            lowerSkill(e->a);
        } else if (e->type==EVENT_BUTTON_INCREASE_SKILL) {
            raiseSkill(e->a);
        } else if (e->type==EVENT_BUTTON_DECREASE_STAT) {
            lowerStat(e->a);
        } else if (e->type==EVENT_BUTTON_INCREASE_STAT) {
            raiseStat(e->a);
        }
    }
//  }
};

#endif


