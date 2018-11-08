// Transform3.h

#ifndef TRANSFORM3_H

#define TRANSFORM3_H

#include "LineF.h"
#include <QDebug>

class Transform3 {
  // maps screen coords to voxel coords
public:
  Transform3();
  Transform3(double const src[3][3]);
  PointF apply(PointF const &a) const {
    PointF p;
    p.x = m[0][0]*a.x + m[0][1]*a.y + m[0][2]*a.z + m[0][3];
    p.y = m[1][0]*a.x + m[1][1]*a.y + m[1][2]*a.z + m[1][3];
    p.z = m[2][0]*a.x + m[2][1]*a.y + m[2][2]*a.z + m[2][3];
    return p;
  }
  LineF apply(LineF const &l) const {
    return LineF(apply(l.p1), apply(l.p2));
  }
    
  void shift(double dx, double dy, double dz); // screen coords
  void rotate(double dxz, double dyz, double x0, double y0);
  void rotatez(double dxy, double x0, double y0);
  void scale(double s, double x0, double y0);
  void report() const;
  Transform3 inverse() const;
  double det() const;
public:
  static Transform3 shifter(double dx, double dy, double dz);
  static Transform3 yrotator(double dxz);
  static Transform3 xrotator(double dyz);
  static Transform3 zrotator(double dxy);
  static Transform3 scaler(double s);
public:
  double m[4][4];
};

Transform3 operator*(Transform3 const &l, Transform3 const &r);

QDebug operator<<(QDebug, Transform3 const &);

#endif
