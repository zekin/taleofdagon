#ifndef CUNITUTIL_H
#define CUNITUTIL_H
#include "enum.h"
#include "CRenderable.h"

class CObjectUtil {
private:
    CObjectUtil() {
    }
public:
    static bool isTypeAUnit(int type) {
        switch( type ) {
        case RENDERABLE_UNIT:
            return true;
        default:
            return false;
        }
    }
};

#endif
