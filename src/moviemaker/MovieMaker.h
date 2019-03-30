// MovieMaker.h

#ifndef MOVIEMAKER_H

#define MOVIEMAKER_H

#include <QMainWindow>
#include "SBEMDB.h"
#include "ServerInfo.h"

class MovieMaker: public QMainWindow {
public:
  MovieMaker(ServerInfo const *info, SBEMDB const *db, QWidget *parent=0);
  virtual ~MovieMaker();
private:
  class MM_Data *d;
};

#endif
