#ifndef CWORLDMAP_H
#define CWORLDMAP_H

#include "CMap.h"

class CWorldMap : public CMap {
public:
    CWorldMap() : CMap(2000,2000) {
    }
    virtual void notify(Event* e) {
        if (e->type==EVENT_RENDER_FRAME) {
            //renderTiles(e->x);
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
#
#endif