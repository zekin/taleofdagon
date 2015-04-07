#ifndef CRENDERER_H
#define CRENDERER_H

#include "CRenderable.h"
#include "IMap.h"
#include "CLocator.h"
#include "CCamera.h"
#include "enum.h"
#include "IRenderer.h"
#include "CEventManager.h"
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
    
private:
    void fillRenderListForXY(float cameraPosX, float cameraPosY) {
        IMap* map = CLocator::getMap();
        
        if ( ! objectList.empty() ) {
            ERROR(LOG) << "Object render list is not empty!";
            return;
        }
        
        for ( int i = round(cameraPosY) + tileRenderRange; i >= round(cameraPosY) - tileRenderRange; i -= Globals::chunk_size ) {
            for ( int j = round(cameraPosX) - tileRenderRange; j <= round(cameraPosX) + tileRenderRange; j += Globals::chunk_size ) {
                CChunk* chunk = map->getChunk( round(cameraPosX), round(cameraPosY) );
                
                for ( int k=0; chunk->getObject(k); k++) {
                    objectList.push_back(chunk->getObject(k));
                }
            }
        }
        
        objectList.sort(renderSort);
    }
public:
    CRenderer() : tileRenderRange(14), waterAnimationDelay(0.30) {
        CEventManager::getInstance()->subscribe(0,this);
    }
    

    virtual void render() {
        IMap* map = CLocator::getMap();
        CCamera* camera = CCamera::getInstance();
        XY cameraPos = camera->getXY();
        XY cameraPosFrac = camera->getXY();
        
        cameraPosFrac.x =  cameraPos.x - (round(cameraPos.x));
        cameraPosFrac.y =-(cameraPos.y - (round(cameraPos.y)));       
        
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
        
        for ( int i = round(cameraPos.y) + tileRenderRange; i >= round(cameraPos.y) - tileRenderRange; i-- ) {
            glPushMatrix();
            
            for ( int j = round(cameraPos.x) - tileRenderRange; j <= round(cameraPos.x) + tileRenderRange; j++ ) {           
                CTile* tileAtLocation = map->at(j,i);
                
                if ( ! tileAtLocation )
                    continue;
                else if ( tileAtLocation->isWaterType() )
                    glCallList( Globals::tile_call[TILE_WATER] );
                else if ( ! tileAtLocation->isMountainType() )
                    glCallList( Globals::tile_call[tileAtLocation->tileType] );
                
                glTranslatef(1,0,0);
            }
            
            glPopMatrix();
            glTranslatef(0,1,0);
        }
        
        glPopMatrix();
        
        objectList.clear();
        fillRenderListForXY(cameraPos.x, cameraPos.y);
        for (std::list<CRenderable*>::iterator object = objectList.begin(); object != objectList.end(); ++object) {
            if ((*object)->type == RENDERABLE_UNIT) {
                IUnit* unitInList = (IUnit*)(*object);
                unitInList->AI();
                
                if (unitInList->isMoving())
                    unitInList->move();
                
                unitInList->render(cameraPos.x,cameraPos.y,cameraPosFrac.x,cameraPosFrac.y,tileRenderRange); 
            }
        }

        glPopMatrix();
    }
    
    virtual void update() {
        IMap* map = CLocator::getMap();
        CCamera* camera = CCamera::getInstance();
        
        if ( map == NULL ) {
            return;
        }
        
 //       XY camera->getXY();
    }
    
    virtual void addChunk(IChunk* addedChunk) {
        if ( addedChunk == NULL ) {
            ERROR(LOG) << "No passed to renderer. Is this chunk deallocated?";
            return;
        }
        
        chunkList.push_back(addedChunk);
    }
    
    virtual void removeChunk(IChunk* chunkToRemove) {
        for ( int i = 0; i < chunkList.size(); i++ ) { 
            if ( chunkToRemove == chunkList[i] ) {
//                chunkList.erase(i);
            }
        }
    }
    
    virtual void flagChunkAsDirty(IChunk* dirtyChunk) {
    }
    
    virtual void notify(Event* event) {
        switch( event->type ) {
            case EVENT_RENDER_FRAME:
//                update();
                render();
                break;
        }
    }
};

#endif