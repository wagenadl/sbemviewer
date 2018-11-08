// sbem3d.cpp

#include <QApplication>

#include "ProjectionWidget.h"
#include "SBEMDB.h"
#include "ServerInfo.h"
#include <stdlib.h>
#include <QDebug>

void usage() {
  fprintf(stderr, "Usage: sbem3d [database]\n");
  exit(1);
}

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  QString dbfn = "/home/wagenaar/Desktop/170428_ganglion10-181107.sbemdb";
  if (argc==2)
    dbfn = argv[1];
  else if (argc>2)
    usage();

  SBEMDB db("sbem");
  db.open(dbfn);
  if (!db.isOpen()) {
    qFatal("Failed to open DB: %s", dbfn.toUtf8().data());
    exit(1);
  }

  ServerInfo dummy;
  
  ProjectionWidget pw(&dummy, &db);
  pw.addVisibleTrees();
  pw.addSelectedTree();
  pw.resize(800, 800);
  pw.show();
  return app.exec();
}

