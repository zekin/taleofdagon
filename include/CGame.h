#ifndef CGAME_H
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>
#include "CEventManager.h"
#include "CCamera.h"
#include "CSoundSystem.h"
#include "CEngine.h"
#include "logging.h"
#include "CLocator.h"
#include "CUnitFactory.h"

#include <stdio.h>
#include <stdlib.h>

class CGame {
private:
    CEventManager* em;
    CCamera* cam;
    SDL_Surface* screen;
    CSoundSystem* sound_manager;
    CEngine* engine;
    CCamera* camera;
    CClock* clock;

private:
    bool full_screen_enabled;
    int width;
    int height;

private: /* Services */
    CUnitFactory* unitFactory;

private: /* Helper methods */
    void initializeServiceLocator() {
        unitFactory = new CUnitFactory();

        CLocator::setUnitFactory( (IUnitFactory*) unitFactory );
    }

public:
    CGame() :
        em(0),
        screen(0),
        width(800),
        height(600) {
        try {
            log4cpp::PropertyConfigurator::configure("log4cpp_settings.properties");
        }
        catch (std::exception& e) {
            std::cout << "log4cpp exception: " << e.what();
        }
    }

    void initialize(unsigned int fullscreen) {
        INFO(LOG) << "====STARTING GAME====";
        full_screen_enabled = (fullscreen != 0);
        SDL_Init(SDL_INIT_EVERYTHING);
        const SDL_VideoInfo* screen_information=SDL_GetVideoInfo();
        if (screen_information != 0 && fullscreen==SDL_FULLSCREEN) {
            width=screen_information->current_w;
            height=screen_information->current_h;

            INFO(LOG) << "Width " << width << ", Height" << height;
        } else {
            INFO(LOG) << "Could not find screen resolution, defaulting to 800x600";
        }
        IMG_Init(IMG_INIT_PNG);
        em=CEventManager::getInstance();
        cam=CCamera::getInstance();
        SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL,1);

        if (fullscreen==SDL_FULLSCREEN) {
            screen=SDL_SetVideoMode(width,height,32,SDL_OPENGL|SDL_GL_DOUBLEBUFFER|SDL_FULLSCREEN);
        } else {
            screen=SDL_SetVideoMode(width,height,32,SDL_OPENGL|SDL_GL_DOUBLEBUFFER|SDL_RESIZABLE);
        }
        //Initialize various libraries
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClearDepth(1.0f);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);
        glShadeModel(GL_SMOOTH);

        glDepthFunc(GL_LEQUAL);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
//    glOrtho(-5.0,5.0,-5.0,5.0,-100,100);
        gluPerspective(45.0,width/height,0.1,100);
        glMatrixMode(GL_MODELVIEW);
        SDL_WM_SetCaption("Tale of Dagon", 0);

        sound_manager = new CSoundSystem();
        engine = new CEngine();
        camera=CCamera::getInstance();
        clock=CClock::getInstance();
        em->notify(Event(EVENT_GAME_STARTED, (long)screen, 60.0));
        em->notify(Event(EVENT_SWITCH_STATES, STATE_MENU));

        initializeServiceLocator();
    }

    int events() {
        SDL_Event e;
        /* sending messages from the top to the bottom */
        while(SDL_PollEvent(&e)) {
            switch(e.type) {
            case SDL_QUIT:
                INFO(LOG) << "SDL_QUIT event received!";
                INFO(LOG) << "====ENDING GAME====";
                return 0;
            case SDL_KEYDOWN:
                em->notify(Event(EVENT_KEYPRESS_DOWN,e.key.keysym.sym));
                break;
            case SDL_KEYUP:
                em->notify(Event(EVENT_KEYPRESS_UP,e.key.keysym.sym));
                break;
            case SDL_VIDEORESIZE:
                width=(e.resize.w < 320) ? 320 : e.resize.w;
                height=(e.resize.h < 240) ? 240 : e.resize.h;

                INFO(LOG) << "Window resize event " << width << "," << height;

                screen=SDL_SetVideoMode(width,height,32,
                                        SDL_OPENGL |
                                        SDL_GL_DOUBLEBUFFER |
                                        SDL_RESIZABLE |
                                        (full_screen_enabled*SDL_FULLSCREEN) );

                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                gluPerspective(45.0,(width*1.0f)/(height*1.0f),0.1,100);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                break;
            default:
                break;
            }
        }
        return 1;
    }
    void update() {
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        cam->setupCamera();
        clock->update();
        SDL_GL_SwapBuffers();
    }
};
#endif
