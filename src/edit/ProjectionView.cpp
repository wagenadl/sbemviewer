// ProjectionView.cpp

#include "ProjectionView.h"
#include <math.h>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QMouseEvent>

void cpmat(double const a[3][3], double b[3][3]) {
  for (int k=0; k<3; k++)
    for (int n=0; n<3; n++)
      b[k][n] = a[k][n];
};

void matmul(double const a[3][3], double const b[3][3], double c[3][3]) {
  for (int k=0; k<3; k++) {
    for (int m=0; m<3; m++) {
      double v = 0;
      for (int n=0; n<3; n++)
        v += a[k][n] * b[n][m];
      c[k][m] = v;
    }
  }
}

ProjectionView::ProjectionView(QWidget *parent): QWidget(parent) {
  setFocusPolicy(Qt::StrongFocus);
  
  xneg = "V";
  xpos = "D";
  yneg = "L?";
  ypos = "R?";
  zneg = "A";
  zpos = "P";

  double xform[3][3] = {0, 1, 0,
                        0, 0, 1,
                        1, 0, 0};
  cpmat(xform, tform);
}

ProjectionView::~ProjectionView() {
}

void ProjectionView::setXAxisLabels(QString xn, QString xp) {
  xneg = xn;
  xpos = xp;
  update();
}

void ProjectionView::setYAxisLabels(QString yn, QString yp) {
  yneg = yn;
  ypos = yp;
  update();
}

void ProjectionView::setZAxisLabels(QString zn, QString zp) {
  zneg = zn;
  zpos = zp;
  update();
}

void ProjectionView::setTree(int tid, QVector<LineF> l) {
  trees[tid] = l;
  update();
}

void ProjectionView::setColors(int tid, QColor cnear, QColor cfar) {
  nearColor[tid] = cnear;
  farColor[tid] = cfar;
  update();
}     


void ProjectionView::mousePressEvent(QMouseEvent *e) {
  presspt = e->pos();
  e->accept();
}

void ProjectionView::keyPressEvent(QKeyEvent *e) {
  double yform[3][3] = {1, 0, 0,
                        0, 0, 1,
                        0, 1, 0};
  double zform[3][3] = {0, 1, 0,
                        1, 0, 0,
                        0, 0, 1};
  double xform[3][3] = {0, 1, 0,
                        0, 0, 1,
                        1, 0, 0};
  switch (e->key()) {
  case Qt::Key_X: cpmat(xform, tform); update(); break;
  case Qt::Key_Y: cpmat(yform, tform); update(); break;
  case Qt::Key_Z: cpmat(zform, tform); update(); break;
  }
}
 
void ProjectionView::mouseMoveEvent(QMouseEvent *e) {
  double dx = (e->pos().x() - presspt.x()) * 4.0 / width();
  double dy = (e->pos().y() - presspt.y()) * 4.0 / height();

  double sx = sin(dy);
  double cx = cos(dy);
  double xform[3][3] = {1, 0,   0,
                        0, cx,-sx,
                        0, sx, cx};
  // positive phix maps positive y to positive z
  double sy = sin(dx);
  double cy = cos(dx);
  double yform[3][3] = {cy,  0, sy,
                        0,   1, 0,
                        -sy, 0, cy};
  
  double qq[3][3];
  matmul(xform, tform, qq);
  matmul(yform, qq, tform);
  
  presspt = e->pos();
  e->accept();
  update();
}

void ProjectionView::paintEvent(QPaintEvent *) {
  QMap< int, QVector<LineF> > xformedtrees;

  auto map = [this](PointF p) {
    PointF q;
    q.x = tform[0][0]*p.x + tform[0][1]*p.y + tform[0][2]*p.z;
    q.y = tform[1][0]*p.x + tform[1][1]*p.y + tform[1][2]*p.z;
    q.z = tform[2][0]*p.x + tform[2][1]*p.y + tform[2][2]*p.z;
    return q;
  };
  
  for (int k: trees.keys()) {
    QVector<LineF> v = trees[k];
    for (LineF &l: v) {
      l.p1 = map(l.p1);
      l.p2 = map(l.p2);
    }
    xformedtrees[k] = v;
  }

  PointF pavg;
  int N = 0;
  for (int k: trees.keys()) {
    QVector<LineF> const &v = xformedtrees[k];
    for (LineF const &l: v) {
      pavg += l.p1;
      pavg += l.p2;
      N++;
      N++;
    }
  }
  pavg /= N;
  double d2max = 0;
  for (int k: trees.keys()) {
    QVector<LineF> const &v = xformedtrees[k];
    for (LineF const &l: v) {
      double d2 = (l.p1 - pavg).L2();
      if (d2>d2max)
        d2max = d2;
      d2 = (l.p2 - pavg).L2();
      if (d2>d2max)
        d2max = d2;
    }
  }
  double dmax = sqrt(d2max);
  
  double xmin = pavg.x - dmax;
  double xmax = pavg.x + dmax;
  double ymin = pavg.y - dmax;
  double ymax = pavg.y + dmax;
  double zmin = pavg.z - dmax;
  double zmax = pavg.z + dmax;

  int w = width();
  int h = height();
  double xrat = w / (xmax - xmin);
  double yrat = h / (ymax - ymin);
  double scale = .98*(xrat < yrat ? xrat : yrat);
  int r = w<h ? w : h;
  double x0 = .5*w - .49*r;
  double y0 = .5*h - .49*r;

  PointF px = .08*r*map(PointF(1, 0, 0));
  PointF py = .08*r*map(PointF(0, 1, 0));
  PointF pz = .08*r*map(PointF(0, 0, 1));

  QPainter p;
  p.begin(this);
  
  p.drawText(.12*w - px.x, .12*h - px.y, xneg);
  p.drawText(.12*w + px.x, .12*h + px.y, xpos);
  p.drawText(.12*w - py.x, .12*h - py.y, yneg);
  p.drawText(.12*w + py.x, .12*h + py.y, ypos);
  p.drawText(.12*w - pz.x, .12*h - pz.y, zneg);
  p.drawText(.12*w + pz.x, .12*h + pz.y, zpos);

  for (int k: xformedtrees.keys()) {
    QVector<LineF> const &v = xformedtrees[k];
    qreal rf, gf, bf;
    qreal rn, gn, bn;
    farColor[k].getRgbF(&rf, &gf, &bf);
    nearColor[k].getRgbF(&rn, &gn, &bn);
    for (auto const &l: v) {
      double z = ((l.p1.z + l.p2.z)/2 - zmin) / (1e-9 + zmax - zmin);
      p.setPen(QPen(QColor::fromRgbF(z*rn + (1-z)*rf,
                                     z*gn + (1-z)*gf,
                                     z*bn + (1-z)*bf), 1.5));
      p.drawLine(QPointF(x0+scale*(l.p1.x - xmin),
                         y0+scale*(l.p1.y - ymin)),
                 QPointF(x0+scale*(l.p2.x - xmin),
                         y0+scale*(l.p2.y - ymin)));
    }
  }
}

void ProjectionView::resizeEvent(QResizeEvent *) {
  update();
}

