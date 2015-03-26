#ifndef CMAP_H
#define CMAP_H
#include <math.h>
#include <iomanip>
#include "CEventManager.h"
#include "enum.h"
#include "IResource.h"
#include "CCamera.h"
#include "IUnit.h"
#include "zekin/noise.h"
#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "logging.h"

struct Tile {
  char num;
  char area;
  Tile(char tile_number, char area=AREA_GRASSLANDS) : num(tile_number), area(area) {}
  CLASSIFIER getClassifier() {
    switch(num) {
      case TILE_GRASS:
      case TILE_SAND:
        return GROUND;
      case TILE_WATER:
        return SEA;
      case TILE_MOUNTAIN:
        return CLIFF;
    }
  }
};

struct Chunk {
private:
  std::vector<Tile*> tiles;
  std::vector<CRenderable*> objects;
  bool initialized;
  int chunk_number;
public:
  Chunk(int chunk_number) : 
  initialized(false), 
  chunk_number(chunk_number) {}
  
  Tile* at(int x, int y) {
    if (!initialized) {
      setTiles(x,y);
    }
    x=x%Globals::chunk_size;
    y=y%Globals::chunk_size;
    return tiles[y*Globals::chunk_size+x];   
  }
  CRenderable* obj(int number) {
    if (number >= objects.size()) {
      return 0;
    } else {
      return objects.at(number);
    }
  }
  void addObject(CRenderable* object) {
    for (int i=0; i<objects.size(); i++) {
      if (object==objects[i]) {
        INFO(LOG) << "Object already registered inside chunk number " << chunk_number;
        return;
      }
    }
    INFO(LOG) << "Object added to chunk number " << chunk_number;
    objects.push_back(object);
  }
  void removeObject(CRenderable* object) {
    for (int i=0; i<objects.size(); i++) {
      if (object==objects[i]) {
        INFO(LOG) << "Object removed from chunk number " << chunk_number;
        objects.erase(objects.begin()+i);
      }
    }
  }
  void setTiles(int x, int y) {
    /* a create chunk function */
    int offsetx=x%Globals::chunk_size;
    int offsety=y%Globals::chunk_size;
    for (int i=y-offsety; i<y-offsety+Globals::chunk_size; i++) {
      for (int j=x-offsetx; j<x-offsetx+Globals::chunk_size; j++) {
        TileInstantiation tile_properties=Globals::getTileAt(2000,2000,j,i);
        if (tile_properties.object_type)
          addObject(new CMapObjectRenderable(tile_properties.object_type,j,i,0));
        if (tile_properties.unit_type) {
			/* cheap way of instantiating units for now, i think we should sent off a CREATE_UNIT message and register them with the unit manager */
          if (tile_properties.unit_type==UNIT_TYPE_VILLAGER)
            addObject(new CUnitVillager(j,i,0));
          if (tile_properties.unit_type==UNIT_TYPE_FIGHTER)
            addObject(new CUnitFighter(j,i,0));
          if (tile_properties.unit_type==UNIT_TYPE_KNIGHT)
            addObject(new CUnitKnight(j,i,0));           
        }
        this->tiles.push_back(new Tile(tile_properties.tile_number,tile_properties.area_type));
      }
    }
    INFO(LOG) << "Creating chunk " << chunk_number;   
    initialized=true;
  }
  bool isInitialized() {
    return initialized;
  }
  void setInitialized() {
    initialized=true;
  }
 
};

struct WallInformation {
  float height;
  int tile_type;
  WallInformation(float height, int tile_type) {}
};

