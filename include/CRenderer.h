#ifndef CRENDERER_H
#define CRENDERER_H

#include "CRenderable.h"
#include "IMap.h"
#include "CLocator.h"
#include "CCamera.h"
#include "enum.h"
#include "IRenderer.h"
#include "CEventManager.h"
#include "IResource.h"
#include "CTile.h"
#include <list>


bool renderSort(CRenderable* first, CRenderable* second) {
    if (first->y > second->y) {
        return false;
    } else {
        return true;   
    }
}
class CRenderer : public IRenderer { 
    std::vector<IChunk*> chunkList;
    std::list<CRenderable*> objectList;
//    std::list<CRenderable*> render;
    int tileRenderRange;
    float waterAnimationDelay;
    float lastWaterAnimationTime;
    int waterIndex;
    
    CMapSheet* map_tiles_resource;
    CSpriteSheet* map_walls_resource;
    SDL_Surface* map_tiles_surface;
    
private:
    void fillRenderListForXY(float cameraPosX, float cameraPosY) {
        IMap* map = CLocator::getMap();
        int chunkSize = Globals::chunk_size;
        
        if ( ! objectList.empty() ) {
            ERROR(LOG) << "Object render list is not empty!";
            return;
        }
        
        /* Grab objects 3 x 3 chunks around camera. */
        for ( int i = round(cameraPosY) + chunkSize; i >= round(cameraPosY) - chunkSize; i -= chunkSize ) {
            for ( int j = round(cameraPosX) - chunkSize; j <= round(cameraPosX) + chunkSize; j += chunkSize) {
                CChunk* chunk = map->getChunk( round(j), round(i) );
                
                for ( int k=0; chunk->getObject(k); k++) {
                    objectList.push_back(chunk->getObject(k));
                }
            }
        }
    }
    
    void initializeTileCalls() {
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
    }
    
public:
    CRenderer() : tileRenderRange(14), waterAnimationDelay(0.30), waterIndex(0) {
        CResourceManager* resources = CResourceManager::getInstance();
        CEventManager::getInstance()->subscribe(0,this);
        map_tiles_resource = resources->getMapSheet("./graphics/tile3.png",16,16);
        map_walls_resource = resources->getSpriteSheet("./graphics/tile3.png",16,16);
        initializeTileCalls();
    }
    

