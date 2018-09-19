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
  void setMaxXYDist(int, int);
  void setAutoNeighbors(bool);
  QString urlRoot() const;
public slots:
  void requestTile(TileID, bool lowprio=false);
  void requestTileSoon(TileID);
  void notifyNewSliceAvailable(int z);
  void findRS(int r, int s);
signals:
  void loaded(TileID);
  //  void requestTiles(QVector<TileID> const &, TileConsumer *requestor);
  void foundRS(int z);
private:
  class TileCacheData *d;
};

#endif
