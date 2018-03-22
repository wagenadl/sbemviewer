// TileCache.h

#ifndef TILECACHE_H

#define TILECACHE_H

#include "TileID.h"
#include <QObject>

class TileCache: public QObject {
  Q_OBJECT;
public:
  TileCache(QString urlroot);
  ~TileCache();
  QImage getTile(TileID const &);
  bool hasTile(TileID const &) const;
  void setMaxRetained(int);
  void setAutoNeighbors(bool);
public slots:
  void requestTile(TileID);
signals:
  void loaded(TileID);
  //  void requestTiles(QVector<TileID> const &, TileConsumer *requestor);
private:
  class TileCacheData *d;
};

#endif