    virtual void render() {
        IMap* map = CLocator::getMap();
        CCamera* camera = CCamera::getInstance();
        float currentWaterAnimationTime = CClock::getInstance()->getTime();
        
        XY cameraPos = camera->getXY();
        XY cameraPosFrac = camera->getXY();
        
        cameraPosFrac.x =  cameraPos.x - (round(cameraPos.x));
        cameraPosFrac.y =-(cameraPos.y - (round(cameraPos.y)));       
        
        if ( currentWaterAnimationTime > lastWaterAnimationTime + waterAnimationDelay ) {
            waterIndex = (waterIndex + 1) % 4;
            lastWaterAnimationTime = currentWaterAnimationTime;
        }
        
        if ( map == NULL ) {
            INFO(LOG) << "Map not set yet.";
            return;
        }
        
        if ( ! map->isInitialized() ) {
            INFO(LOG) << "Map not initialized yet.";
            return;
        }
        
        camera->timeOfDay();
        
        glEnable(GL_BLEND);
        glPushMatrix();
        glTranslatef( -(tileRenderRange+cameraPosFrac.x), -(tileRenderRange+cameraPosFrac.y), 0);
        glPushMatrix();
        /* Render Tiles */
        
        for ( int i = round(cameraPos.y) + tileRenderRange; i >= round(cameraPos.y) - tileRenderRange; i-- ) {
            glPushMatrix();
            
            for ( int j = round(cameraPos.x) - tileRenderRange; j <= round(cameraPos.x) + tileRenderRange; j++ ) {           
                CTile* tileAtLocation = map->at(j,i);
                
                if ( ! tileAtLocation )
                    continue;
                else if ( tileAtLocation->isWaterType() )
                    glCallList( Globals::tile_call[TILE_WATER + waterIndex] );
                else if ( ! tileAtLocation->isMountainType() )
                    tileAtLocation->render();
                
                glTranslatef(1,0,0);
            }
            
            glPopMatrix();
            glTranslatef(0,1,0);
        }
        
        glPopMatrix();
        
        /* Render Mountains */
        glPushMatrix();
        for ( int i = round(cameraPos.y) + tileRenderRange; i >= round(cameraPos.y) - tileRenderRange; i-- ) {
            glPushMatrix();
            
            for ( int j = round(cameraPos.x) - tileRenderRange; j <= round(cameraPos.x) + tileRenderRange; j++ ) {
                CTile* tileAtLocation=map->at(j,i);
                
                if ( tileAtLocation != 0 ) {
                    
                    tileAtLocation->renderRoof();
//                    if ( tileAtLocation->isWallType() ) {
//                        if (!map->hasTileWall(DIRECTION_SOUTH, j, i))
//                            map->renderWall(DIRECTION_SOUTH, tileAtLocation->getTileType() );
//                        if (!map->hasTileWall(DIRECTION_WEST, j, i))
//                            map->renderWall(DIRECTION_WEST, tileAtLocation->getTileType() );
//                        if (!map->hasTileWall(DIRECTION_EAST, j, i))
//                            map->renderWall(DIRECTION_EAST, tileAtLocation->getTileType() );
//                        
//                        map->renderRoof( tileAtLocation->getTileType() );
//                    }
                }
                glTranslatef(1,0,0);
            }
            glPopMatrix();
            glTranslatef(0,1,0);
        }
        glPopMatrix();       
        
        /* Render Units */
        for (std::list<CRenderable*>::iterator object = objectList.begin(); object != objectList.end(); ++object) {
            (*object)->render(cameraPos.x,cameraPos.y,cameraPosFrac.x,cameraPosFrac.y,tileRenderRange); 
        }
        glPopMatrix();
    }
    
    virtual void update() {
        IMap* map = CLocator::getMap();
        CCamera* camera = CCamera::getInstance();
        XY cameraPos = camera->getXY();
        
        if ( map == NULL ) 
            return;
        
        
        objectList.clear();
        fillRenderListForXY(cameraPos.x, cameraPos.y);
        
        for (std::list<CRenderable*>::iterator object = objectList.begin(); object != objectList.end(); ++object) {
            if ((*object)->type == RENDERABLE_UNIT) {
                IUnit* unitInList = (IUnit*)(*object);
                CChunk* currentChunk = NULL;
                CChunk* chunkToCheckAgainst = NULL;
                
                unitInList->AI();
                
                if ( ! unitInList->isMoving())
                    continue;
                
                currentChunk = map->getChunk( round(unitInList->x), round(unitInList->y) );
                unitInList->move();
                chunkToCheckAgainst = map->getChunk( round(unitInList->x), round(unitInList->y) );
                
                /* If unit is moving, check if the chunk needs to change. */
                if ( chunkToCheckAgainst != currentChunk ) {
                    currentChunk->removeObject( (CRenderable*) unitInList );
                    chunkToCheckAgainst->addObject( (CRenderable*) unitInList );
                }
                
            }
        }
        
        objectList.sort(renderSort);
    }
    
    virtual CMapSheet* getMapSheet() {
        return map_tiles_resource;
    }
    virtual CSpriteSheet* getWallSheet() {
        return map_walls_resource;
    }
    
    virtual void notify(Event* event) {
        switch( event->type ) {
            case EVENT_RENDER_FRAME:
                update();
                render();
                break;
        }
    }
    virtual bool isIndoors() {
        CCamera* camera = CCamera::getInstance();
        IMap* map = CLocator::getMap();
        XY position = camera->getXY();
        
        CTile* tile = map->at(position.x,position.y);
        
        if ( tile->isIndoors() ) 
            return true;
        else
            return false;
            
    }
};

#endif