#ifndef UTIL_H
#define UTIL_H
#include <math.h>

  //structures
  struct vec3 { float x,y,z; };
  struct vec2 { float x,y;  };
  
  inline float UTIL_v3len(float x, float y, float z) { return sqrtf(x*x+y*y+z*z); }
  struct vec3 UTIL_v3add(struct vec3 * w, struct vec3 * v)
  {
    struct vec3 t;
    t.x = w->x + v->x;
    t.y = w->y + v->y;
    t.z = w->z + v->z;
    return t;
  }
  struct vec3 UTIL_v3mul(struct vec3 * w, struct vec3 * v)
  {
    struct vec3 t;
    t.x = w->x * v->x;
    t.y = w->y * v->y;
    t.z = w->z * v->z;
    return t;
  }
  struct vec3 UTIL_v3scale(struct vec3 * v, float s)
  {
    struct vec3 t;
    t.x = v->x * s;
    t.y = v->y * s;
    t.z = v->z * s;
    return t;
  }
  float UTIL_v3dot(struct vec3 * w, struct vec3 * v)
  {
    return w->x*v->x + w->y*v->y + w->z*v->z;
  }
#endif