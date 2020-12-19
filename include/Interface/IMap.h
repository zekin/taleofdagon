#ifndef IMAP_H
#define IMAP_H

#include "enum.h"
#include "CEventManager.h"
#include "CRenderable.h"
#include "CUnit.h"
#include "IChunk.h"
#include "CChunk.h"
#include "IUnitFactory.h"
#include "ITileFactory.h"
#include "CLocator.h"
#include "CTile.h"
#include "IMapGenerator.h"
#include "constants.h"

class CTile;

class IMap : public IEventable {
public:
    virtual bool isInitialized() = 0;
    virtual bool collide(IUnit* srcUnit, int posx, int posy) = 0;
    virtual CTile* at(int x, int y) = 0;
    virtual CChunk* getChunk(int x, int y) = 0;
    virtual bool hasTileWall(int direction, int x, int y) = 0;
    virtual void renderRoof(int tile) = 0;
    virtual void renderWall(int direction, int tile) = 0;
    virtual void initialize(int chunk_number) = 0;
    virtual void renderChunk(float posx, float posy, float pos_frac_x, float pos_frac_y, int render_tiles_view) = 0;
    virtual void renderTiles(float time) = 0;
};

#endif