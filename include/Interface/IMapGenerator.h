#ifndef IMAPGENERATOR_H
#define IMAPGENERATOR_H

#include "struct.h"
class IMapGenerator {
public:
   virtual int getWidth() { return 1; }
   virtual int getHeight() { return 1; }
   virtual TileInstantiation getTileAt(float x, float y) = 0;
   virtual void generateWorldImage() = 0;
};

#endif