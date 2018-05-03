// Overview.cpp

#include "Overview.h"
#include "ServerInfo.h"
#include "TileViewer.h"
#include "TileCache.h"
#include "ViewInfo.h"

#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QPainter>

static int appropriateScale(QSize s, ServerInfo const *info) {
  int dx = info->integer("x1") - info->integer("x0");
  int dy = info->integer("y1") - info->integer("y0");
  int amax = info->integer("amax");
  int w = s.width();
  int h = s.height();
  if (dx<1 || dy<1)
    return 0;
  if (w<1 || h<1)
    return 99;
  // we want dx to fit inside w<<a
  int a = 0;
  while (((w<<a) < dx  ||  (h<<a) < dy) && a<amax)
    a++;
  return a;
}

Overview::Overview(QWidget *parent):
  QWidget(parent) {
  tileviewer = 0;
  cache = 0;
  info = 0;
  a = 0;
  mrgx = mrgy = 0;
}

Overview::~Overview() {
}

void Overview::setCache(class TileCache *c) {
  cache = c;
  connect(c, SIGNAL(loaded(TileID)),
          this, SLOT(update()));
  update();
}

void Overview::setViewer(class TileViewer *tv) {
  tileviewer = tv;
  update();
}

void Overview::setInfo(class ServerInfo *si) {
  info = si;
  update();
}

void Overview::paintEvent(QPaintEvent *) {
  if (!cache)
    return;
  if (!info)
    return;
  if (!tileviewer)
    return;

  QPainter ptr;
  ptr.begin(this);

  ViewInfo vi = tileviewer->currentView();

  int L = ViewInfo::TILESIZE;
  a = appropriateScale(size(), info);
  int x0 = info->integer("x0");
  int c0 = (x0 >> a)/L;
  int x1 = info->integer("x1");
  int c1 = 1 + ((x1-1) >> a)/L;
  int y0 = info->integer("y0");
  int r0 = (y0 >> a)/L;
  int y1 = info->integer("y1");
  int r1 = 1 + ((y1-1) >> a)/L;
  int cutleft = (x0>>a) - L*c0;
  int cuttop = (y0>>a) - L*r0;

  mrgx = width()/2 - ((x1-x0)>>a)/2;
  mrgy = height()/2 - ((y1-y0)>>a)/2;
  
  for (int r=r0; r<r1; r++) {
    for (int c=c0; c<c1; c++) {
      QImage img = cache->getTile(TileID(a, vi.z, r, c));
      if (img.isNull())
        continue;
      ptr.drawImage(QPoint(c*L-cutleft + mrgx, r*L-cuttop + mrgy), img);
    }
  }

  int vx0 = (vi.xl - x0) >> a;
  int vx1 = (vi.xr - x0) >> a;
  int vy0 = (vi.yt - y0) >> a;
  int vy1 = (vi.yb - y0) >> a;
  ptr.setPen(QPen(QColor(255,255,0), 2));
  ptr.setBrush(QBrush(Qt::NoBrush));
  ptr.drawRect(QRect(QPoint(vx0 + mrgx, vy0 + mrgy),
                     QPoint(vx1 + mrgx, vy1 + mrgy)));
}

void Overview::resizeEvent(QResizeEvent *) {
  update();
}

void Overview::mousePressEvent(QMouseEvent *e) {
  emitClicked(e->pos());
}

void Overview::emitClicked(QPoint pos) {
  if (!info)
    return;
  int x = pos.x() - mrgx;
  int y = pos.y() - mrgy;
  int x0 = info->integer("x0");
  int y0 = info->integer("y0");
  emit clicked((x<<a) + x0, (y<<a) + y0);
}

void Overview::mouseMoveEvent(QMouseEvent *e) {
  emitClicked(e->pos());
}

