// TileCache.cpp

#include "TileCache.h"
#include "TileCacheData.h"
#include "TiffLoader.h"

#include <QImage>
#include <algorithm>
#include <QMap>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QDateTime>

constexpr int MAXATONCE = 6;

TileCacheData::TileCacheData(QString urlroot, TileCache *parent):
  QObject(parent), urlroot(urlroot), tc(parent) {
  maxret = 1000;
  maxdx = maxdy = 10;
  autoneighbors = false;
  t = 0;
  maxz = 100000;
  
  nam = new QNetworkAccessManager(this);
  connect(nam, SIGNAL(finished(QNetworkReply*)),
          this, SLOT(receiveQNR(QNetworkReply*)));
}

void TileCacheData::processPrePending() {
  while (pending.size() < MAXATONCE && !prepending.isEmpty()) {
    TileID id = prepending.takeFirst();
    QUrl url = QString("%1/tile/A%2/Z%3/Y%4/X%5")
      .arg(urlroot)
      .arg(id.a)
      .arg(id.z)
      .arg(id.y)
      .arg(id.x);
    urlmap[url] = id;
    pending[id] = nam->get(QNetworkRequest(url));
    connect(pending[id], &QNetworkReply::readyRead,
	    [this, id]() { gotsome[id] = true; });
  }
}

void TileCacheData::dropOtherRequests(TileID id) {
  for (TileID t: pending.keys()) {
    if (t.a != id.a
        || abs(t.z - id.z)>2
        || abs(t.x - id.x)>maxdx
        || abs(t.y - id.y)>maxdy) {
      QNetworkReply *r = pending[t];
      if (!r->isFinished() && !gotsome.contains(t)) {
	pending[t]->abort();
	pending.remove(t);
      }
    }
  }

  for (auto it=prepending.begin(); it!=prepending.end(); ) {
    TileID t = *it;
    if (t.a != id.a
        || abs(t.z - id.z)>2
        || abs(t.x - id.x)>maxdx
        || abs(t.y - id.y)>maxdy) 
      it = prepending.erase(it);
    else
      ++it;
  }
}

void TileCacheData::clean() {
  if (tiles.size() <= maxret)
    return;
  QList<int> ages = age.values();
  std::sort(ages.begin(), ages.end());
  int a0 = ages[ages.size() - int(.7*maxret)];
  for (TileID id: tiles.keys()) {
    if (age[id]<a0) {
      tiles.remove(id);
      age.remove(id);
    }
  }
}

void TileCache::notifyNewSliceAvailable(int z) {
  d->maxz = z;
  // bluntly forget all empty images so they will be requested anew
  for (TileID id: d->tiles.keys()) {
    if (d->tiles[id].isNull()) {
      d->tiles.remove(id);
      d->age.remove(id);
    }
  }
}
  

void TileCacheData::receiveQNR(QNetworkReply *reply) {
  if (urlmap.contains(reply->url())) {
    TileID id = urlmap[reply->url()];
    urlmap.remove(reply->url());
    pending.remove(id);
    gotsome.remove(id);

    switch (reply->error()) {
    case QNetworkReply::NoError: {
      // could be good
      QByteArray data = reply->read(1000*1000);
      if (data.isEmpty()) {
	qDebug() << "No image from" << reply->url().toString();
        tiles[id] = QImage();
        age[id] = ++t;
      } else {
        QImage img = TiffLoader::read(data);
        if (img.isNull()) {
          qDebug() << "Not a valid image from"
                   << reply->url().toString() << data;
          tiles[id] = QImage();
          age[id] = ++t;
        } else {
          // got an image!
          tiles[id] = img;
          age[id] = ++t;
          tc->loaded(id);
          clean();
        }
      }
    } break;
    case QNetworkReply::OperationCanceledError:
      qDebug() << "(Canceled)";
      break;
    default:
      //qDebug() << "Received error" << reply->error() << reply->errorString()
      //         << "from" << reply->url().toString();
      break;
    }
  } else if (reply->url().toString().contains("findRS/")) {
    QByteArray data = reply->read(1000);
    QString str(data);
    bool ok;
    int z = str.toInt(&ok);
    if (ok)
      tc->foundRS(z);
    else
      qDebug() << "Bad result from findRS:" << str
               << " [" << reply->url().toString() << "]";
  } else {
    qDebug() << "Received unexpected QNR:" << reply->url().toString();
  }
  reply->deleteLater();
  processPrePending();
}

TileCache::TileCache(QString urlroot): d(new TileCacheData(urlroot, this)) {
}

TileCache::~TileCache() {
  delete d;
}

QImage TileCache::getTile(TileID const &id) {
  if (d->tiles.contains(id) && !d->tiles[id].isNull()) {
    if (d->autoneighbors) {
      TileID a;
      a = id; a.z+=1;
      if (a.z<=d->maxz)
	requestTileSoon(a);
      a = id; a.z-=1;
      if (a.z>=0)
	requestTileSoon(a);
      a = id; a.x+=1;
      requestTileSoon(a);
      a = id; a.x-=1;
      requestTileSoon(a);
      a = id; a.y-=1;
      requestTileSoon(a);
      a = id; a.y+=1;
      requestTileSoon(a);
    }
    return d->tiles[id];
  } else {
    requestTile(id);
    return QImage();
  }
}

bool TileCache::hasTile(TileID const &id) const {
  return d->tiles.contains(id);
}

void TileCache::setMaxRetained(int mx) {
  d->maxret = mx;
  d->clean();
}

void TileCache::setMaxXYDist(int x, int y) {
  d->maxdx = x;
  d->maxdy = y;
}
 
void TileCache::setAutoNeighbors(bool an) {
  d->autoneighbors = an;
}

void TileCache::requestTileSoon(TileID id) {
  requestTile(id, true);
}

void TileCache::requestTile(TileID id, bool lowprio) {
  if (!id.isValid())
    return;
  if (d->tiles.contains(id))
    return;
  if (d->pending.contains(id))
    return;
  if (d->prepending.contains(id)) {
    if (!lowprio) {
      d->prepending.removeOne(id);
      d->prepending.push_front(id);
      d->processPrePending();
    }
    return;
  }
  if (id.x<0 || id.y<0 || id.z<0)
    return;

  // let's cancel far-away layers, and other a
  if (!lowprio)
    d->dropOtherRequests(id);

  d->gotsome[id] = false;
  if  (lowprio)
    d->prepending.push_back(id);
  else
    d->prepending.push_front(id);

  d->processPrePending();
}

void TileCache::findRS(int r, int s) {
  QUrl url = QString("%1/findRS/R%2/S%3").arg(d->urlroot)
    .arg(r).arg(s);
  d->nam->get(QNetworkRequest(url));
}

QString TileCache::urlRoot() const {
  return d->urlroot;
}

