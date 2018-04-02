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
  bool cleanexit = settings.get("cleanexit", false).toBool();
  settings.set("cleanexit", false);
  QApplication app(argc, argv);
  TileCache cache(server + "/tile");
  ServerInfo info(server);
  MainWindow mw(&cache, &info);
  mw.show();
  if (cleanexit) {
    TileViewer *tv = mw.tileViewer();
    
    QString dbfn = settings.get("database", "").toString();
    if (!dbfn.isEmpty() && QFileInfo::exists(dbfn))
      mw.openDB(dbfn);
  }
  app.exec();
  settings.set("cleanexit", true);
  return 0;
}
