// TileViewer.cpp

#include "TileViewer.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <cmath>
#include "ServerInfo.h"
#include "ViewInfo.h"
#include "Overlay.h"

#include "../../sbemtools/1020/Gray.h"

TileViewer::TileViewer(QWidget *parent):
  QWidget(parent),
  cache(0), info(0) {
  mode = Mode_View;
  z_ = 805;
  x_ = 63*ViewInfo::TILESIZE;
  y_ = 150*ViewInfo::TILESIZE;
  a = 0;
  angleaccum = 0;
  setMouseTracking(true);
  isdrag = false;
  blk = 0;
  wht = 255;
  gamma = 1;
  sharp = 0;
}

void TileViewer::setInfo(ServerInfo *i) {
  info = i;
}

void TileViewer::setCache(TileCache *c) {
  cache = c;
  cache->setAutoNeighbors(true);
  connect(cache, SIGNAL(loaded(TileID)),
          this, SLOT(update()));
}

void TileViewer::setScale(int a1) {
  int x0 = x();
  int y0 = y();
  a = a1;
  enforceA();
  setPosition(x0, y0);
  emit scaleChanged(a);
  update();
}

void TileViewer::setZ(int z1) {
  z_ = z1;
  enforceZ();
  reportView();
  update();
}

int TileViewer::x() const {
  return x_ + (width()<<a)/2;
}

int TileViewer::y() const {
  return y_ + (height()<<a)/2;
}


int TileViewer::visibleXRange() const {
  return width() << a;
}

int TileViewer::visibleYRange() const {
  return height() << a;
}

void TileViewer::stepX(int dx) {
  x_ += dx;
  enforceX();
  reportView();
  update();
}

void TileViewer::stepY(int dy) {
  y_ += dy;
  enforceY();
  reportView();
  update();
}

void TileViewer::stepZ(int dz) {
  z_ += dz;
  enforceZ();
  reportView();
  update();
}

void TileViewer::setPosition(int x1, int y1) {
  x_ = x1 - (width()<<a)/2;
  y_ = y1 - (height()<<a)/2;
  enforceX();
  enforceY();
  reportView();
  update();
}

void TileViewer::setPosition(int x1, int y1, int z1) {
  x_ = x1 - (width()<<a)/2;
  y_ = y1 - (height()<<a)/2;
  z_ = z1;
  enforceX();
  enforceY();
  enforceZ();
  reportView();
  update();
}

void TileViewer::setX(int x1) {
  x_ = x1 - (width()<<a)/2;
  enforceX();
  reportView();
  update();
}


void TileViewer::setY(int y1) {
  y_ = y1 - (width()<<a)/2;
  enforceY();
  reportView();
  update();
}

ViewInfo TileViewer::currentView() const {
  return ViewInfo(width(), height(), x_, y_, z_, a);
}

int TileViewer::level(float p) const {
  ViewInfo vi(currentView());
  QVector<uint8_t> bits;
  for (int r=vi.r0; r<=vi.r1; r++) {
    for (int c=vi.c0; c<=vi.c1; c++) {
      if (c<0 || r<0)
        continue;
      QImage img0 = cache->getTile(TileID(vi.a, vi.z, r, c));
      if (!img0.isNull()) {
        int L0 = bits.size();
        int W = img0.width();
        int H = img0.height();
        bits.resize(L0 + W*H);
        uint8_t *dst = bits.data() + L0;
        for (int y=0; y<H; y++) {
          uint8_t const *src = img0.scanLine(y);
          /* One would think that
               memcpy(dst, img0.scanLine(y), W);
               dst += W;
             would be good enough.
             However, that fails to take into account that there are undefined
             pixels near the edges. Those are GRAY per ../1000/Gray.h.
             So we have to exclude all of those pixels that are GRAY.
          */
          uint8_t const *end = src + W;
          while (src<end && *src==Gray::GRAY)
            src++;
          while (end>src && end[-1]==Gray::GRAY)
            end--;
          memcpy(dst, src, end-src);
          dst += end-src;
        }
        bits.resize(dst - bits.data());
      }
    }
  }
  int L = bits.size();
  if (L==0)
    return 128;
  int K = floor(p * L);
  if (K>=L)
    --K;
  if (K<0)
    K = 0;
  uint8_t *dst = bits.data();
  std::nth_element(dst, dst + K, dst + L);
  return dst[K];
}

