// Line.h

#ifndef LINEF_H

#define LINEF_H

struct PointF {
  double x, y, z;
  PointF(double x=0, double y=0, double z=0): x(x), y(y), z(z) { }
};

struct LineF {
  PointF p1, p2;
};

#endif
