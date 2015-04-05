#ifndef CRENDERER_H
#define CRENDERER_H

#include "CRenderable.h"
#include "IMap.h"

class CRenderer {
    std::vector<IChunk*> chunkList;
public:
    CRenderer() {
    }
    virtual void addChunk(IChunk* addedChunk) {
    }
    virtual void removeChunk(IChunk* chunkToRemove) {
    }
    virtual void flagChunkAsDirty(IChunk* dirtyChunk) {
    }
};

#endif