#ifndef CMAP_H
#define CMAP_H
#include <math.h>
#include <iomanip>
#include "CEventManager.h"
#include "enum.h"
#include "IResource.h"
#include "CCamera.h"
#include <zekin/noise.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

struct Tile {
  int num;
  Tile(int tile_number) : num(tile_number) {}
  CLASSIFIER getClassifier() {
    switch(num) {
      case TILE_GRASS:
      case TILE_DIRT: 
      case TILE_ROCKS:
      case TILE_SAND:
      case TILE_MUD:
      case TILE_TALL_GRASS:
      case TILE_PAVEMENT:
      case TILE_COBBLESTONE:
        return GROUND;
      case TILE_WATER:
        return SEA;
      case TILE_MOUNTAIN:
      case TILE_MOUNTAIN_PEAK:
      case TILE_MOUNTAIN_LULL:
        return CLIFF;
    }
  }
};
class CMap : public IEventable {
private:
  int width;
  int height;
  std::vector<Tile> tiles;
  std::vector<unsigned int> tile_call;
  unsigned int texture_tiles[20];
  CMapSheet* map_tiles_resource;
  SDL_Surface* map_tiles_surface;
  double sphere(double radius, double x, double y) {
    double value=sqrt(radius*radius-(x-radius)*(x-radius)-(y-radius)*(y-radius));
    if isnan(value)
      value=0;
    return value;
  }
  /* noise weight : how much more noise is in */
  /* sphere weight : how round the island is */
  /* initial frequency : */
  double generateIslandHeight(double radius, int x, int y, double noise_weight, double sphere_weight, double initial_frequency, double persistance, double passes) {
    double value = ((sphere(radius,x,y)/radius)-0.15)*sphere_weight + 
                  ((fbmXY( ((double)x)*initial_frequency, ((double)y)*initial_frequency,persistance,passes)+1)/2)*noise_weight;
    if (isnan(value)) {value=0;}
    return value;
  }
  
public:
  CMap(int width, int height) : width(width), height(height), map_tiles_surface(0), map_tiles_resource(0) { 
    CEventManager::getInstance()->subscribe(1,this);
    /* load image for tiles */
    map_tiles_resource=CResourceManager::getInstance()->getMapSheet("./graphics/tile3.png",16,16);
//    map_tiles_resource = new CMapSheet("./graphics/tile3.png",16,16);
  } 
  Tile* at(int x, int y) {
    if (x<0) {
      x=width+x%width;
    } else {
      x=x%width;
    }
    if (y<0) {
      y=height+y%height;
    } else {
      y=y%height;
    }
      return &tiles[y*width+x];
  }
  
