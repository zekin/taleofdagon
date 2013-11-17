#ifndef IRESOURCE_H
#define IRESOURCE_H
#include <string>
#include <iostream>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>
#include "enum.h"
#include <iomanip>

enum {
  RESOURCE_IMAGE,
  RESOURCE_MUSIC,
  RESOURCE_SOUND
};
class IResource {
protected:
  int type;
  std::string path;
public:
  IResource() {}
  IResource(std::string path) : path(path) {}
  virtual ~IResource() {}
  virtual bool load(std::string path) {}
};

class CSpriteSheet : public IResource {
protected:
  SDL_Surface* surface;
  unsigned int* sheet_texture_id;
  int cell_width;
  int cell_height;
public:
  CSpriteSheet() {}
  CSpriteSheet(std::string path, int cell_width, int cell_height) : IResource(path), cell_width(cell_width), cell_height(cell_height), surface(0) 
  {
    load();
    loadTexture();
  }
  virtual ~CSpriteSheet() {
    delete[] sheet_texture_id;
    if (surface)
      delete surface;
  }
  virtual bool load() {
    surface=IMG_Load(path.c_str());
    if (surface)
      std::clog << "Loaded " << path << std::endl;
    else
      std::cerr << "Error: could not load image at path " << path << std::endl;
    if ((surface->w/cell_width*surface->h/cell_height)>0)
      sheet_texture_id=new unsigned int[surface->w/cell_width * surface->h/cell_height];
  }
  SDL_Rect getFrameSDL(int i) {
    if (cell_width > surface->w) {
      std::cerr << "Error: Width of frame is bigger than cell width, using image dimensions" << std::endl;
      cell_width=surface->w;
    }
    if (cell_width < 1) {
      std::cerr << "Error: Width of frame is less than 1, setting to 1." << std::endl;
      cell_width=1;
    }
    if (cell_height > surface->h) {
      std::cerr << "Error: Height of frame is bigger than cell height, using image dimensions" << std::endl;
      cell_height=surface->h;
    }
    if (cell_height < 1) {
      std::cerr << "Error: Height of frame is less than 1, setting to 1." << std::endl;
      cell_height=1;
    }
    if (i<0) {
      std::cerr << "Error: Frame less than 0 passed into frame animation" << std::endl;
    }
    int WIDTH_CELLS=surface->w/cell_width;
    int HEIGHT_CELLS=surface->h/cell_width;
    int x1=(int)((i%WIDTH_CELLS)*cell_width);
    int y1=(int)((i/WIDTH_CELLS)*cell_height);
    int x2=(int)((i%WIDTH_CELLS+1)*cell_width);
    int y2=(int)((i/WIDTH_CELLS+1)*cell_height);

    SDL_Rect r = {x1,y1,cell_width,cell_height};
    return r;
  }  Rect getFrame(int i) {
    if (cell_width > surface->w) {
      std::cerr << "Error: Width of frame is bigger than cell width, using image dimensions" << std::endl;
      cell_width=surface->w;
    }
    if (cell_width < 1) {
      std::cerr << "Error: Width of frame is less than 1, setting to 1." << std::endl;
      cell_width=1;
    }
    if (cell_height > surface->h) {
      std::cerr << "Error: Height of frame is bigger than cell height, using image dimensions" << std::endl;
      cell_height=surface->h;
    }
    if (cell_height < 1) {
      std::cerr << "Error: Height of frame is less than 1, setting to 1." << std::endl;
      cell_height=1;
    }
    if (i<0) {
      std::cerr << "Error: Frame less than 0 passed into frame animation" << std::endl;
    }
    int WIDTH_CELLS=surface->w/cell_width;
    int HEIGHT_CELLS=surface->h/cell_width;
    double x1=(double)((i%WIDTH_CELLS)*cell_width)/(double)(WIDTH_CELLS*cell_width);
    double y1=(double)((i/WIDTH_CELLS)*cell_height)/(double)(HEIGHT_CELLS*cell_height);
    double x2=(double)((i%WIDTH_CELLS+1)*cell_width)/(double)(WIDTH_CELLS*cell_width);//*cell_width)/(WIDTH_CELLS*cell_width);
    double y2=(double)((i/WIDTH_CELLS+1)*cell_height)/(double)(HEIGHT_CELLS*cell_height);

    return Rect(XY(x1,y1),XY(x2,y1),XY(x1,y2),XY(x2,y2));
  }
  
