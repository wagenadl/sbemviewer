// ModeDlg.cpp

#include "ModeDlg.h"


ModeDlg::ModeDlg(QWidget *parent): QWidget(parent) {
  ui = new Ui_ModeDlg;
  ui->setupUi(this);
}

ModeDlg::~ModeDlg() {
  delete ui;
}