  int hasTileWall(int direction, int x, int y) {
    Tile* src_tile=at(x,y);
    Tile* dest_tile;
    if (src_tile==0) 
      return 0;
    if (src_tile->num==1) 
      return 0;
    switch(direction) {
      case DIRECTION_NORTH: 
        dest_tile=at(x,y+1);
        if (dest_tile==0)
          return 1;
        if (dest_tile->num==1)
          return 1;
        else
          return 0;
      case DIRECTION_EAST:
        dest_tile=at(x+1,y);
        if (dest_tile==0)
          return 1;
        if (dest_tile->num==1)
          return 1;
        else
          return 0;
      case DIRECTION_SOUTH:
        dest_tile=at(x,y-1);
        if (dest_tile==0)
          return 1;
        if (dest_tile->num==1)
          return 1;
        else
          return 0;
      case DIRECTION_WEST:
        dest_tile=at(x-1,y);
        if (dest_tile==0)
          return 1;
        if (dest_tile->num==1)
          return 1;
        else
          return 0;
      default:
        return 0;
    }
  }
  void renderWall(int direction) {
    glBindTexture(GL_TEXTURE_2D, map_tiles_resource->getTextureID(3));   
    switch(direction) {
      case DIRECTION_NORTH:
        break;
      case DIRECTION_EAST:
        glBegin(GL_QUADS);
        glTexCoord2d(0.0,1.0);
        glVertex3d(0.5,-0.5,-0.5);
        glTexCoord2d(0.0,0.0);       
        glVertex3d(0.5,-0.5,0.5);
        glTexCoord2d(1.0,0.0);
        glVertex3d(0.5,0.5,0.5);
        glTexCoord2d(1.0,1.0);       
        glVertex3d(0.5,0.5,-0.5);
        glEnd();
        break;
      case DIRECTION_SOUTH:
        glBegin(GL_QUADS);        
        glTexCoord2d(0.0,1.0);
        glVertex3d(-0.5,-0.5,-0.5);
        glTexCoord2d(0.0,0.0);
        glVertex3d(-0.5,-0.5,0.5);
        glTexCoord2d(1.0,0.0);
        glVertex3d(0.5,-0.5,0.5);
        glTexCoord2d(1.0,1.0);
        glVertex3d(0.5,-0.5,-0.5);
        glEnd();
        break;
      case DIRECTION_WEST:
        glBegin(GL_QUADS);
        glTexCoord2d(0.0,1.0);
        glVertex3d(-0.5,0.5,-0.5);
        glTexCoord2d(0.0,0.0);
        glVertex3d(-0.5,0.5,0.5);
        glTexCoord2d(1.0,0.0);
        glVertex3d(-0.5,-0.5,0.5);
        glTexCoord2d(1.0,1.0);
        glVertex3d(-0.5,-0.5,-0.5);
        glEnd();
        break;
      default:
        break; 
    }
  }
  void initialize() {
    /* load tile data */
    
    const double radius= (width > height) ? height/2.0 : width/2.0;
    const double x=width/2;
    const double y=height/2;
    for (int i=0; i<height; i++) {
      for (int j=0; j<width; j++) {
        double val=generateIslandHeight(radius,j, i, 0.65, 0.35, 0.09, 3.4, 5);
//        std::clog << "Tile " << j << ", " << i << " height: " << val << std::endl;
        if (0.0 <= val && val <= 0.48)
          tiles.push_back(Tile(TILE_WATER));
        else if (0.45 < val && val <= 0.53)
          tiles.push_back(Tile(TILE_SAND));
        else if (0.5 < val && val <= 0.7)
          tiles.push_back(Tile(TILE_GRASS));
        else if (0.8 < val && val <= 1.0)
          tiles.push_back(Tile(TILE_MOUNTAIN));
        else
          tiles.push_back(Tile(TILE_MOUNTAIN));
//          std::cout << "value outside of generation, expected 0 to 1 : val-" << val << std::endl;
      }
    }
    for (int i=0; i<16; i++) {
      tile_call.push_back(glGenLists(1));
      glNewList(tile_call[i],GL_COMPILE);
      glBindTexture(GL_TEXTURE_2D, map_tiles_resource->getTextureID(i));
      glBegin(GL_QUADS);
      if (i==1) {
        glTexCoord2d(0.0,1.0);
        glVertex3d(-1,-1,0);
        glTexCoord2d(0.0,0.0);
        glVertex3d(-1,1,0);
        glTexCoord2d(1.0,0.0);
        glVertex3d(1,1,0);
        glTexCoord2d(1.0,1.0);
        glVertex3d(1,-1,0);
      } else {
        glTexCoord2d(0.0,1.0);
        glVertex3d(-1,-1,0.1);
        glTexCoord2d(0.0,0.0);
        glVertex3d(-1,1,0.1);
        glTexCoord2d(1.0,0.0);
        glVertex3d(1,1,0.1);
        glTexCoord2d(1.0,1.0);
        glVertex3d(1,-1,0.1);
        glEnd();
        /*
        if (hasTileWall(DIRECTION_NORTH, XY(this
        glBindTexture(GL_TEXTURE_2D, map_tiles_resource->getTextureID(3));
        glBegin(GL_QUADS);
        glTexCoord2d(0.0,1.0);
        glVertex3d(0.5,-0.5,-0.5);
        glTexCoord2d(0.0,0.0);       
        glVertex3d(0.5,-0.5,0.5);
        glTexCoord2d(1.0,0.0);
        glVertex3d(0.5,0.5,0.5);
        glTexCoord2d(1.0,1.0);       
        glVertex3d(0.5,0.5,-0.5);
       */
      }
      glEnd();
      glEndList();
    }
  }
  void RenderTiles(float time) {
    static float lastAnimTime=0;
    static int index=0;
    const float delay=0.25;
    glEnable(GL_BLEND);    
    if (time>lastAnimTime+delay) {
      index=(index+1)%4;
      lastAnimTime=time;
    }
    XY pos=CCamera::getInstance()->getXY();
    XY pos_frac=CCamera::getInstance()->getXY();
    pos_frac.x=pos.x-(round(pos.x));     
    pos_frac.y=pos.y-(round(pos.y));
    glTranslatef(-(30+pos_frac.x),-(30+pos_frac.y),0);
      /* render tiles */
    for (int i=round(pos.y)-30; i<=round(pos.y)+30; i++) {
      glPushMatrix();
      for (int j=round(pos.x)-30; j<=round(pos.x)+30; j++) {
        Tile* current=at(j,i);
        if (!current) {
        }
        else if (current->num==8) {
          glCallList(tile_call[8+index]);
        }
        else if (current!=0) {
          glCallList(tile_call[current->num]);
        }
        glTranslatef(1,0,0);
      }
      glPopMatrix();
      glTranslatef(0,1,0);
    }
  }
  virtual void notify(Event* e) {
    if (e->type == EVENT_RENDER_FRAME) { 
      RenderTiles(e->x);
    }
    if (e->type == EVENT_GAME_STARTED) {
      initialize();
    }
//  std::clog << "EventID: " << e->type << " Data: " << e->a << std::endl;
//  printf("EventID");
  }
};

class CMapDemo : public CMap {
  
};
#endif