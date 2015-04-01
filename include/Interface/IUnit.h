#ifndef IUNIT_H
#define IUNIT_H
enum {
  SKIN_WHITE,
  SKIN_BLACK,
  SKIN_YELLOW,
  SKIN_RED,
  SKIN_BROWN
};
enum {
  AI_PLAYER,
  AI_STAND_AROUND,
  AI_WALKING
};
enum {
  ARMOR_CLOTHES,
  ARMOR_SCALEMAIL,
  ARMOR_CHAINMAIL,
  ARMOR_PLATEMAIL
};


class IMap;
class IUnit {
public:

protected:
  float unitPosX;
  float unitPosY;
protected:
  virtual void init() = 0;
  virtual void move() = 0;
  virtual void AI() = 0;
  virtual void registerToMap(IMap* map) = 0;
};
#endif