// simpleclient.cpp

#include "TileCache.h"
#include "ServerInfo.h"
#include "MainWindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char **argv) {
  QString server = "http://leechem.caltech.edu:9090";
  QApplication app(argc, argv);
  TileCache cache(server + "/tile");
  ServerInfo info(server);
  MainWindow mw(&cache, &info);
  mw.show();
  app.exec();
  return 0;
}
