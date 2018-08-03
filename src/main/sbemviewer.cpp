// simpleclient.cpp

#include "TileCache.h"
#include "ServerInfo.h"
#include "MainWindow.h"
#include <QApplication>
#include <QDebug>
#include <QFileInfo>
#include <QInputDialog>

#include "Settings.h"
#include "TileViewer.h"

const QString defaultServer = "http://leechem.caltech.edu:9090";

QString getServer(QString dflt) {
  QString server = QInputDialog::getText(0, "SBEM Viewer", "Server name",
                                         QLineEdit::Normal, dflt);
  if (!server.startsWith("http://") && !server.startsWith("https://"))
    server = "http://" + server;
  if (server.lastIndexOf(":") <= server.indexOf(":"))
    server += ":9090";
  return server;
}

int main(int argc, char **argv) {
  Settings settings;
  bool cleanexit = settings.get("cleanexit", false).toBool();
  QString dbfn = settings.get("database", "").toString();
  QString srv0 = settings.get("server", defaultServer).toString();

  settings.set("cleanexit", false);
  if (!cleanexit)
    settings.set("database", "");

  QApplication app(argc, argv);

  QString server = getServer(srv0);
  if (server != srv0) {
    // database likely refers to specific server, so...
    cleanexit = false;
    dbfn = "";
    settings.set("database", "");
  }
  
  TileCache cache(server);
  ServerInfo info(server);

  MainWindow mw(&cache, &info);
  mw.show();
  mw.setWindowTitle(mw.windowTitle() + " at " + server);
  if (cleanexit) {
    TileViewer *tv = mw.tileViewer();
    tv->setPosition(settings.get("x", tv->x()).toInt(),
                    settings.get("y", tv->y()).toInt(),
                    settings.get("z", tv->z()).toInt());
    tv->setScale(settings.get("a", tv->scale()).toInt());
    if (!dbfn.isEmpty() && QFileInfo::exists(dbfn))
      mw.openDB(dbfn);
  }

  app.exec();

  TileViewer *tv = mw.tileViewer();
  settings.set("x", tv->x());
  settings.set("y", tv->y());
  settings.set("z", tv->z());
  settings.set("a", tv->scale());
  settings.set("cleanexit", true);
  settings.set("server", server);
  return 0;
}
