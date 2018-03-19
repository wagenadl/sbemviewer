// TileID.h

#ifndef TILEID_H

#define TILEID_H

class TileID {
public:
  TileID(int a=0, int z=0, int y=0, int x=0): a(a), z(z), y(y), x(x) { }
  bool operator<(TileID const &o) const {
    if (a!=o.a)
      return a<o.a;
    else if (z!=o.z)
      return z<o.z;
    else if (y!=o.y)
      return y<o.y;
    else
      return x<o.x;
  }
public:
  int a, z, y, x;
};

#endif
