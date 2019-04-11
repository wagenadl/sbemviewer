// MovieMaker.h

#ifndef MOVIEMAKER_H

#define MOVIEMAKER_H

#include <QMainWindow>

class MovieMaker: public QMainWindow {
public:
  MovieMaker(class SBEMDB const *db, class SomaLabel const *sm, QWidget *parent=0);
  virtual ~MovieMaker();
private:
  class MM_Data *d;
};

#endif
