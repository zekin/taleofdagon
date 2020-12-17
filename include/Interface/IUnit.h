#ifndef IUNIT_H
#define IUNIT_H
#include "CRenderable.h"


class IMap;
class IUnit : public CRenderable {
public:

protected:
    IUnit(int x, int y, int z) : CRenderable(x,y,z) {}
    float unitPosX;
    float unitPosY;
public:
    virtual void init() = 0;
    virtual void move() = 0;
    virtual void AI() = 0;
    virtual bool isMoving() = 0;
    virtual float getX() = 0;
    virtual float getY() = 0;
};
#endif
