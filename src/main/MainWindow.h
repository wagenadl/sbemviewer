// MainWindow.h

#ifndef MAINWINDOW_H

#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow: public QMainWindow {
  Q_OBJECT;
public:
  MainWindow(class TileCache *cache, class ServerInfo *info);
  virtual ~MainWindow();
  class SBEMDB *db();
protected:
  void resizeEvent(QResizeEvent *);
public:
  void aboutAct();
  void openDB(QString fn);
private:
  class Ui_MainWindow *ui;
  class MWData *d;
};

#endif
