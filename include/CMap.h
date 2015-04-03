#ifndef CMAP_H
#define CMAP_H
#include <math.h>
#include <iomanip>
#include "CEventManager.h"
#include "enum.h"
#include "IResource.h"
#include "CCamera.h"
#include "CUnit.h"
#include "zekin/noise.h"
#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "logging.h"
#include "IMap.h"



class CMap : public IMap {
public:
  CMap(int width, int height) : 
  IMap(width, height) { }

  CTile* at(int x, int y) {
    int chunk_num=0;
    int chunk_x=0;
    int chunk_y=0;
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
    
//    chunk_x=x/Globals::ChunkSize;
//    chunk_y=y/Globals::ChunkSize*ChunksWide
    chunk_num=(y/Globals::chunk_size)*chunks_wide+(x/Globals::chunk_size);
//    std::clog << "Chunk num requested " << chunk_num << std::endl;
    
//    if (chunk_num > 8) {
//      std::clog << "Chunk num requested " << chunk_num << " x: " << x << " y: " << y << " width is " << width << std::endl;
//      return 0;
//    }
    return chunks.at(chunk_num)->at(x,y);
  }
  virtual CChunk* getChunk(int x, int y) {
    int chunk_num=0;
    int chunk_x=0;
    int chunk_y=0;
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
    
    chunk_num=(y/Globals::chunk_size)*chunks_wide+(x/Globals::chunk_size);
    return chunks.at(chunk_num);
  }
  
