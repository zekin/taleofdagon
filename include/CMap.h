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
protected:
    int width;
    int height;
    bool initialized_tile_data;
    int chunks_wide;
    int chunks_high;
    std::vector<CChunk*> chunks;
    unsigned int texture_tiles[20];
    CMapSheet* map_tiles_resource;
    CSpriteSheet* map_walls_resource;
    SDL_Surface* map_tiles_surface;
    
public:
    CMap(int width, int height) :
        width(width),
        height(height),
        map_tiles_surface(0),
        map_tiles_resource(0),
        initialized_tile_data(false),
        chunks_wide(0),
        chunks_high(0) {
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
            initializeTileCalls();
        }
    }
    
    virtual void initializeTileCalls() {
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
    
    virtual CTile* at(int x, int y) {
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
        int chunkIndex=0;
        
        if ( x < 0 )
            x = width + x % width;
        else
            x = x % width;
        
        if ( y < 0 )
            y = height + y % height;
        else
            y = y % height;

        chunkIndex = ( y / Globals::chunk_size ) * chunks_wide + ( x / Globals::chunk_size );
        return chunks.at( chunkIndex );
    }

    /* TODO: needs refactoring */
    virtual bool hasTileWall(int direction, int x, int y) {
        CTile* srcTile = at(x,y);
        CTile* destTile = NULL;
        
        int coordinateToCheckX = x;
        int coordinateToCheckY = y;
        
        if ( ! srcTile )
            return false;
        
        switch ( direction ) {
            case DIRECTION_NORTH:
                coordinateToCheckY -= 1;
                break;
            case DIRECTION_EAST:
                coordinateToCheckX += 1;
                break;
            case DIRECTION_SOUTH:
                coordinateToCheckY += 1;
                break;
            case DIRECTION_WEST:
                coordinateToCheckX -= 1;
                break;
        }
        
        destTile = at(coordinateToCheckX,coordinateToCheckY);
        
        if ( ! destTile )
            return true;
            
        if ( destTile->isWallType() )
            return true;
        
        return false;
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
        XYZ colors( 0, 0, 0 );
        colors = CCamera::getInstance()->timeOfDay(true);
        
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
    
    virtual void initialize(int chunk_number) {
        /* load tile data */
        if (initialized_tile_data==true)
            return;

        for (int i=0; i<chunks_high*chunks_wide; i++) {
            chunks.push_back(new CChunk(i));
        }
        initialized_tile_data=true;
    }
    virtual bool isInitialized() {
        return initialized_tile_data;
    }
    
    /* collision detection needs work, movement needs to be less restrictive. */
    virtual bool collide(IUnit* srcUnit, int posx, int posy) {
        CTile * tileToCheck = at(posx, posy);
        CChunk* chunk = NULL;
        IUnit* unitInChunk = NULL;
        
        if ( tileToCheck->isCollidable() ) 
            return true;
        
        chunk = getChunk(posx, posy);
        unitInChunk = chunk->getUnit(0);

        for ( int i = 0; unitInChunk; i++, unitInChunk = chunk->getUnit(i) ) {
            if ( round(unitInChunk->x) == posx && round(unitInChunk->y) == posy && srcUnit != unitInChunk) 
                return true;
        }

        return false;
    }
    
    virtual void renderChunk(float posx, float posy, float pos_frac_x, float pos_frac_y, int render_tiles_view) {
        for (int j=-16; j<=16; j+=16) {
            for (int k=-16; k<=16; k+=16) {
                CChunk* currentChunk=getChunk(round(posx-j),round(posy-k));
                int i=0;
                
                while ( currentChunk->getObject(i) ) {
                    if ( currentChunk->getObject(i)->type==RENDERABLE_UNIT ) {
                        CUnit* unit=(CUnit*)(currentChunk->getObject(i));
                        unit->AI();
                        
                        if ( unit->ai_state==AI_WALKING ) {
                            unit->move();
                        }
                        
                        currentChunk->getObject(i)->render(posx,posy,pos_frac_x, pos_frac_y, render_tiles_view);
                        CChunk* chunkToCheckAgainst=getChunk( round(unit->x), round(unit->y) );
                        
                        if ( chunkToCheckAgainst != currentChunk ) {
                            //We're in trouble, we have to move this unit over to another chunk
                            currentChunk->removeObject( (CRenderable*)unit );
                            chunkToCheckAgainst->addObject( (CRenderable*)unit );
                        }
                        
                        i++;
                    } else {
                        currentChunk->getObject(i)->render(posx,posy,pos_frac_x, pos_frac_y, render_tiles_view);
                        
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
        const float delay=0.30;
        
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
        
        pos_frac.x=  pos.x - (round(pos.x));
        pos_frac.y=-(pos.y - (round(pos.y)));
        
        glPushMatrix();
        glTranslatef( -(render_tiles_view+pos_frac.x), -(render_tiles_view+pos_frac.y), 0);
        glPushMatrix();
        
        for ( int i = round(pos.y) + render_tiles_view; i >= round(pos.y) - render_tiles_view; i-- ) {
            glPushMatrix();
            
            for ( int j = round(pos.x) - render_tiles_view; j <= round(pos.x) + render_tiles_view; j++ ) {
                CTile* tileAtLocation = at(j,i);
                
                if ( tileAtLocation->isWaterType() ) {
                    glCallList( Globals::tile_call[TILE_WATER+index] );
                } else if ( tileAtLocation ) {
                    if ( ! tileAtLocation->isMountainType() ) {
                        glCallList( Globals::tile_call[tileAtLocation->tileType] );
                    }
                }
                glTranslatef(1,0,0);
            }
            glPopMatrix();
            glTranslatef(0,1,0);
        }
        
        glPopMatrix();
        glPushMatrix();
        
        for ( int i = round(pos.y) + render_tiles_view; i >= round(pos.y) - render_tiles_view; i-- ) {
            glPushMatrix();
            
            for ( int j = round(pos.x) - render_tiles_view; j <= round(pos.x) + render_tiles_view; j++ ) {
                CTile* current=at(j,i);
                
                if (current!=0) {
                    if ( current->isWallType() ) {
                        if (!hasTileWall(DIRECTION_SOUTH, j,i))
                            renderWall(DIRECTION_SOUTH, current->tileType);
                        if (!hasTileWall(DIRECTION_WEST, j,i))
                            renderWall(DIRECTION_WEST, current->tileType);
                        if (!hasTileWall(DIRECTION_EAST, j,i))
                            renderWall(DIRECTION_EAST, current->tileType);
                        renderRoof(current->tileType);
                    }
                }
                glTranslatef(1,0,0);
            }
            glPopMatrix();
            glTranslatef(0,1,0);
        }
        
        glPopMatrix();
        renderChunk(pos.x,pos.y,pos_frac.x,pos_frac.y,render_tiles_view);
        glPopMatrix();
    }

    virtual void notify(Event* e) {
        if (e->type == EVENT_RENDER_FRAME) {
//            renderTiles(e->x);
        }
        if (e->type == EVENT_INITIALIZE) {
            initialize(0);
        }
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
//            renderTiles(e->x);
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
