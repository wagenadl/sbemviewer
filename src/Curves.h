// Curves.h

#ifndef CURVES_H

#define CURVES_H

#include <QWidget>
#include "ui_Curves.h"

class Curves: public QWidget {
  Q_OBJECT;
public:
  Curves(QWidget *parent=0);
  virtual ~Curves();
public:
  Ui_Curves *ui;
};

#endif
