#ifndef ICHUNK_H
#define ICHUNK_H
class CTile;
class CRenderable;

class IChunk {
protected:
public:
  virtual void addObject(CRenderable* object)=0;
  virtual CRenderable* getObject(int object)=0;
  virtual CTile* at(int x, int y)=0;
  bool isInitialized()=0;
  void removeObject(CRenderable* object)=0;
  void setInitialized()=0;
  void setTiles(int x, int y)=0;
};

#endif