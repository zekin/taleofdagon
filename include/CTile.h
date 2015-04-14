#ifndef CTILE_H
#define CTILE_H
#include "enum.h"
#include "CLocator.h"
#include "CCamera.h"
#include <IRenderer.h>
#include <SDL/SDL_opengl.h>

class CTile {
protected:
    char tileType;
    char area;
public:
    CTile(char tile_number, char area=AREA_GRASSLANDS) : tileType(tile_number), area(area) {}
    
    char getTileType() {
        return tileType;
    }
    
    char getAreaType() {
        return area;
    }

    bool isWaterType() {
        if ( tileType == TILE_WATER )
            return true;
        else
            return false;
    }
    
    bool isMountainType() {
      if ( tileType == TILE_MOUNTAIN || 
           tileType == TILE_ICE_MOUNTAIN || 
           tileType == TILE_RIGID_MOUNTAIN )
          return true;
      else
          return false;
    }
    
    bool isWallType() {
      if (tileType == TILE_MOUNTAIN || 
          tileType == TILE_ICE_MOUNTAIN || 
          tileType == TILE_RIGID_MOUNTAIN || 
          tileType == TILE_CITY_WALL ||
          tileType == TILE_CITY_HOUSE_WALL)
          return true;
      else
          return false;
    }
    bool isIndoors() {
      if (tileType == TILE_CITY_HOUSE_FLOOR)
          return true;
      else
          return false;
    }   
    bool isCollidable() {
      if (tileType == TILE_MOUNTAIN || 
          tileType == TILE_ICE_MOUNTAIN || 
          tileType == TILE_RIGID_MOUNTAIN || 
          tileType == TILE_CITY_WALL || 
          tileType == TILE_WATER ||
          tileType == TILE_CITY_HOUSE_WALL)
          return true;
      else
          return false;       
    }
    virtual void render() {
        glCallList( Globals::tile_call[tileType] );
    }
    virtual void renderRoof() {
        
    }
};

class CTileShopFloor : public CTile {
public:
   CTileShopFloor() : CTile( TILE_CITY_HOUSE_FLOOR, AREA_CITY ) {
   }   
   
   void render() {
       glCallList( Globals::tile_call[TILE_CITY_HOUSE_FLOOR] );
   }
   
   void renderRoof() {
       IRenderer* renderer = CLocator::getRenderer();
       GLuint textureId = renderer->getMapSheet()->getTextureID(TILE_CITY_HOUSE_WALL2);
       XYZ colors( 0, 0, 0 );
       colors = CCamera::getInstance()->timeOfDay(true);      
       glColor3f(colors.x,colors.y,colors.z);      
       
       if ( renderer->isIndoors() ) 
           return;
       
       glEnable(GL_DEPTH_TEST);
       glDisable(GL_BLEND);

       glBindTexture( GL_TEXTURE_2D, textureId );
       glBegin(GL_QUADS);
       glTexCoord2d(0.0,0.5);
       glVertex3d(-0.5,-0.5,1.9);
       glTexCoord2d(0.0,0.0);
       glVertex3d(-0.5,0.5,1.9);
       glTexCoord2d(0.5,0.0);
       glVertex3d(0.5,0.5,1.9);
       glTexCoord2d(0.5,0.5);
       glVertex3d(0.5,-0.5,1.9);
       glEnd();
       glDisable(GL_DEPTH_TEST);
       glEnable(GL_BLEND);
   }
};
class CTileShopWall : public CTile {
public:
   CTileShopWall() : CTile( TILE_CITY_HOUSE_WALL, AREA_CITY ) {
   }   
   
   void render() {
       glCallList( Globals::tile_call[TILE_CITY_GROUND] );
   }
   
