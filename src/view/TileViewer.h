// TileViewer.h

#ifndef TILEVIEWER_H

#define TILEVIEWER_H

#include <QWidget>
#include "TileCache.h"
#include "Point.h"
#include "Mode.h"
#include "ViewInfo.h"

class TileViewer: public QWidget {
  Q_OBJECT;
public:
  static constexpr int MAXA = 8;
public:
  TileViewer(QWidget *parent=0);
  int x() const;
  int y() const;
  int z() const { return z_; }
  int scale() const { return a; }
  int visibleXRange() const;
  int visibleYRange() const;
  int level(float p) const;
  void addOverlay(class Overlay *);
  void removeOverlay(class Overlay *);
  Point mapToSBEM(QPoint widgetcoords) const;
  ViewInfo currentView() const;				      
public slots:
  void setMode(Mode);
  void setCache(TileCache *cache);
  void setInfo(class ServerInfo *info);
  void setScale(int); // 0 is full resolution, +n is 2^n reduced
  void setZ(int z); // slice #
  void setX(int x); // in pixels; refers to center of window
  void setY(int y); // in pixels; refers to center of window
  void setRS(int r, int s); // run, slice positioning
  void setPosition(int x, int y); // combination of above
  void setPosition(int x, int y, int z); // combination of above
  void stepX(int dx);
  void stepY(int dy);
  void stepZ(int dz);
  void setBlackLevel(int v);
  void setWhiteLevel(int v);
  void setGamma(float g);
  void setSharpening(float s);
signals:
  void viewChanged(int x, int y, int z, bool iscursor);
  void scaleChanged(int a);
protected:
  void paintEvent(QPaintEvent *) override;
  void resizeEvent(QResizeEvent *) override;
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void mouseReleaseEvent(QMouseEvent *) override;
  void wheelEvent(QWheelEvent *) override;
  void keyPressEvent(QKeyEvent *) override;
  void enterEvent(QEvent *) override;
  void leaveEvent(QEvent *) override;
private:
  void reportView();
  void enforceX();
  void enforceY();
  void enforceZ();
  void enforceA();
private:
  int x_, y_, z_, a;
  int lastpx, lastpy;
  int angleaccum;
  class TileCache *cache;
  class ServerInfo *info;
  bool isdrag;
  int blk, wht;
  float gamma;
  int sharp;
  QVector<uint8_t> lut;
  bool lutok;
  QVector<class Overlay *> overlays;
  Mode mode;
};

#endif
