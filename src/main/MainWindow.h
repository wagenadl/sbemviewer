// MainWindow.h

#ifndef MAINWINDOW_H

#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow: public QMainWindow {
  Q_OBJECT;
public:
  MainWindow(class TileCache *cache, class ServerInfo *info);
  virtual ~MainWindow();
  class SBEMDB *database() const;
  class TileViewer *tileViewer() const;
protected:
  void resizeEvent(QResizeEvent *);
  void closeEvent(QCloseEvent *);
public:
  void aboutAct();
  void openDB(QString fn);
private:
  class Ui_MainWindow *ui;
  class MWData *d;
};

#endif
