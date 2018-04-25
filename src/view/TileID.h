// TileID.h

#ifndef TILEID_H

#define TILEID_H

#include <QDebug>

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

inline QDebug &operator<<(QDebug &dbg, TileID const &id) {
  QString txt = QString("Tile(%1:%2,%3,%4)")
    .arg(id.a).arg(id.x).arg(id.y).arg(id.z);
  dbg << txt.toUtf8().data();
  return dbg;
}

#endif
