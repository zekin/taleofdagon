#ifndef CCAMERA_H
#define CCAMERA_H
#include "CClock.h"
#include "CEventManager.h"
#include "enum.h"
#include "CRenderable.h"
#include <SDL/SDL_opengl.h>
#include "logging.h"


class CCamera : public IEventable {
private:
    CRenderable* camera_target;
    CRenderable camera_target_default;
    SDL_Surface* screen; //for dimensions
//  int directions[4];
    bool moving[4];
public:
    CCamera() : camera_target(0) {
        for (int i=0; i<4; i++) {
            moving[i]=0;
        }
        camera_target_default=CRenderable(0,0,-10);
        CEventManager::getInstance()->subscribe(0,this);
        setTarget(&camera_target_default);
    }
    void setTarget(CRenderable* target) {
        if (target==0) {
            INFO(LOG) << "Target being set to camera_target_default";
            camera_target=&camera_target_default;
        } else {
            camera_target=target;
        }
    }
    void setTarget(int x, int y, int z) {
        /* TODO: do map bounds checking i guess */
        INFO(LOG) << "Camera at: (" << x << ", " << y << ", " << z << ")";
        camera_target->x=x;
        camera_target->y=y;
        camera_target->z=z;
    }
    void updateTarget() {
//    if (moving[DIRECTION_NORTH])
//      camera_target_default.y+=0.5/25.0;
//    if (moving[DIRECTION_EAST])
//      camera_target_default.x+=0.5/25.0;
//    if (moving[DIRECTION_SOUTH])
//      camera_target_default.y-=0.5/25.0;
//    if (moving[DIRECTION_WEST])
//      camera_target_default.x-=0.5/25.0;
    }

    XYZ timeOfDay(bool getValues=false) {
        float time_of_day=CClock::getInstance()->time_of_day();
        const float mpi=3.14159265358979;
        float red=((-cos(2*mpi*time_of_day/360.0)+1)/2.0)*0.5 + 0.5;
        float green=((-cos(2*mpi*time_of_day/360.0)+1)/2.0)*0.6 + 0.4;
        float blue=((-cos(2*mpi*time_of_day/360.0)+1)/2.0)*1.0 + 0.3;
        if (getValues==false) {
            glColor3f(red,green,blue);
            return XYZ(red,green,blue);
        } else {
            return XYZ(red,green,blue);
        }
    }
    XY getXY() {
        return XY((double)camera_target->x, (double)camera_target->y);
    }
    void startGUI() {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float extra_sides=((float)screen->w/(float)screen->h - 1.0)/2.0;
        glOrtho(-extra_sides,1.0+extra_sides,1.0,0.0,1,-1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glColor3f(1,1,1);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    }
    void endGUI() {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0,640.0/480.0,0.1,100.0);
        glMatrixMode(GL_MODELVIEW);
        glDisable(GL_BLEND);
        glLoadIdentity();
        timeOfDay();
//    glEnable(GL_DEPTH_TEST);
    }
    void setupCamera() {
//   glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
//    glRotatef(-45,0,1,1);
        glRotatef(-45,1,0,0);
        glTranslatef(0,10,-10);
//    glTranslatef(-camera_target_default.x,-camera_target_default.y,-camera_target_default.z);
    }
    virtual void notify(Event* e) {
        if (e->type==EVENT_GAME_STARTED) {
            if (e->a==0) {
                INFO(LOG) << "Did not receive screen pointer, something is terribly wrong, exit.";
            } else {
                screen=(SDL_Surface*)e->a;
            }

        }
        if (e->type==EVENT_CAMERA_TARGET) {
            if (e->a==0) {
                this->setTarget(e->x,e->y,e->z);
                this->setTarget(&camera_target_default);
            } else {
                INFO(LOG) << "Recieved camera target at (" <<
                          ((CRenderable*)e->a)->x << "," <<
                          ((CRenderable*)e->a)->y << "," <<
                          ((CRenderable*)e->a)->z << ")";
                setTarget((CRenderable*)e->a);
//        camera_target=(CRenderable*)(e->a);
            }
        }// else if (e->type==EVENT_CAMERA_MOVE_START) {
        // if (0 <= e->a && e->a <= 3) {
        //   moving[e->a]=true;
        //   std::cerr << "Moving" << std::endl;
        // } else {
        //   std::cerr << "Error: Incorrect camera start move direction received" << std::endl;
        // }
//    } else if (e->type==EVENT_CAMERA_MOVE_END) {
//      if (0 <= e->a && e->a <= 3) {
//        moving[e->a]=false;
//        std::cerr << "Not Moving" << std::endl;
//      } else {
//        std::cerr << "Error: Incorrect camera end move direction received" << std::endl;
//      }
        /* } */
        else if (e->type==EVENT_RENDER_FRAME) {
            moving[DIRECTION_NORTH]=1;
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
