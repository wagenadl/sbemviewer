// TileCacheData.h

#ifndef TILECACHEDATA_H

#define TILECACHEDATA_H

#include <QObject>
#include "TileCache.h"
#include <QMap>
#include <QImage>
#include "TileID.h"
#include <QUrl>

class TileCacheData: public QObject {
  Q_OBJECT;
public:
  TileCacheData(QString urlroot, TileCache *parent);
public slots:
  void receiveQNR(class QNetworkReply *);
public:
  QString urlroot;
  int maxret;
  bool autoneighbors;
  int t;
  QMap<TileID, QImage> tiles;
  QMap<TileID, int> age;
  class QNetworkAccessManager *nam;
  TileCache *tc;
  QMap<QUrl, TileID> urlmap;
  QMap<TileID, QNetworkReply *> pending;
  QMap<TileID, bool> gotsome;
public:
  void clean();
};

#endif
