// Settings.cpp

#include "Settings.h"

Settings::Settings(QObject *parent):
  QSettings("Caltech-WagenaarLab", "SBEMViewer", parent) {
}

QVariant Settings::get(QString const &key, QVariant const &dflt) {
  return QSettings::value(key, dflt);
}

bool Settings::contains(QString const &key) {
  return QSettings::contains(key);
}

void Settings::set(QString const &key, QVariant const &val) {
  setValue(key, val);
}
