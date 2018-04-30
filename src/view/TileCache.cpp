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

TileCacheData::TileCacheData(QString urlroot, TileCache *parent):
  QObject(parent), urlroot(urlroot), tc(parent) {
  maxret = 1000;
  maxdx = maxdy = 10;
  autoneighbors = false;
  t = 0;
  
  nam = new QNetworkAccessManager(this);
  connect(nam, SIGNAL(finished(QNetworkReply*)),
          this, SLOT(receiveQNR(QNetworkReply*)));
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
  for (auto it=soonrequests_bytime.begin();
       it!=soonrequests_bytime.end(); ) {
    TileID t = it.value();
    if (t.a != id.a
        || abs(t.z - id.z)>2
        || abs(t.x - id.x)>maxdx + 2
        || abs(t.y - id.y)>maxdy + 2) {
      it = soonrequests_bytime.erase(it);
      soonrequests_byid.remove(t);
      qDebug() << "Dropping postponed" << t << maxdx << maxdy << id;
    } else {
      ++it;
    }
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

void TileCache::notifyNewSliceAvailable() {
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
	  qDebug() << "Got image from" << reply->url().toString() << "for" << id  << "size" << img.size();
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
      qDebug() << "Received error" << reply->error() << reply->errorString()
               << "from" << reply->url().toString();
      break;
    }
  } else {
    qDebug() << "Received unexpected QNR:" << reply->url().toString();
  }
  reply->deleteLater();
}

TileCache::TileCache(QString urlroot): d(new TileCacheData(urlroot, this)) {
  startTimer(100);
}

TileCache::~TileCache() {
  delete d;
}

QImage TileCache::getTile(TileID const &id) {
  if (d->tiles.contains(id) && !d->tiles[id].isNull()) {
    if (d->autoneighbors) {
      TileID a;
      a = id; a.z+=1;
      requestTileSoon(a);
      a = id; a.z-=1;
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
  if (d->tiles.contains(id))
    return;
  if (d->pending.contains(id))
    return;
  if (d->soonrequests_byid.contains(id))
    return;
  qDebug() << "requestilesoon" << id;
  d->dropOtherRequests(id);
  QDateTime t = QDateTime::currentDateTime().addMSecs(100);
  d->soonrequests_byid.insert(id, t);
  d->soonrequests_bytime.insert(t, id);
}

void TileCache::timerEvent(QTimerEvent *) {
  QDateTime now = QDateTime::currentDateTime();
  while (!d->soonrequests_bytime.isEmpty()) {
    auto it = d->soonrequests_bytime.begin();
    if (it.key() < now) {
      TileID id = it.value();
      qDebug() << "Requesting postponed" << id;
      requestTile(id);
      d->soonrequests_byid.remove(id);
      d->soonrequests_bytime.erase(it);
    } else {
      break;
    }
  }
}

void TileCache::requestTile(TileID id) {
  if (d->tiles.contains(id))
    return;
  if (d->pending.contains(id))
    return;
  if (id.x<0 || id.y<0 || id.z<0)
    return;

  // let's cancel far-away layers, and other a
  d->dropOtherRequests(id);

  QUrl url = QString("%1/A%2/Z%3/Y%4/X%5")
    .arg(d->urlroot)
    .arg(id.a)
    .arg(id.z)
    .arg(id.y)
    .arg(id.x);
  qDebug() << "request" << url.toString() << "for" << id;
  d->urlmap[url] = id;
  d->gotsome[id] = false;
  d->pending[id] = d->nam->get(QNetworkRequest(url));
  connect(d->pending[id], &QNetworkReply::readyRead,
	  [this, id]() { d->gotsome[id] = true; });
}

QString TileCache::urlRoot() const {
  return d->urlroot;
}
