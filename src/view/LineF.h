// Line.h

#ifndef LINEF_H

#define LINEF_H

#include <QDebug>

struct PointF {
  double x, y, z;
  PointF(double x=0, double y=0, double z=0): x(x), y(y), z(z) { }
  double L2() const {
    return x*x + y*y + z*z;
  }
  double L2xy() const {
    return x*x + y*y;
  }
  inline PointF &operator+=(PointF const &b) {
    x += b.x; y += b.y; z += b.z;
    return *this;
  }
  inline PointF &operator/=(double b) {
    x /= b; y /= b; z /= b;
    return *this;
  }
};

inline PointF operator-(PointF const &a, PointF const &b) {
  return PointF(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline PointF operator+(PointF const &a, PointF const &b) {
  return PointF(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline PointF operator*(double a, PointF const &b) {
  return PointF(a * b.x, a * b.y, a * b.z);
}

inline PointF operator*(PointF const &b, double a) {
  return PointF(a * b.x, a * b.y, a * b.z);
}

inline PointF operator/(PointF const &b, double a) {
  return PointF(b.x / a, b.y / a, b.z / a);
}

struct LineF {
  PointF p1, p2;
  LineF(PointF p1=PointF(), PointF p2=PointF()): p1(p1), p2(p2) { }
};

inline QDebug operator<<(QDebug dbg, PointF const &p) {
  dbg << "(" << p.x << p.y << p.z <<")";
  return dbg;
}

inline QDebug operator<<(QDebug dbg, LineF const &l) {
  dbg << "{" << l.p1 << l.p2 << "}";
  return dbg;
}

#endif
