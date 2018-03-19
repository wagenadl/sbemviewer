// Navigation.h

#ifndef NAVIGATION_H

#define NAVIGATION_H

#include <QWidget>
#include "ui_Navigation.h"

class Navigation: public QWidget {
public:
  Navigation(QWidget *parent=0);
  virtual ~Navigation();
public:
  Ui_Navigation *ui;
};

#endif
