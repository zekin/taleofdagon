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
    virtual bool isInitialized()=0;
    virtual void removeObject(CRenderable* object)=0;
    virtual void setInitialized()=0;
    virtual void setTiles(int x, int y)=0;
};

#endif