  /* TODO: needs refactoring */
  int hasTileWall(int direction, int x, int y) {
    CTile* src_tile=at(x,y);
    CTile* dest_tile;
    if (src_tile==0) 
      return 0;
    if (src_tile->num==1) 
      return 0;
    switch(direction) {
      case DIRECTION_NORTH: 
        dest_tile=at(x,y-1);
        if (dest_tile==0)
          return 1;
        if (dest_tile->num == TILE_MOUNTAIN ||
            dest_tile->num == TILE_ICE_MOUNTAIN ||
            dest_tile->num == TILE_RIGID_MOUNTAIN ||
            dest_tile->num == TILE_CITY_WALL)
          return 1;
        else
          return 0;
        break;
      case DIRECTION_EAST:
        dest_tile=at(x+1,y);
         if (dest_tile->num == TILE_MOUNTAIN ||
            dest_tile->num == TILE_ICE_MOUNTAIN ||
            dest_tile->num == TILE_RIGID_MOUNTAIN ||
            dest_tile->num == TILE_CITY_WALL)
           return 1;
         else return 0;
        break;
     case DIRECTION_SOUTH:
        dest_tile=at(x,y+1);
         if (dest_tile->num == TILE_MOUNTAIN ||
            dest_tile->num == TILE_ICE_MOUNTAIN ||
            dest_tile->num == TILE_RIGID_MOUNTAIN ||
            dest_tile->num == TILE_CITY_WALL)
           return 1;
         else 
           return 0;
        break;
     case DIRECTION_WEST:
        dest_tile=at(x-1,y);
         if (dest_tile->num == TILE_MOUNTAIN ||
            dest_tile->num == TILE_ICE_MOUNTAIN ||
            dest_tile->num == TILE_RIGID_MOUNTAIN ||
            dest_tile->num == TILE_CITY_WALL)
           return 1;
         else 
           return 0;
        break;
     default:
        return 0;
    }
  }
  void renderRoof(int tile) {
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    
    glBindTexture(GL_TEXTURE_2D, map_walls_resource->getTextureID(tile));      
    glBegin(GL_QUADS);
    glTexCoord2d(0.0,1.0);
      glVertex3d(-0.5,-0.5,3.0);
      glTexCoord2d(0.0,0.0);
      glVertex3d(-0.5,0.5,3.0);
      glTexCoord2d(1.0,0.0);
      glVertex3d(0.5,0.5,3.0);
      glTexCoord2d(1.0,1.0);
      glVertex3d(0.5,-0.5,3.0);
    glEnd();
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    
  }
  void renderWall(int direction, int tile) {
    XYZ colors(0,0,0);
    colors=CCamera::getInstance()->timeOfDay(true);   
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, map_walls_resource->getTextureID(tile));   
    switch(direction) {
      case DIRECTION_NORTH:
        break;
        
      case DIRECTION_EAST:
        glBegin(GL_QUADS);
        glColor3f(colors.x*.9,colors.y*.9,colors.z*.9);
        glTexCoord2d(0.0,3.0);
        glVertex3d(0.5,-0.5,0.0);
        glTexCoord2d(0.0,0.0);       
        glVertex3d(0.5,-0.5,3.0);
        glTexCoord2d(1.0,0.0);
        glVertex3d(0.5,0.5,3.0);
        glTexCoord2d(1.0,3.0);       
        glVertex3d(0.5,0.5,0.0);
        glEnd();
        break;
      case DIRECTION_SOUTH:
        glBegin(GL_QUADS);        
        glColor3f(colors.x*.75,colors.y*.75,colors.z*0.75);              
        glTexCoord2d(0.0,3.0);
        glVertex3d(-0.5,-0.5,0.0);
        glTexCoord2d(0.0,0.0);
        glVertex3d(-0.5,-0.5,3.0);
        glTexCoord2d(1.0,0.0);
        glVertex3d(0.5,-0.5,3.0);
        glTexCoord2d(1.0,3.0);
        glVertex3d(0.5,-0.5,0.0);
        glEnd();
        glColor3f(colors.x,colors.y,colors.z);
        break;
      case DIRECTION_WEST:
        glBegin(GL_QUADS);
        glColor3f(colors.x*.50,colors.y*.50,colors.z*.50);                     
        glTexCoord2d(0.0,3.0);
        glVertex3d(-0.5,0.5,0.0);
        glTexCoord2d(0.0,0.0);
        glVertex3d(-0.5,0.5,3.0);
        glTexCoord2d(1.0,0.0);
        glVertex3d(-0.5,-0.5,3.0);
        glTexCoord2d(1.0,3.0);
        glVertex3d(-0.5,-0.5,0.0);
        glEnd();
        glColor3f(colors.x,colors.y,colors.z);       
        break;
      default:
        break; 
    }
    CCamera::getInstance()->timeOfDay();
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
  }
  void initialize(int chunk_number) {
    /* load tile data */
    if (initialized_tile_data==true) 
      return;
    
    for (int i=0; i<chunks_high*chunks_wide; i++) {
      chunks.push_back(new CChunk(i));
    }
    initialized_tile_data=true;
  }
  /* collision detection needs work, movement needs to be less restrictive. */
  virtual bool collide(int posx, int posy) {
    
      INFO(LOG) << posx << " " << posy;
      switch(at(posx,posy)->num) {
        case TILE_CITY_WALL:
        case TILE_MOUNTAIN:
        case TILE_ICE_MOUNTAIN:
        case TILE_RIGID_MOUNTAIN:
        case TILE_WATER:
          return true;
      }
      
      return false;
  }
  void renderChunk(float posx, float posy, float pos_frac_x, float pos_frac_y, int render_tiles_view) {
    for (int j=-16; j<=16; j+=16) {
      for (int k=-16; k<=16; k+=16) {
        CChunk* chunk=getChunk(round(posx-j),round(posy-k));
        int i=0;
        while(chunk->getObject(i)) {
          if (chunk->getObject(i)->type==RENDERABLE_UNIT) {
            CUnit* unit=(CUnit*)(chunk->getObject(i));
            
            /* really bad collision detection for proof of concept, need to eliminate coupling issue through inheritance */
            /* unit manager needs to create the units, map needs to store them as cmapobjects, units created have stored map that inherit cmapobject */
            /* this allows us to fix inheritance problems, because extended cmapobjects know about map and can check collision */
            unit->AI();
            if (unit->ai_state==AI_WALKING) {
            //std::cout << "x" << u->x << " y" << u->y << std::endl;
//            if (unit->direction==DIRECTION_NORTH) 
//              if (!collide(round(unit->x),round(unit->y-0.5) ))
//                unit->move();
//            if (unit->direction==DIRECTION_EAST) 
//              if (!collide(round(unit->x+0.5),round(unit->y) ))
//                unit->move();
//            if (unit->direction==DIRECTION_SOUTH)
//              if (!collide(round(unit->x) ,round(unit->y+0.5) ))
//                unit->move();
//            if (unit->direction==DIRECTION_WEST)
//              if (!collide(round(unit->x-0.5),round(unit->y) ))
                unit->move();
            }
            chunk->getObject(i)->render(posx,posy,pos_frac_x, pos_frac_y, render_tiles_view);             
            CChunk* chunk_check=getChunk(round(unit->x),round(unit->y));
            if (chunk_check!=chunk) {
              //We're in trouble, we have to move this unit over to another chunk
              chunk->removeObject((CRenderable*)unit);
              chunk_check->addObject((CRenderable*)unit);
            }
            i++;
          } else {
            chunk->getObject(i)->render(posx,posy,pos_frac_x, pos_frac_y, render_tiles_view);
            i++;
          }
        }
      }
    }
  }
  void renderTiles(float time) {
    static float last_animation_time=0;
    static int index=0;
    const int render_tiles_view=14;
    const float delay=0.40;
    if (initialized_tile_data == false) {
      return;
    }
    glEnable(GL_BLEND);    
    if (time > last_animation_time + delay) {
      index=(index + 1) % 4;
      last_animation_time=time;
    }
    CCamera::getInstance()->timeOfDay();
    XY pos=CCamera::getInstance()->getXY();
    XY pos_frac=CCamera::getInstance()->getXY();
    pos_frac.x=  pos.x-(round(pos.x));     
    pos_frac.y=-(pos.y-(round(pos.y)));
    glPushMatrix();
    glTranslatef(-(render_tiles_view+pos_frac.x),-(render_tiles_view+pos_frac.y),0);
      /* render tiles */
//    glTranslatef(0, 40, 0);
    glPushMatrix();
    for (int i=round(pos.y)+render_tiles_view; i>=round(pos.y)-render_tiles_view; i--) {
      glPushMatrix();
      for (int j=round(pos.x)-render_tiles_view; j<=round(pos.x)+render_tiles_view; j++) {
        CTile* current=at(j,i);
//        if (!current) {
//        }
        if (current->num==TILE_WATER) {
          glCallList(Globals::tile_call[TILE_WATER+index]);
        }
        else if (current!=0) {
           if (current->num != TILE_MOUNTAIN && 
	       current->num != TILE_ICE_MOUNTAIN && 
	       current->num != TILE_RIGID_MOUNTAIN) {         
             glCallList(Globals::tile_call[current->num]);
           }
        }
        glTranslatef(1,0,0);
      }
      glPopMatrix();
      glTranslatef(0,1,0);
    }
    glPopMatrix();
    glPushMatrix();
    for (int i=round(pos.y)+render_tiles_view; i>=round(pos.y)-render_tiles_view; i--) {
      glPushMatrix();
      for (int j=round(pos.x)-render_tiles_view; j<=round(pos.x)+render_tiles_view; j++) {
        CTile* current=at(j,i);
        if (current!=0) {
          if (current->num==TILE_MOUNTAIN || 
              current->num==TILE_ICE_MOUNTAIN || 
              current->num==TILE_RIGID_MOUNTAIN ||
              current->num==TILE_CITY_WALL) {
            if (!hasTileWall(DIRECTION_SOUTH, j,i))
              renderWall(DIRECTION_SOUTH, current->num);
            if (!hasTileWall(DIRECTION_WEST, j,i))
              renderWall(DIRECTION_WEST, current->num);           
            if (!hasTileWall(DIRECTION_EAST, j,i))
              renderWall(DIRECTION_EAST, current->num);           
            renderRoof(current->num);
         } 
        }
        glTranslatef(1,0,0);
      }
      glPopMatrix();
      glTranslatef(0,1,0);
    }   
    glPopMatrix();
    
    
    renderChunk(pos.x,pos.y,pos_frac.x,pos_frac.y,render_tiles_view);
//   glPushMatrix();
//   for (int i=round(pos.y)+render_tiles_view; i>=round(pos.y)-render_tiles_view; i--) {
//      glPushMatrix();
//     for (int j=round(pos.x)-render_tiles_view; j<=round(pos.x)+render_tiles_view; j++) {
//          Tile* current=at(j,i);
//          Chunk* chunk=getChunk(round(pos.x),round(pos.y));
//          if (chunk->obj(0)->x==j && chunk->obj(0)->y==i) {
//            chunk->obj(0)->render(pos.x,pos.y,pos_frac.x,pos_frac.y, render_tiles_view);
//          }
//         glTranslatef(1,0,0);
//     } 
//     glPopMatrix();
//     glTranslatef(0,1,0);
//   }
//   glPopMatrix();
   glPopMatrix();
}

