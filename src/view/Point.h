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

inline Point operator-(Point const &a, Point const &b) {
  return Point(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Point operator+(Point const &a, Point const &b) {
  return Point(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Point operator*(int a, Point const &b) {
  return Point(a * b.x, a * b.y, a * b.z);
}

inline Point operator*(Point const &b, int a) {
  return Point(a * b.x, a * b.y, a * b.z);
}

inline Point operator/(Point const &b, int a) {
  return Point(b.x / a, b.y / a, b.z / a);
}

inline QDebug &operator<<(QDebug &dbg, Point const &p) {
  dbg << "Point(" << p.x << "," << p.y << "," << p.z << ")";
  return dbg;
}


#endif
