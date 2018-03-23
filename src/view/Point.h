// Point.h

#ifndef POINT_H

#define POINT_H

#include <QDebug>

struct Point {
  int x;
  int y;
  int z;
  Point(int x=0, int y=0, int z=0): x(x), y(y), z(z) {}
};

inline QDebug &operator<<(QDebug &dbg, Point const &p) {
  dbg << "Point(" << p.x << "," << p.y << "," << p.z << ")";
  return dbg;
}


#endif
