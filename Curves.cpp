// Curves.cpp

#include "Curves.h"

Curves::Curves(QWidget *parent): QWidget(parent) {
  ui = new Ui_Curves;
  ui->setupUi(this);
}

Curves::~Curves() {
  delete ui;
}
