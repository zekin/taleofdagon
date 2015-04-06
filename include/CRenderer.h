#ifndef CRENDERER_H
#define CRENDERER_H

#include "CRenderable.h"
#include "IMap.h"
#include "CLocator.h"
#include "CCamera.h"

class CRenderer {
    std::vector<IChunk*> chunkList;
    std::list<CRenderable*> render;
    
public:
    CRenderer() {
    }
    
    virtual void render() {
        
    }
    
    virtual void update() {
        IMap* map = CLocator::getMap();
        CCamera* camera = CCamera::getInstance();
        
        
    }
    
    virtual void addChunk(IChunk* addedChunk) {
        if ( addedChunk == NULL ) {
            INFO(ERROR) << "No passed to renderer. Is this chunk deallocated?";
            return;
        }
        
        chunkList.push_back(addedChunk);
    }
    
    virtual void removeChunk(IChunk* chunkToRemove) {
        for ( int i = 0; i < chunkList.size(); i++ ) { 
            if ( chunkToRemove == chunkList[i] ) {
                chunkList.erase(i);
            }
        }
    }
    
    virtual void flagChunkAsDirty(IChunk* dirtyChunk) {
    }
};

#endif