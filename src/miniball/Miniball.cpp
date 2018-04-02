// Miniball.cpp

#include "Miniball.h"

#include "SmallestEnclosingCircle.hpp"
#include <cmath>

Miniball::Miniball(QVector<Point> const &p3d) {
  int N = p3d.size();
  if (N==0) {
    r = 0;
    return;
  }

  std::vector<SEC::Point> p2d(N);
  double zsum = 0;
  for (int n=0; n<N; n++) {
    p2d[n].x = p3d[n].x;
    p2d[n].y = p3d[n].y;
    zsum += p3d[n].z;
  }
  SEC::Circle circle = SEC::makeSmallestEnclosingCircle(p2d);

  c.x = round(circle.c.x);
  c.y = round(circle.c.y);
  c.z = zsum / N;
  r = circle.r;
}
