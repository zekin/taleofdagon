#ifndef IRENDERER_H
#define IRENDERER_H

#include "CRenderable.h"
#include "IMap.h"

class CMapSheet;
class CSpriteSheet;

class IRenderer : public IEventable {
public:
    IRenderer() {
    }
    virtual void render() = 0;
    virtual void update() = 0;
    virtual bool isIndoors() = 0;
    virtual CMapSheet* getMapSheet() = 0;
    virtual CSpriteSheet* getWallSheet() = 0;
};
#endif