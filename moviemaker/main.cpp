// main.cpp

#include "MovieMaker.h"
#include <QApplication>
#include <QDebug>
#include "SBEMDB.h"

int usage() {
  qDebug() << "Usage: sbemmoviemaker SBEMDB";
  return 1;
}

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  if (argc!=2)
    return usage();
  SBEMDB db;
  db.open(argv[1]);
  if (!db.isOpen()) {
    qDebug() << "Cannot open db";
    return 2;
  }
  db.query("attach database ':memory:' as buildup");
  db.query("create table buildup.edges ( nid1, nid2 )");
  db.query("create table buildup.frontier ( nid )");
  db.query("create table buildup.interior ( nid )");
  db.query("create index buildup.intidx on interior(nid)");
  db.query("create index buildup.frontidx on frontier(nid)");
  
  MovieMaker mm(&db);
  mm.show();
  int res = app.exec();
  db.close();
  return res;
}
