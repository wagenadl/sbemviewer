// ServerInfo.h

#ifndef SERVERINFO_H

#define SERVERINFO_H

#include <QVariant>

class ServerInfo {
public:
  ServerInfo(QString urlroot);
  ~ServerInfo();
  QStringList keys() const;  
  QMap<QString, QVariant> const &map() const;
  QVariant value(QString) const;
  bool contains(QString) const;
  double real(QString k) const { return value(k).toDouble(); }
  int integer(QString k) const { return value(k).toInt(); }
  QString string(QString k) const { return value(k).toString(); }
  void update();
private:
  class ServerInfoData *d;
};

#endif