void TileViewer::paintEvent(QPaintEvent *e) {
  if (!cache)
    return;
  ViewInfo vi(currentView());

  QPainter ptr;
  ptr.begin(this);
  for (int r=vi.r0; r<=vi.r1; r++) {
    for (int c=vi.c0; c<=vi.c1; c++) {
      if (c<0 || r<0)
        continue;
      QImage img0 = cache->getTile(TileID(vi.a, vi.z, r, c));
      if (!img0.isNull()) {
        QImage img = img0;
        if (blk>0 || wht<255 || gamma!=1) {
          uint8_t *b = img.bits();
          int L = img.width() * img.height();
          if (lut.isEmpty() || !lutok) {
            lut.resize(256);
            for (int x=0; x<256; x++) {
              float v = x;
              v -= blk;
              v /= (wht-blk);
              v = pow(v, gamma);
              v *= 255;
              if (v<0)
                v = 0;
              else if (v>255)
                v = 255;
              lut[x] = v;
            }
          }
          uint8_t const *lutp = lut.data();
          while (L--) {
            *b = lutp[*b];
            b++;
          }
        }
        if (sharp>0) {
          uint8_t *dst = img.bits();
          uint8_t const *src = img0.bits();
          int X = img.width();
          int Y = img.height();
          for (int y=1; y<Y-1; y++) {
            uint8_t *d = dst + (y*X) + 1;
            uint8_t const *s = src + (y*X) + 1;
            for (int x=1; x<X-1; x++) {
              int v = 256*int(*d);
              v -= sharp * (int(s[X]) + int(s[-X]) + int(s[1]) + int(s[-1])
                            - 4*int(*s));
              v /= 256;
              if (v<0)
                v = 0;
              else if (v>255)
                v = 255;
              *d++ = v;
              s++;
            }
          }
        }
        ptr.drawImage(QPoint((c-vi.c0)*ViewInfo::TILESIZE - vi.dx,
                             (r-vi.r0)*ViewInfo::TILESIZE - vi.dy), img);
      }
    }
  }

  float scl = info->contains("dx") ? info->real("dx") : 0.0055;
  float um_per_px = scl * (1<<a);
  QStringList lbls{"100 nm", "200 nm", "500 nm",
      "1 μm", "2 μm", "5 μm",
      "10 μm", "20 μm", "50 μm",
      "100 μm", "200 μm", "500 μm" };
  QList<double> ums{.1, .2, .5, 1, 2, 5, 10, 20, 50, 100, 200, 500};
  int k = 0;
  int idealpix = 100;
  while (k<ums.size()-1 && um_per_px*idealpix > ums[k])
    k++;
  int px = ums[k] / um_per_px;
  QString lbl = lbls[k];
  ptr.setPen(QPen(QColor(0, 0, 0, 60), 10));
  ptr.drawLine(QPoint(width() - px - 20, height() - 100),
               QPoint(width() - 20, height() - 100));
  ptr.setPen(QPen(QColor(0, 0, 0, 10), 10));
  QFont f = ptr.font();
  f.setPointSizeF(f.pointSizeF()*1.3);
  ptr.setFont(f);
  for (int dx=-3; dx<=3; dx++)
    for (int dy=-3; dy<=3; dy++)
      ptr.drawText(QRect(QPoint(width() - px - 20+dx, height() - 80 + dy),
			 QSize(px, 70)), Qt::AlignHCenter | Qt::AlignTop, lbl);
  ptr.setPen(QPen(QColor(128, 255, 255), 6));
  ptr.drawLine(QPoint(width() - px - 20, height() - 100),
               QPoint(width() - 20, height() - 100));
  ptr.drawText(QRect(QPoint(width() - px - 20, height() - 80),
                     QSize(px, 70)), Qt::AlignHCenter | Qt::AlignTop, lbl);

  for (auto *o: overlays)
    o->paint(&ptr, e->rect(), vi);
}

void TileViewer::resizeEvent(QResizeEvent *) {
  update();
}

void TileViewer::mousePressEvent(QMouseEvent *e) {
  e->accept();
  if (!(e->modifiers() & Qt::ControlModifier))
    for (auto *o: overlays)
      if (o->mousePress(mapToSBEM(e->pos()), e->button(), e->modifiers(), a))
	return;

  lastpx = e->x();
  lastpy = e->y();
  isdrag = true;
}

void TileViewer::mouseMoveEvent(QMouseEvent *e) {
  e->accept();

  if (isdrag) {
    x_ -= (e->x() - lastpx) << a;
    y_ -= (e->y() - lastpy) << a;
    lastpx = e->x();
    lastpy = e->y();
    update();
  } else {
    if (mode!=Mode_View && !(e->modifiers() & Qt::ControlModifier))
      for (auto *o: overlays)
	if (o->mouseMove(mapToSBEM(e->pos()), e->buttons(), e->modifiers(), a))
	  break;
  }

  emit viewChanged(x_ + (e->x()<<a),
                   y_ + (e->y()<<a),
                   z_,
                   true);
}


Point TileViewer::mapToSBEM(QPoint p) const {
  return Point(x_ + (p.x()<<a),
	       y_ + (p.y()<<a),
	       z_);
}

void TileViewer::reportView() {
  emit viewChanged(x_ + (width()<<a)/2,
                   y_ + (height()<<a)/2,
                   z_,
                   false);
}

void TileViewer::enterEvent(QEvent *) {
}

void TileViewer::leaveEvent(QEvent *) {
  reportView();
}

void TileViewer::mouseReleaseEvent(QMouseEvent *e) {
  isdrag = false;
  e->accept();
  if (mode!=Mode_View && !(e->modifiers() & Qt::ControlModifier))
    for (auto *o: overlays)
      if (o->mouseRelease(mapToSBEM(e->pos()), e->button(), e->modifiers(), a))
	break;
}

