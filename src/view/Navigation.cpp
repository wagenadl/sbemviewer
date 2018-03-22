// Navigation.cpp

#include "Navigation.h"

Navigation::Navigation(QWidget *parent): QWidget(parent) {
  ui = new Ui_Navigation;
  ui->setupUi(this);
}

Navigation::~Navigation() {
  delete ui;
}