  virtual SDL_Surface* getResource() { return surface; };
  void loadTexture() { 
    const int max_frames=(surface->w/cell_width) * (surface->h/cell_height);
    std::clog << "Number of textures to load " << max_frames << std::endl;
    if (max_frames<0) { 
      std::cerr << "Error: Trying to allocate a negative number of textures?" << std::endl; 
    } else if (max_frames>1000) { 
      std::cerr << "Error: Why are you trying to allocate so many textures? (> 1000)" << std::endl; 
    }
    glGenTextures(max_frames,sheet_texture_id);
    for (int i=0; i<max_frames; i++) {
      glBindTexture(GL_TEXTURE_2D, sheet_texture_id[i]);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      SDL_Surface* temp_surface=SDL_CreateRGBSurface(0,cell_width,cell_height,32,0xFF,0xFF00,0xFF0000,0xFF000000);
      SDL_Rect r1=getFrameSDL(i);
      SDL_BlitSurface(surface, &r1, temp_surface, 0);
      gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,temp_surface->w,temp_surface->h,GL_RGBA,GL_UNSIGNED_BYTE,temp_surface->pixels);
      delete temp_surface;
    }
  }
  unsigned int getTextureID(int num) {
    return sheet_texture_id[num];
  }
  
};

typedef std::vector<int> FRAMELIST;
//class CAnimationSheet : public CSpriteSheet {
//  std::vector<FRAMELIST> animations;
//  CAnimationSheet(std::string path, int cell_width, int cell_height) : CSpriteSheet(path), cell_width(cell_width), cell_height(cell_height), surface(0) {}
//  
//  void loadAnimation(FRAMELIST frame_list) {
//    
//  }
//};
class CMapSheet : public IResource {
protected:
  SDL_Surface* surface;
  unsigned int* sheet_texture_id;
  int cell_width;
  int cell_height;
public:
  CMapSheet() {}
  CMapSheet(std::string path, int cell_width, int cell_height) : IResource(path), cell_width(cell_width), cell_height(cell_height), surface(0) 
  {
    load();
    loadTexture();
  }
  virtual ~CMapSheet() {
    delete[] sheet_texture_id;
    if (surface)
      delete surface;
  }
  virtual bool load() {
    surface=IMG_Load(path.c_str());
    if (surface==0)
      std::cerr << "Error: could not load image at path " << path << std::endl;
    else
      std::clog << "Loaded " << path << std::endl;
    if ((surface->w/cell_width*surface->h/cell_height)>0)
      sheet_texture_id=new unsigned int[surface->w/cell_width * surface->h/cell_height];
  }
  SDL_Rect getFrameSDL(int i) {
    if (cell_width > surface->w) {
      std::cerr << "Error: Width of frame is bigger than cell width, using image dimensions" << std::endl;
      cell_width=surface->w;
    }
    if (cell_width < 1) {
      std::cerr << "Error: Width of frame is less than 1, setting to 1." << std::endl;
      cell_width=1;
    }
    if (cell_height > surface->h) {
      std::cerr << "Error: Height of frame is bigger than cell height, using image dimensions" << std::endl;
      cell_height=surface->h;
    }
    if (cell_height < 1) {
      std::cerr << "Error: Height of frame is less than 1, setting to 1." << std::endl;
      cell_height=1;
    }
    if (i<0) {
      std::cerr << "Error: Frame less than 0 passed into frame animation" << std::endl;
    }
    int WIDTH_CELLS=surface->w/cell_width;
    int HEIGHT_CELLS=surface->h/cell_width;
    int x1=(int)((i%WIDTH_CELLS)*cell_width);
    int y1=(int)((i/WIDTH_CELLS)*cell_height);
    int x2=(int)((i%WIDTH_CELLS+1)*cell_width);
    int y2=(int)((i/WIDTH_CELLS+1)*cell_height);

    SDL_Rect r = {x1,y1,cell_width,cell_height};
    return r;
  }  Rect getFrame(int i) {
    if (cell_width > surface->w) {
      std::cerr << "Error: Width of frame is bigger than cell width, using image dimensions" << std::endl;
      cell_width=surface->w;
    }
    if (cell_width < 1) {
      std::cerr << "Error: Width of frame is less than 1, setting to 1." << std::endl;
      cell_width=1;
    }
    if (cell_height > surface->h) {
      std::cerr << "Error: Height of frame is bigger than cell height, using image dimensions" << std::endl;
      cell_height=surface->h;
    }
    if (cell_height < 1) {
      std::cerr << "Error: Height of frame is less than 1, setting to 1." << std::endl;
      cell_height=1;
    }
    if (i<0) {
      std::cerr << "Error: Frame less than 0 passed into frame animation" << std::endl;
    }
    int WIDTH_CELLS=surface->w/cell_width;
    int HEIGHT_CELLS=surface->h/cell_width;
    double x1=(double)((i%WIDTH_CELLS)*cell_width)/(double)(WIDTH_CELLS*cell_width);
    double y1=(double)((i/WIDTH_CELLS)*cell_height)/(double)(HEIGHT_CELLS*cell_height);
    double x2=(double)((i%WIDTH_CELLS+1)*cell_width)/(double)(WIDTH_CELLS*cell_width);//*cell_width)/(WIDTH_CELLS*cell_width);
    double y2=(double)((i/WIDTH_CELLS+1)*cell_height)/(double)(HEIGHT_CELLS*cell_height);

    return Rect(XY(x1,y1),XY(x2,y1),XY(x1,y2),XY(x2,y2));
  }
  
