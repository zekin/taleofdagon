#ifndef IRENDERER_H
#define IRENDERER_H

#include "IMap.h"

class IRenderer {
public:
    virtual void addChunk(IChunk* addedChunk) = 0;
    virtual void removeChunk(IChunk* chunkToRemove) = 0;
    virtual void flagChunkAsDirty(IChunk* dirtyChunk) = 0;
};
#endif