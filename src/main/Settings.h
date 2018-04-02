// Settings.h

#ifndef SETTINGS_H

#define SETTINGS_H

#include <QSettings>

class Settings: private QSettings {
public:
  Settings(QObject *parent=0);
  QVariant get(QString const &key, QVariant const &dflt=QVariant()) const;
  bool contains(QString const &key) const;
  void set(QString const &key, QVariant const &val);
};

#endif
