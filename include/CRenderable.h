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
    
    virtual bool isAUnit() {
        switch( type ) {
        case RENDERABLE_UNIT:
            return true;
        default:
            return false;
        }
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
//        this->y-=clock->deltaT()/2.0;//(CClock::getInstance()->getTime()-time_last)/2.0;

//    time_last=CClock::getInstance()->getTime();
    }
};
#endif