   void renderRoof() {
       IRenderer* renderer = CLocator::getRenderer();
       GLuint textureId = renderer->getMapSheet()->getTextureID(TILE_CITY_HOUSE_WALL3);
       XYZ colors( 0, 0, 0 );
       colors = CCamera::getInstance()->timeOfDay(true);      
       
       glEnable(GL_DEPTH_TEST);
       glDisable(GL_BLEND);

       glBindTexture( GL_TEXTURE_2D, textureId );
       glBegin(GL_QUADS);
       glColor3f(colors.x,colors.y,colors.z);
       glTexCoord2d(0.0,0.5);
       glVertex3d(-0.5,-0.5,2.0);
       glTexCoord2d(0.0,0.0);
       glVertex3d(-0.5,0.5,2.0);
       glTexCoord2d(0.5,0.0);
       glVertex3d(0.5,0.5,2.0);
       glTexCoord2d(0.5,0.5);
       glVertex3d(0.5,-0.5,2.0);
       
       glColor3f(colors.x*.9,colors.y*.9,colors.z*.9);
       glTexCoord2d(0.0,1.5);
       glVertex3d(0.5,-0.5,0.0);
       glTexCoord2d(0.0,0.0);
       glVertex3d(0.5,-0.5,2.0);
       glTexCoord2d(0.5,0.0);
       glVertex3d(0.5,0.5,2.0);
       glTexCoord2d(0.5,1.5);
       glVertex3d(0.5,0.5,0.0);
       
       glColor3f(colors.x*.75,colors.y*.75,colors.z*0.75);
       glTexCoord2d(0.0,1.5);
       glVertex3d(-0.5,-0.5,0.0);
       glTexCoord2d(0.0,0.0);
       glVertex3d(-0.5,-0.5,2.0);
       glTexCoord2d(0.5,0.0);
       glVertex3d(0.5,-0.5,2.0);
       glTexCoord2d(0.5,1.5);
       glVertex3d(0.5,-0.5,0.0);
       
       glColor3f(colors.x*.50,colors.y*.50,colors.z*.50);
       glTexCoord2d(0.0,1.5);
       glVertex3d(-0.5,0.5,0.0);
       glTexCoord2d(0.0,0.0);
       glVertex3d(-0.5,0.5,2.0);
       glTexCoord2d(0.5,0.0);
       glVertex3d(-0.5,-0.5,2.0);
       glTexCoord2d(0.5,1.5);
       glVertex3d(-0.5,-0.5,0.0);
       
       glEnd();
       glDisable(GL_DEPTH_TEST);
       glEnable(GL_BLEND);
   }
};
class CTileCityWall : public CTile {
public:
   CTileCityWall() : CTile( TILE_CITY_WALL, AREA_CITY ) {
   }   
   
   void render() {
       glCallList( Globals::tile_call[TILE_CITY_WALL] );
   }
   
   void renderRoof() {
       IRenderer* renderer = CLocator::getRenderer();
       GLuint textureId = renderer->getMapSheet()->getTextureID(TILE_CITY_WALL);
       XYZ colors( 0, 0, 0 );
       colors = CCamera::getInstance()->timeOfDay(true);
        
//       if ( renderer->isIndoors() ) 
//           return;
       
       glEnable(GL_DEPTH_TEST);
       glDisable(GL_BLEND);

       glBindTexture( GL_TEXTURE_2D, textureId );
       glBegin(GL_QUADS);
       
       glColor3f(colors.x,colors.y,colors.z);
       glTexCoord2d(0.0,0.5);
       glVertex3d(-0.5,-0.5,3.0);
       glTexCoord2d(0.0,0.0);
       glVertex3d(-0.5,0.5,3.0);
       glTexCoord2d(0.5,0.0);
       glVertex3d(0.5,0.5,3.0);
       glTexCoord2d(0.5,0.5);
       glVertex3d(0.5,-0.5,3.0);
       
       glColor3f(colors.x*.9,colors.y*.9,colors.z*.9);
       glTexCoord2d(0.0,1.5);
       glVertex3d(0.5,-0.5,0.0);
       glTexCoord2d(0.0,0.0);
       glVertex3d(0.5,-0.5,3.0);
       glTexCoord2d(0.5,0.0);
       glVertex3d(0.5,0.5,3.0);
       glTexCoord2d(0.5,1.5);
       glVertex3d(0.5,0.5,0.0);
       
       glColor3f(colors.x*.75,colors.y*.75,colors.z*0.75);
       glTexCoord2d(0.0,1.5);
       glVertex3d(-0.5,-0.5,0.0);
       glTexCoord2d(0.0,0.0);
       glVertex3d(-0.5,-0.5,3.0);
       glTexCoord2d(0.5,0.0);
       glVertex3d(0.5,-0.5,3.0);
       glTexCoord2d(0.5,1.5);
       glVertex3d(0.5,-0.5,0.0);
       
       glColor3f(colors.x*.50,colors.y*.50,colors.z*.50);
       glTexCoord2d(0.0,1.5);
       glVertex3d(-0.5,0.5,0.0);
       glTexCoord2d(0.0,0.0);
       glVertex3d(-0.5,0.5,3.0);
       glTexCoord2d(0.5,0.0);
       glVertex3d(-0.5,-0.5,3.0);
       glTexCoord2d(0.5,1.5);
       glVertex3d(-0.5,-0.5,0.0);
       glEnd();
       glDisable(GL_DEPTH_TEST);
       glEnable(GL_BLEND);
   }
};
#endif