// ServerInfo.cpp

#include "ServerInfo.h"
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QEventLoop>

class ServerInfoData {
public:
  ServerInfoData() {
    nam = 0;
  }
  ~ServerInfoData() {
    if (nam)
      delete nam;
  }
public:
  QVariant splitVector(QString s) {
    QStringList bits = s.split(":", QString::SkipEmptyParts);
    QVariantList vv;
    if (s.contains(".")) 
      for (QString b: bits)
	vv << b.toDouble();
    else 
      for (QString b: bits)
	vv << b.toInt();
    return QVariant(vv);
  }
  void respond(QNetworkReply *r) {
    orig["urlroot"] = "\"" + urlroot + "\"";
    QByteArray data = r->read(1000*1000);
    QString s(data);
    QStringList lines = s.split("\n");
    for (QString kv: lines) {
      kv = kv.simplified();
      QStringList bits = kv.split("=");
      if (bits.size()>=2) {
        QString k = bits.takeFirst();
        QString v = bits.join("=");
	orig[k] = v;
      } else if (!kv.isEmpty()) {
	qDebug() << "ServerInfo: Cannot parse" << kv;
      }
    }
    interpretOrig();
    r->deleteLater();
    nam->deleteLater();
    nam = 0;
  }
  void interpretOrig() {
    for (auto it=orig.begin(); it!=orig.end(); it++) {
      QString k = it.key();
      QString v = it.value();
      if (v.startsWith("\"") && v.endsWith("\""))
	infos[k] = v.mid(1, v.size()-2);
      else if (v.contains(":"))
	infos[k] = splitVector(v);
      else if (v.contains("."))
	infos[k] = v.toDouble();
      else
	infos[k] = v.toInt();
    }
  }
public:
  QNetworkAccessManager *nam;
  QString urlroot;
  QMap<QString, QVariant> infos;
  QMap<QString, QString> orig;
};

ServerInfo::ServerInfo(QString urlroot) {
  d = new ServerInfoData;
  d->urlroot = urlroot;
  d->nam = 0;
  update();
}

ServerInfo::ServerInfo() {
  d = new ServerInfoData;
  d->nam = 0;
}

ServerInfo::ServerInfo(QMap<QString, QString> const &origmap) {
  d = new ServerInfoData;
  d->nam = 0;
  d->orig = origmap;
  d->interpretOrig();
}

ServerInfo::~ServerInfo() {
  delete d;
}

void ServerInfo::update() {
  if (!d->nam) {
    d->nam = new QNetworkAccessManager;
    QObject::connect(d->nam, &QNetworkAccessManager::finished,
                     [this](QNetworkReply *r) { d->respond(r); });
    d->nam->get(QNetworkRequest(d->urlroot + "/info"));
  }
}

QMap<QString, QVariant> const &ServerInfo::map() const {
  return d->infos;
}

QStringList ServerInfo::keys() const {
  return QStringList(d->infos.keys());
}

QVariant ServerInfo::value(QString k) const {
  if (d->infos.contains(k))
    return d->infos[k];
  else
    return QVariant();
}

bool ServerInfo::contains(QString k) const {
  return d->infos.contains(k);
}

double ServerInfo::real(QString k, double dflt) const {
  if (contains(k))
    return real(k);
  else
    return dflt;
}

QString ServerInfo::original(QString k) const {
  if (contains(k))
    return d->orig[k];
  else
    return QString();
}

bool ServerInfo::waitForResponse(int timeout_ms) {
  if (d->nam==0)
    return true;
  QTime t0;
  t0.start();
  QEventLoop loop;
  int dt = timeout_ms - t0.elapsed();
  while (dt>0) {
    if (d->nam==0)
      return true;
    loop.processEvents(QEventLoop::ExcludeUserInputEvents, dt);
    dt = timeout_ms - t0.elapsed();
  }
  return false;
}

QMap<QString, QString> const &ServerInfo::originalMap() const {
  return d->orig;
}

