// Point.h

#ifndef POINT_H

#define POINT_H

struct Point {
  int x;
  int y;
  int z;
  Point(int x=0, int y=0, int z=0): x(x), y(y), z(z) {}
};

#endif
