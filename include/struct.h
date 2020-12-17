#ifndef STRUCT_H
#define STRUCT_H

#include <string>
#include <vector>
#include "constants.h"

static unsigned int global_id=1000;

struct Event {
    Event() : type(0), a(0), b(0), c(0), x(0.0), y(0.0), z(0.0) {}
    Event(int type) : type(type), a(0), b(0), c(0), x(0.0), y(0.0), z(0.0) {}
    Event(int type, long data) : type(type), a(data), b(0), c(0), x(0.0), y(0.0), z(0.0) {}
    Event(int type, std::string string) : type(type), a(0), b(0), c(0), x(0), y(0), z(0), string(string) {}
    Event(int type, float x, float y, float z) : type(type), x(x), y(y), z(z) {}
    Event(int type, long data, float x) : type(type), a(data), b(0), c(0), x(x), y(0), z(0) {}
    std::string string;
    unsigned int type;
    long a,b,c;
    float q,r,s,x,y,z;
};
struct XY {
    double x;
    double y;
    XY() : x(0), y(0) {}
    XY(double x,double y) : x(x), y(y) {}
    int dir() {
        if (x >= abs(y)) return DIRECTION_EAST;
        else if (-y >= abs(x)) return DIRECTION_NORTH;
        else if (-x >= abs(y)) return DIRECTION_WEST;
        else if (y >= abs(x)) return DIRECTION_SOUTH;
        else {
            ERROR(LOG) << "Direction at " << x << "," << y << "doesn't meet any criteria";
            return -1;
        }
    }
};
struct XYZ {
    double x;
    double y;
    double z;
    XYZ() : x(0), y(0) {}
    XYZ(double x, double y, double z) : x(x), y(y), z(z) {}

};

struct Rect {
    XY tl,
       tr,
       bl,
       br;
    Rect(XY tl, XY tr, XY bl, XY br) : tl(tl), tr(tr), bl(bl), br(br) { }
};

struct TileInstantiation {
    int tile_number;
    int object_type;
    int unit_type;
    int area_type;
    TileInstantiation(int tile_number, int area_type, int object_type=0, int unit_type=0) :
        tile_number(tile_number),
        object_type(object_type),
        unit_type(unit_type),
        area_type(area_type) {}
};

struct Animation {
private:
    float time;
    std::vector<int> frame_numbers;
public:
    Animation() {}
};
#endif