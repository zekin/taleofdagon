#ifndef CUNITVILLAGER_H
#define CUNITVILLAGER_H

#include "CUnit.h"

class CUnitVillager : public CUnit {
public:
    CUnitVillager(float x, float y, float z) : CUnit(x,y,z) {
        speed=0.8;
        armor=new Clothes(XYZ((float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX));
    }
};

#endif
