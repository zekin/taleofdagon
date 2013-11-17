#ifndef TYPES_H
#define TYPES_H
enum {null=0};
typedef unsigned long  u64;
typedef unsigned int   u32;
typedef unsigned short u16;
typedef unsigned char  u8;
typedef signed long    s64;
typedef signed int     s32;
typedef signed short   s16;
typedef signed char    s8;
typedef float          f32;
typedef double         f64;


typedef struct vec2i vec2i;
typedef struct vec2f vec2f;
typedef struct vec3i vec3i;
typedef struct vec3f vec3f;

struct vec2i { s32 x; s32 y; };
struct vec2f { f32 x; f32 y; };
struct vec3i { s32 x; s32 y; s32 z; };
struct vec3f { f32 x; f32 y; f32 z; };



#endif