  virtual SDL_Surface* getResource() { return surface; };
  void loadTexture() { 
    const int max_frames=(surface->w/cell_width) * (surface->h/cell_height);
    std::clog << "Number of textures to load " << max_frames << std::endl;
    if (max_frames<0) { 
      std::cerr << "Error: Trying to allocate a negative number of textures?" << std::endl; 
    } else if (max_frames>1000) { 
      std::cerr << "Error: Why are you trying to allocate so many textures? (> 1000)" << std::endl; 
    }
    SDL_Surface* s=IMG_Load("./graphics/mask.png");
    glGenTextures(max_frames,sheet_texture_id);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
    glDisable(GL_DEPTH_TEST);
    for (int i=0; i<max_frames; i++) {
      glBindTexture(GL_TEXTURE_2D, sheet_texture_id[i]);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      SDL_Surface* temp_surface=SDL_CreateRGBSurface(0,cell_width*2,cell_height*2,32,s->format->Rmask,s->format->Gmask,s->format->Bmask,s->format->Amask);
      for (int j=0; j<cell_width*cell_height*4; j++) {
       ((unsigned int*)temp_surface->pixels)[j] = ((unsigned int*)s->pixels)[j]&0xFF000000;
//      ((unsigned int*)temp_surface->pixels)[j] |= ((unsigned int*)surface->pixels)[surface->w*(j/16)+(j%16)+16*i]&0xFFFFFF;
      }
      SDL_Rect r1=getFrameSDL(i);
      SDL_Rect r2={0,0,cell_width,cell_height};
      SDL_BlitSurface(surface, &r1, temp_surface, &r2);
      r2.x+=cell_width;
      SDL_BlitSurface(surface, &r1, temp_surface, &r2);     
      r2.y+=cell_height;
      SDL_BlitSurface(surface, &r1, temp_surface, &r2);     
      r2.x-=cell_width;
      SDL_BlitSurface(surface, &r1, temp_surface, &r2);     
//      for (int i=0; i<cell_width*cell_height*4; i++)
        gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,temp_surface->w,temp_surface->h,GL_RGBA,GL_UNSIGNED_BYTE,temp_surface->pixels);
//      gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,s->w,s->h,GL_RGBA,GL_UNSIGNED_BYTE,s->pixels);
      delete temp_surface;
    }
  }
  unsigned int getTextureID(int num) {
    return sheet_texture_id[num];
  }
  
};
#endif
//    gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,surface->w,surface->h,GL_RGB,GL_UNSIGNED_BYTE,surface->pixels);
    
//    SDL_Surface* mask=IMG_Load("./graphics/mask.png");
//    if (mask==NULL) {
//      std::clog << "Error: could not load mask" << std::endl;
//    } else {
//      std::clog << "Created new mask surface" << std::endl;
//    }
//    SDL_Surface* temp_surface=SDL_CreateRGBSurface(0,surface->w*2,surface->h*2,32,0xFF,0xFF00,0xFF0000,0xFF000000);
//    if (temp_surface==NULL) {
//      std::clog << "Error: Could not create a new surface." << std::endl;
//    } else {
//      std::clog << "Created new surface" << std::endl;
//    }
//
//    for (int i=0; i<surface->h/cell_height; i++) {
//      for (int j=0; j<surface->w/cell_width; j++) {
//        unsigned int* ptr=(unsigned int*)temp_surface->pixels;
//        SDL_Rect rect1 = {cell_width*j,cell_height*i,cell_width,cell_height};
//        SDL_Rect rect2 = {(cell_width*2)*j,(cell_height*2)*i,cell_width,cell_height};
//        SDL_Rect rect3 = {0,0,64,64};
//        SDL_BlitSurface(surface,&rect1,mask,&rect3);
//        SDL_BlitSurface(surface,&rect1,temp_surface,&rect2);
//        rect2.x+=cell_width;
//        SDL_BlitSurface(surface,&rect1,temp_surface,&rect2);
//        rect2.x-=cell_width;
//        rect2.y+=cell_height;
//        SDL_BlitSurface(surface,&rect1,temp_surface,&rect2);
//        rect2.x+=cell_height;
//        SDL_BlitSurface(surface,&rect1,temp_surface,&rect2);     
//        for(int k=0;k<32;k++)
//          for (int l=0;k<32;k++)
//            ptr[k+l*temp_surface->w]=ptr[k+l*temp_surface->w] | 0x7f000000;
//      }
//    }
 