#ifndef GL_TEXT_H
#define GL_TEXT_H

#include <SDL/SDL_ttf.h>
#include "types.h"


typedef struct {
  int advance;
  int xmin;
  int xmax;
  int ymin;
  int ymax;
} Metric;
typedef struct {
  Metric metric;
  SDL_Surface * surface;
  u32 texture;
  u32 list;
} Glyph;
typedef struct {
  u32 height;
  int lineskip;
  int FLAGS;
  TTF_Font * font;
  Glyph glyph[256];
  char buffer[4096];
} Text;

Text _text;
void SDL_InitText()
{
  TTF_Init();
}
void SDL_CreateText(char * fontname, int ptSize)
{
  SDL_Color foreground = {255,255,255,255}, background = {0x0,0x0,0x0,0x0};
  int i=0;
  u32 texBuf[256];
  Metric *m;
  _text.font = TTF_OpenFont(fontname, ptSize);
  if (!_text.font){SDL_SetError("SDL_CreateText: Error creating font. Check the path and name."); return;}
  TTF_SetFontOutline(_text.font, 0);
  
#ifdef GLAPI  
  glEnable(GL_TEXTURE_2D);
  glGenTextures(256, texBuf);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE);       // Blending Function For Translucency Based On Source Alpha 
#endif

  for (i=0; i<256; i++) 
    {
#ifdef GLAPI
      _text.glyph[i].texture=texBuf[i];
#endif
      _text.glyph[i].surface=TTF_RenderGlyph_Blended(_text.font,i,foreground);
      TTF_GlyphMetrics(_text.font,i,
                       &_text.glyph[i].metric.xmin,
                       &_text.glyph[i].metric.xmax,
                       &_text.glyph[i].metric.ymin,
                       &_text.glyph[i].metric.ymax,
                       &_text.glyph[i].metric.advance);
      #ifdef GLAPI
      glBindTexture(GL_TEXTURE_2D,_text.glyph[i].texture);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
      gluBuild2DMipmaps(GL_TEXTURE_2D, 
                        GL_RGBA, 
                        _text.glyph[i].surface->w, 
                        _text.glyph[i].surface->h, 
                        GL_RGBA, 
                        GL_UNSIGNED_BYTE, 
                        _text.glyph[i].surface->pixels);
      _text.glyph[i].list=glGenLists(1);
      glNewList(_text.glyph[i].list,GL_COMPILE);
      m=&_text.glyph[i].metric;
      glBindTexture(GL_TEXTURE_2D, _text.glyph[i].texture); 
      glBegin(GL_QUADS);    
      glTexCoord2f(0.0f, 0.0f);
      glVertex2f(m->xmin, _text.lineskip-m->ymax);// metrics[*p].xmin, _text.lineskip - _text.metrics[*p].ymax);
      glTexCoord2f(1.0f, 0.0f);
      glVertex2f(m->xmin+m->xmax,_text.lineskip-m->ymax);//_text.metrics[*p].xmin+_text.metrics[*p].xmax,_text.lineskip - _text.metrics[*p].ymax);
      glTexCoord2f(1.0f, 1.0f);
      glVertex2f(m->xmin+m->xmax,_text.lineskip-m->ymin);//_text.metrics[*p].xmin+_text.metrics[*p].xmax,_text.lineskip-_text.metrics[*p].ymin);
      glTexCoord2f(0.0f, 1.0f);
      glVertex2f(m->xmin,_text.lineskip-m->ymin);//_text.metrics[*p].xmin,_text.lineskip -_text.metrics[*p].ymin);
      glEnd();
      glEndList();
      glTranslatef(m->advance,0,0);
      #endif
    }
  
  _text.height = TTF_FontHeight(_text.font);
  _text.lineskip = TTF_FontLineSkip(_text.font);
  TTF_CloseFont(_text.font);
}
/*! \brief Uses the currently loaded font to draw to the screen at X, Y on surface scr with the string s
 *  \param s The string to draw
 *  \param scr The SDL_Surface to be drawn to
 *  \param x The x coordinate of the text
 *  \param y The y coordinate of the text
 */

void SDL_PrintText(SDL_Surface * scr, int x, int y,char * s)
{
  char * p; int base=0;
  SDL_Rect r={x,y,0,0};
  if (s==0) return;
  if (!_text.font){SDL_SetError("SDL_PrintText: Font not created or did not initialize correctly."); return;}
  for (p=s; *p != 0; p++)
  {
    r.w = _text.glyph[*p].surface->w;
    r.h = _text.glyph[*p].surface->h;
    r.y = y+_text.lineskip - _text.glyph[*p].metric.ymax;
    r.x += _text.glyph[*p].metric.xmin;
    SDL_BlitSurface(_text.glyph[*p].surface, 0, scr, &r);
    r.x+=_text.glyph[*p].metric.advance-_text.glyph[*p].metric.xmin;
  }
}
#ifdef GLAPI
void SDL_GL_PrintText(float x, float y, float rotation, const char * string)
{
  int start_x=0, 
      start_y=0;
  char * p; 
  /* error checking */
  if (string==0) {SDL_SetError("SDL_GL_PrintText: Null string"); return;}
  /* reset the view */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();  
  glOrtho(0,640,480,0,-1,1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();  
  start_x=x; start_y=y+_text.lineskip;
  glTranslatef(start_x,start_y,0);
  if (rotation != 0)
    {glRotatef(rotation,0,0,-1);}
  for (p=string; *p != 0; p++)
  {
    glCallList(_text.glyph[*p].list);
    glTranslatef(_text.glyph[*p].metric.advance,0,0);
  }  
}
void SDL_GL_printf(float x, float y, float rotation,const char * format, ...)
{  
  int start_x=0, 
      start_y=0;
  char * p; 
  va_list ap;
  int va_len=0;
  /* error checking */
  if (format==0) {SDL_SetError("SDL_GL_PrintText: Null string"); return;}
  /* reset the view */
  va_start(ap,format);
  va_len=vsnprintf(_text.buffer, 4095, format, ap); 
  va_end(ap);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();  
  glOrtho(0,640,480,0,-1,1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();  
  start_x=x; start_y=y+_text.lineskip;
  glTranslatef(start_x,start_y,0);
  if (rotation != 0)
    {glRotatef(rotation,0,0,-1);}
  for (p=_text.buffer; p<_text.buffer+va_len; p++)
  {
    glCallList(_text.glyph[*p].list);
    glTranslatef(_text.glyph[*p].metric.advance,0,0);
  }  
}

#endif
#endif