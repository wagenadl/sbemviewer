// LineF.cpp

#include "LineF.h"
#include <cmath>

bool LineF::contains(PointF p, double mrg) const {
  auto sq = [](double x) { return x*x; };
  // According to wikipedia:
  // https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
  // The distance between point p and line through p1 and p2 is given by
  // a/b where a and b are:
  double a = fabs((p2.y-p1.y)*p.x - (p2.x-p1.x)*p.y + p2.x*p1.y - p2.y*p1.x);
  double b = sqrt(sq(p2.y-p1.y) + sq(p2.x-p1.x));
  // Instead of testing a/b < m, I test a < m*b to avoid misery when p1==p2.
  return a < mrg*b;
}
