// MovieMaker.h

#ifndef MOVIEMAKER_H

#define MOVIEMAKER_H

#include <QMainWindow>
#include "SBEMDB.h"

class MovieMaker: public QMainWindow {
public:
  MovieMaker(SBEMDB const *db, QWidget *parent=0);
  virtual ~MovieMaker();
private:
  class MM_Data *d;
};

#endif
