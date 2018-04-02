// simpleclient.cpp

#include "TileCache.h"
#include "ServerInfo.h"
#include "MainWindow.h"
#include <QApplication>
#include <QDebug>
#include <QFileInfo>
#include "Settings.h"

int main(int argc, char **argv) {
  Settings settings;
  QString server = "http://leechem.caltech.edu:9090";
  QApplication app(argc, argv);
  TileCache cache(server + "/tile");
  ServerInfo info(server);
  MainWindow mw(&cache, &info);
  mw.show();
  if (QFileInfo::exists("/tmp/sbemv.sbemdb"))
    mw.openDB("/tmp/sbemv.sbemdb");
  app.exec();
  return 0;
}
