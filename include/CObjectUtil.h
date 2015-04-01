#ifndef CUNITUTIL_H
#define CUNITUTIL_H
#include "enum.h"

class CObjectUtil {
private:
  CObjectUtil() {
  }
public:
  static bool isTypeAUnit(int type) {
    switch( type ) {
      case UNIT_TYPE_FIGHTER:
      case UNIT_TYPE_KNIGHT:
      case UNIT_TYPE_VILLAGER:
        return true;
      default:
        return false;
    }
  }
};

#endif