void TileViewer::keyPressEvent(QKeyEvent *e) {
  switch (e->key()) {
  case Qt::Key_Plus: case Qt::Key_Equal: {
    int xc = x_ + (width()<<a) / 2;
    int yc = y_ + (height()<<a) / 2;
    a--;
    enforceA();
    emit scaleChanged(a);
    x_ = xc - (width()<<a) / 2;
    y_ = yc - (height()<<a) / 2;

    
    update();
  } break;
  case Qt::Key_Minus: {
    int xc = x_ + (width()<<a) / 2;
    int yc = y_ + (height()<<a) / 2;
    a++;
    enforceA();
    emit scaleChanged(a);
    x_ = xc - (width()<<a) / 2;
    y_ = yc - (height()<<a) / 2;
    update();
  } break;
  case Qt::Key_Up:
    if (e->modifiers() & Qt::ShiftModifier) 
      z_ -= 10;
    else 
      z_ -= 1;
    enforceZ();
    reportView();
    update();
    break;
  case Qt::Key_Down:
    if (e->modifiers() & Qt::ShiftModifier) 
      z_ += 10;
    else 
      z_ += 1;
    enforceZ();
    reportView();
    update();
    break;
  case Qt::Key_PageUp:
    if (e->modifiers() & Qt::ShiftModifier) 
      z_ -= 1000;
    else
      z_ -= 100;
    enforceZ();
    reportView();
    update();
    break;
  case Qt::Key_PageDown:
    if (e->modifiers() & Qt::ShiftModifier) 
      z_ += 1000;
    else 
      z_ += 100;
    enforceZ();
    reportView();
    update();
    break;
  default:
    for (auto *o: overlays)
      if (o->keyPress(e))
	break;
    break;
  }
  e->accept();
}

void TileViewer::wheelEvent(QWheelEvent *e) {
  angleaccum += e->angleDelta().y();
  int step = round(angleaccum / 120.0);
  angleaccum -= step*120;
  if (e->modifiers() & Qt::ControlModifier) {
    int xc = x_ + (e->x()<<a);
    int yc = y_ + (e->y()<<a);
    if (step>0)
      a--;
    else if (step<0) 
      a++;
    enforceA();
    x_ = xc - (e->x()<<a);
    y_ = yc - (e->y()<<a);
    emit scaleChanged(a);
  } else {
    if (step>0) 
      z_ += (e->modifiers() & Qt::ShiftModifier) ? 10 : 1;
    else if (step<0 && z_>0)
      z_ -= (e->modifiers() & Qt::ShiftModifier) ? 10 : 1;
    enforceZ();
  }
  emit viewChanged(x_ + (e->x()<<a),
                   y_ + (e->y()<<a),
                   z_,
                   true);
  update();
  e->accept();
}

void TileViewer::enforceX() {
  int x0 = info->contains("x0") ? info->integer("x0") : 15000;
  int x1 = info->contains("x1") ? info->integer("x1") : 50000;
  x1 -= (9*width()/10) << a;
  x0 -= (width()/10) << a;
  if (x_<x0)
    x_ = x0;
  if (x_>x1)
    x_ = x1;
}

void TileViewer::enforceY() {
  int y0 = info->contains("y0") ? info->integer("y0") : 50000;
  int y1 = info->contains("y1") ? info->integer("y1") : 99000;
  y1 -= (9*height()/10) << a;
  y0 -= (height()/10) << a;
  qDebug() << "enforceY" << y_ << y0 << y1;
  if (y_<y0)
    y_ = y0;
  if (y_>y1)
    y_ = y1;
  qDebug() << " => " << y_;
}
  
void TileViewer::enforceZ() {
  int z0 = info->contains("z0") ? info->integer("z0") : 0;
  int z1 = info->contains("z1") ? info->integer("z1") : 99000;
  if (z_<z0)
    z_ = z0;
  
  if (z_>z1) {
    z_ = z1;
    cache->notifyNewSliceAvailable();
    info->update();
  }
}
  
void TileViewer::enforceA() {
  int a0 = 0;
  int a1 = info->contains("amax") ? info->integer("amax") : 8;
  if (a<a0)
    a = a0;
  if (a>a1)
    a = a1;
}
  
void TileViewer::setBlackLevel(int v) {
  blk = v;
  lutok = false;
  update();
}

void TileViewer::setWhiteLevel(int v) {
  wht = v;
  lutok = false;
  update();
}

void TileViewer::setGamma(float g) {
  gamma = g;
  lutok = false;
  update();
}

void TileViewer::setSharpening(float s) {
  sharp = 255*s;
  update();
}

void TileViewer::addOverlay(Overlay *o) {
  overlays << o;
  o->setMode(mode);
}

void TileViewer::removeOverlay(Overlay *o) {
  overlays.removeAll(o);
}


void TileViewer::setMode(Mode m) {
  mode = m;
  for (auto *o: overlays)
    o->setMode(m);
}
