// main.cpp

#include "MovieMaker.h"
#include <QApplication>
#include <QDebug>
#include "SBEMDB.h"
#include "SomaLabel.h"

int usage() {
  qDebug() << "Usage: sbemmoviemaker SBEMDB [LabelFile]";
  return 1;
}

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  if (argc<2 || argc>3)
    return usage();
  SBEMDB db;
  SomaLabel sm;
  
  db.open(argv[1]);
  if (!db.isOpen()) {
    qDebug() << "Cannot open db";
    return 2;
  }

  if (argc>=3) {
    if (!sm.load(argv[2])) {
      qDebug() << "Cannot open label file";
      return 2;
    }
  }
  
  MovieMaker mm(&db, &sm);
  mm.show();
  int res = app.exec();
  db.close();
  return res;
}
