#ifndef IRENDERER_H
#define IRENDERER_H

#include "CRenderable.h"
#include "IMap.h"

class IRenderer : public IEventable {
public:
    IRenderer() {
    }
    virtual void render() = 0;
    virtual void update() = 0;
    virtual void addChunk(IChunk* addedChunk) = 0;
    virtual void removeChunk(IChunk* chunkToRemove) = 0;
    virtual void flagChunkAsDirty(IChunk* dirtyChunk) = 0;
};
#endif