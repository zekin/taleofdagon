#ifndef CUNITFIGHTER_H
#define CUNITFIGHTER_H

#include "CUnit.h"

class CUnitFighter : public CUnit {
public:
    CUnitFighter(float x, float y, float z) : CUnit(x,y,z) {
        speed=0.6;
        armor=new ScaleMail();
    }
};
#endif
