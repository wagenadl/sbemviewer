// Miniball.h

#ifndef MINIBALL_H

#define MINIBALL_H

#include <QVector>
#include "Point.h"

class Miniball {
  /* Calculate minimal enclosing sphere in 2D
     This is a wrapper around code by Project Nayuki
     https://www.nayuki.io/page/smallest-enclosing-circle
     Not the fastest implementation on the net, but most straightforward
     to adapt for our use.
     I am using mean(z) for the z-coordinate of the center.
  */
public:
  Miniball(QVector<Point> const &);
  Point center() const { return c; }
  int radius() const { return r; }
private:
  Point c;
  int r;
};

#endif
