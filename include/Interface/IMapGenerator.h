#ifndef IMAPGENERATOR_H
#define IMAPGENERATOR_H

class IMapGenerator {
public:
   virtual TileInstantiation getTileAt(float x, float y) = 0;
   virtual void generateWorldImage() = 0;
};

#endif