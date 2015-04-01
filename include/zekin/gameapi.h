#ifndef GAMEAPI_H
#define GAMEAPI_H

#include "util.h"
struct camera
{
  float height, rot1, rot2;
  struct vec3 pos;
  struct vec3 vel;
  struct vec3 acc;  
};

struct game
{
  struct vec2 mouse;
  struct vec2 size;
  unsigned int keys[256];
  unsigned int press[256];
  unsigned int isrunning;
};



#endif