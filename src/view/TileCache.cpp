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

TileCacheData::TileCacheData(QString urlroot, TileCache *parent):
  QObject(parent), urlroot(urlroot), tc(parent) {
  maxret = 1000;
  autoneighbors = false;
  t = 0;
  
  nam = new QNetworkAccessManager(this);
  connect(nam, SIGNAL(finished(QNetworkReply*)),
          this, SLOT(receiveQNR(QNetworkReply*)));
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
}

TileCache::~TileCache() {
  delete d;
}

QImage TileCache::getTile(TileID const &id) {
  if (d->tiles.contains(id)) {
    if (d->autoneighbors) {
      TileID a;
      a = id; a.z+=1;
      requestTile(a);
      a = id; a.z-=1;
      requestTile(a);
      a = id; a.x+=1;
      requestTile(a);
      a = id; a.x-=1;
      requestTile(a);
      a = id; a.y-=1;
      requestTile(a);
      a = id; a.y+=1;
      requestTile(a);
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
  
void TileCache::setAutoNeighbors(bool an) {
  d->autoneighbors = an;
}

void TileCache::requestTile(TileID id) {
  if (d->tiles.contains(id))
    return;
  if (d->pending.contains(id))
    return;
  if (id.x<0 || id.y<0 || id.z<0)
    return;

  // let's cancel far-away layers, and other a
  for (TileID t: d->pending.keys()) {
    if (abs(t.z - id.z)>2 || t.a != id.a) {
      QNetworkReply *r = d->pending[t];
      if (!r->isFinished() && !d->gotsome.contains(t)) {
	d->pending[t]->abort();
	d->pending.remove(t);
      }
    }
  }

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