class CMap : public IEventable {
private:
  int width;
  int height;
  bool initialized_tile_data;
  int chunks_wide;
  int chunks_high;
//  std::vector<Tile> tiles;
  std::vector<Chunk*> chunks;
//  std::vector<Region> regions;
//  std::vector<unsigned int> tile_call;
  unsigned int texture_tiles[20];
  CMapSheet* map_tiles_resource;
  CSpriteSheet* map_walls_resource;
  SDL_Surface* map_tiles_surface;
  
public:
  void initialize_tile_calls() {
    for (int i=0; i<29; i++) {
      Globals::tile_call.push_back(glGenLists(1));
      glNewList(Globals::tile_call[i],GL_COMPILE);
      glBindTexture(GL_TEXTURE_2D, map_tiles_resource->getTextureID(i));
      glBegin(GL_QUADS);
      if (i==TILE_ICE_MOUNTAIN || i==TILE_RIGID_MOUNTAIN || i==TILE_MOUNTAIN) {
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
     }
      glEnd();
      glEndList();
    }
     
     Globals::tile_call_initialized=true;
  }
  CMap(int width, int height) : width(width), height(height), map_tiles_surface(0), map_tiles_resource(0), initialized_tile_data(false), chunks_wide(0), chunks_high(0) { 
//    CEventManager::getInstance()->subscribe(1,this);
    /* load image for tiles */
    if (width%Globals::chunk_size>0) {
      this->width=(width/Globals::chunk_size)*Globals::chunk_size; // or modulus
      INFO(LOG) << "Invalid width specified, not a multiple of chunksize(" << Globals::chunk_size << "), reverting to floor of nearest:" << width;
    } 
    if (height%Globals::chunk_size>0) {
      this->height=(height/Globals::chunk_size)*Globals::chunk_size;
      INFO(LOG) << "Invalid height specified, not a multiple of chunksize(" << Globals::chunk_size << "), reverting to floor of nearest:" << height;
    }
    chunks_wide=width/Globals::chunk_size;
    chunks_high=height/Globals::chunk_size;
    map_tiles_resource=CResourceManager::getInstance()->getMapSheet("./graphics/tile3.png",16,16);
    map_walls_resource=CResourceManager::getInstance()->getSpriteSheet("./graphics/tile3.png",16,16);   
    if (Globals::tile_call_initialized==false) {
      initialize_tile_calls();
    }
  } 
  Tile* at(int x, int y) {
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
  Chunk* getChunk(int x, int y) {
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
    Tile* src_tile=at(x,y);
    Tile* dest_tile;
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
        /* laggy solution, too much, we'll use that short circuited if statement for now even though its ugly and more work to maintain */
//        for (int i=0; i<wall_info.size(); i++) {
//          if (dest_tile->num == wall_info[i].tile_type) {
//            return 1;
//          } else {
//            return 0;
//          }
//        }
        break;
      case DIRECTION_EAST:
        dest_tile=at(x+1,y);
         if (dest_tile->num == TILE_MOUNTAIN ||
            dest_tile->num == TILE_ICE_MOUNTAIN ||
            dest_tile->num == TILE_RIGID_MOUNTAIN ||
            dest_tile->num == TILE_CITY_WALL)
           return 1;
         else return 0;
        
//        for (int i=0; i<wall_info.size(); i++) {
//          if (dest_tile->num == wall_info[i].tile_type) {
//            return 1;
//          } else {
//            return 0;
//          }
//        }       
        break;
     case DIRECTION_SOUTH:
        dest_tile=at(x,y+1);
         if (dest_tile->num == TILE_MOUNTAIN ||
            dest_tile->num == TILE_ICE_MOUNTAIN ||
            dest_tile->num == TILE_RIGID_MOUNTAIN ||
            dest_tile->num == TILE_CITY_WALL)
           return 1;
         else return 0;
        
//        for (int i=0; i<wall_info.size(); i++) {
//          if (dest_tile->num == wall_info[i].tile_type) {
//            return 1;
//          } else {
//            return 0;
//          }
//        }       
        break;
     case DIRECTION_WEST:
        dest_tile=at(x-1,y);
         if (dest_tile->num == TILE_MOUNTAIN ||
            dest_tile->num == TILE_ICE_MOUNTAIN ||
            dest_tile->num == TILE_RIGID_MOUNTAIN ||
            dest_tile->num == TILE_CITY_WALL)
           return 1;
         else return 0;
        
//        for (int i=0; i<wall_info.size(); i++) {
//          if (dest_tile->num == wall_info[i].tile_type) {
//            return 1;
//          } else {
//            return 0;
//          }
//        }       
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
      chunks.push_back(new Chunk(i));
//      std::clog << "I'm pushing back chunk " << i << std::endl;
    }
      
//    if (initialized_tile_data==true)
//      return;
    
//    const double radius= (width > height) ? height/2.0 : width/2.0;
//    const double x=width/2;
//    const double y=height/2;
    
//    for (int i=0; i<height; i++) {
//      for (int j=0; j<width; j++) {
//        double val=Globals::generateIslandHeight(radius,j, i, 0.65, 0.35, 0.09, 3.4, 5);
//        if (0.0 <= val && val <= 0.48)
//          tiles.push_back(Tile(TILE_WATER));
//        else if (0.45 < val && val <= 0.53)
//          tiles.push_back(Tile(TILE_SAND));
//        else if (0.5 < val && val <= 0.7)
//          tiles.push_back(Tile(TILE_GRASS));
//        else if (0.8 < val && val <= 1.0)
//          tiles.push_back(Tile(TILE_MOUNTAIN));
//        else
//          tiles.push_back(Tile(TILE_MOUNTAIN));
//      }
//    }
    initialized_tile_data=true;
  }
  /* collision detection needs work, movement needs to be less restrictive. */
  bool collide(int posx, int posy) {
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
        Chunk* chunk=getChunk(round(posx-j),round(posy-k));
        int i=0;
        while(chunk->obj(i)) {
          if (chunk->obj(i)->type==RENDERABLE_UNIT) {
            CUnitRenderable* unit=(CUnitRenderable*)(chunk->obj(i));
            
            /* really bad collision detection for proof of concept, need to eliminate coupling issue through inheritance */
			/* unit manager needs to create the units, map needs to store them as cmapobjects, units created have stored map that inherit cmapobject */
			/* this allows us to fix inheritance problems, because extended cmapobjects know about map and can check collision */
			/* not enough time to fix this, this is what happens when you slack off and write bad OO at the last minute */
            unit->AI();
            if (unit->ai_state==AI_WALKING) {
//				std::cout << "x" << u->x << " y" << u->y << std::endl;
            if (unit->direction==DIRECTION_NORTH) 
              if (!collide(round(unit->x),round(unit->y-0.5) ))
                unit->Move();
            if (unit->direction==DIRECTION_EAST) 
              if (!collide(round(unit->x+0.5),round(unit->y) ))
                unit->Move();
            if (unit->direction==DIRECTION_SOUTH)
              if (!collide(round(unit->x) ,round(unit->y+0.5) ))
                unit->Move();
            if (unit->direction==DIRECTION_WEST)
              if (!collide(round(unit->x-0.5),round(unit->y) ))
                unit->Move();
            }
            chunk->obj(i)->render(posx,posy,pos_frac_x, pos_frac_y, render_tiles_view);             
            Chunk* chunk_check=getChunk(round(unit->x),round(unit->y));
            if (chunk_check!=chunk) {
              //We're in trouble, we have to move this unit over to another chunk
              chunk->removeObject((CRenderable*)unit);
              chunk_check->addObject((CRenderable*)unit);
            }
            i++;
          
          } else {
	    chunk->obj(i)->render(posx,posy,pos_frac_x, pos_frac_y, render_tiles_view);
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
        Tile* current=at(j,i);
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
        Tile* current=at(j,i);
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
      Tile* tile=at(player->x,player->y);
      
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