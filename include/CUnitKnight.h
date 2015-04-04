#ifndef CUNITKNIGHT
#define CUNITKNIGHT

#include "CUnit.h"

class CUnitKnight : public CUnit {
public:
    CUnitKnight(float x, float y, float z) : CUnit(x,y,z) {
        speed=0.4;
        armor=new ChainMail();
    }
};

#endif