//   RenderChunk(pos.x,pos.y-16.0);
//   RenderChunk(pos.x,pos.y+16.0);  
//    Chunk* chunk=getChunk(round(pos.x),round(pos.y));
//    int i=0;
//    while(chunk->obj(i)) {
//      chunk->obj(i)->render(pos.x,pos.y,pos_frac.x,pos_frac.y);
//      i++;
//    }
     
  
  virtual void notify(Event* e) {
    if (e->type == EVENT_RENDER_FRAME) { 
      renderTiles(e->x);
    }
    if (e->type == EVENT_INITIALIZE) {
      initialize(0);
      
   }
//  std::clog << "EventID: " << e->type << " Data: " << e->a << std::endl;
//  printf("EventID");
  }
};

//static std::vector<unsigned int> tile_call;
 
//class CMapDemo : public CMap {
//public:
//  CMapDemo() : CMap(2000,2000) { 
//  } 
//  virtual void notify(Event* e) {
//    if (e->type==EVENT_RENDER_FRAME) {
//      RenderTiles(e->x);
//    }
//    if (e->type==EVENT_INITIALIZE) {
//      initialize(0);
//    }
//  }
//  static CMapDemo* getInstance() {
//    static CMapDemo* instance=0;
//    if (instance==0) {
//      instance = new CMapDemo();
//    }
//    return instance;
//  }
//};
class CWorldMap : public CMap {
public:
  CWorldMap() : CMap(2000,2000) { 
  } 
  virtual void notify(Event* e) {
    if (e->type==EVENT_RENDER_FRAME) {
      renderTiles(e->x);
    }
    if (e->type==EVENT_INITIALIZE) {
      initialize(0);
      CPlayerRenderable* player=CPlayerRenderable::getInstance();
      CTile* tile=at(player->x,player->y);
      
      getChunk(player->x,player->y)->addObject((CRenderable*)player);
      
    }
  }
  static CWorldMap* getInstance() {
    static CWorldMap* instance=0;
    if (instance==0) {
      instance = new CWorldMap();
    }
    return instance;
  }
};
#endif