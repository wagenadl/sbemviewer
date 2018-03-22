// ModeDlg.h

#ifndef MODEDLG_H

#define MODEDLG_H

#include <QWidget>
#include "ui_ModeDlg.h"

class ModeDlg: public QWidget {
public:
  ModeDlg(QWidget *parent=0);
  virtual ~ModeDlg();
public:
  Ui_ModeDlg *ui;
};

#endif
