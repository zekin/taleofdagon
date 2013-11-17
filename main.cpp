#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>
#include "CEventManager.h"
#include "CStateIntro.h"
#include "CSoundSystem.h"
#include "CCamera.h"
#include "CMap.h"
#include "CClock.h"
#include <iostream>
#include <stdio.h>

CEventManager* em=0;
CCamera* cam=0;
SDL_Surface* screen=0;

void InitializeSingletons() {
  SDL_Init(SDL_INIT_EVERYTHING);
  IMG_Init(IMG_INIT_PNG);
  em=CEventManager::getInstance();
  cam=CCamera::getInstance();
  
  screen=SDL_SetVideoMode(640,480,32,SDL_OPENGL|SDL_GL_DOUBLEBUFFER);
  /* initialize the fuck out of opengl here */
  glClearColor(0.0f,0.0f,0.0f,0.0f);
  glClearDepth(1.0f);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
//  glOrtho(-5.0,5.0,-5.0,5.0,-100,100);
  gluPerspective(45.0,640.0/480.0,0.1,100);
  glMatrixMode(GL_MODELVIEW);
  SDL_WM_SetCaption("Tale of Dagon", 0);
}

int SDL_EventHandling() {
  SDL_Event e;
  /* sending messages from the top to the bottom, ohhh yeah, this is how we do in the cargo cult */
  while(SDL_PollEvent(&e)) {
    switch(e.type) {
      case SDL_QUIT:
        return 0;
      case SDL_KEYDOWN:
        em->notify(Event(EVENT_KEYPRESS_DOWN,e.key.keysym.sym));
        break;
      case SDL_KEYUP:
        em->notify(Event(EVENT_KEYPRESS_UP,e.key.keysym.sym));
        break;
      default:
        break;
    }
  }
  return 1;
}
int main(int argc, char **argv) {
  InitializeSingletons();

  CSoundSystem SoundManager;
  CStateIntro state_intro;
//  CStateGame state_game;
  CClock clock(60.0,1.0);
  /* pointers will be signed integers and we will grow oranges in alaska */
  em->notify(Event(EVENT_GAME_STARTED, 0, 25.0)); 
  em->notify(Event(EVENT_SWITCH_STATES, STATE_INTRO));
  cam->setTarget(50.0,50.0,0.0);
  while(SDL_EventHandling()) {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    cam->setupCamera();
    clock.update();
   // em->notify(Event(EVENT_RENDER_FRAME));
    
//    SDL_Delay(25);
    SDL_GL_SwapBuffers();
  }
  return 0;
}
