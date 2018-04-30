// Overview.h

#ifndef OVERVIEW_H

#define OVERVIEW_H

#include <QWidget>

class Overview: public QWidget {
  Q_OBJECT;
public:
  Overview(QWidget *parent=0);
  virtual ~Overview();
public slots:
  void setInfo(class ServerInfo *);
  void setCache(class TileCache *);
  void setViewer(class TileViewer *);
signals:
  void clicked(int x, int y);
protected:
  void paintEvent(QPaintEvent *) override;
  void resizeEvent(QResizeEvent *) override;
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
private:
  void emitClicked(QPoint);
private:
  TileViewer *tileviewer;
  ServerInfo const *info;
  TileCache *cache;
  int a;
  int mrgx, mrgy;
};

#endif
