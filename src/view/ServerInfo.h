// ServerInfo.h

#ifndef SERVERINFO_H

#define SERVERINFO_H

#include <QVariant>

class ServerInfo {
public:
  ServerInfo(QString urlroot);
  ServerInfo(); // creates dummy serverinfo object with no contents
  ServerInfo(QMap<QString, QString> const &origmap);
  ~ServerInfo();
  QStringList keys() const;  
  QMap<QString, QVariant> const &map() const;
  QMap<QString, QString> const &originalMap() const;
  QVariant value(QString) const;
  bool contains(QString) const;
  double real(QString k) const { return value(k).toDouble(); }
  double real(QString k, double dflt) const;
  int integer(QString k) const { return value(k).toInt(); }
  QString string(QString k) const { return value(k).toString(); }
  QString original(QString k) const;
  void update();
  bool waitForResponse(int timeout_ms=1000);
private:
  class ServerInfoData *d;
};

#endif
