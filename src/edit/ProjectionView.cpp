// ProjectionView.cpp

#include "ProjectionView.h"
#include <math.h>
#include <QDebug>
#include <QPainter>
#include <QPen>

ProjectionView::ProjectionView(QWidget *parent): QWidget(parent) {
  phix = 0;
  phiy = 0;
  xneg = "x";
  xpos = "X";
  yneg = "y";
  ypos = "Y";
  zneg = "z";
  zpos = "Z";
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

void ProjectionView::setYRotation(int p) {
  phiy = p*3.141592/180;
  update();
}

void ProjectionView::setXRotation(int p) {
  phix = p*3.141592/180;
  update();
}

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

void ProjectionView::paintEvent(QPaintEvent *) {
  QMap< int, QVector<LineF> > xformedtrees;

  double sx = sin(phix);
  double cx = cos(phix);
  double xform[3][3] = {1, 0,   0,
                        0, cx,-sx,
                        0, sx, cx};
  // positive phix maps positive y to positive z
  double sy = sin(phiy);
  double cy = cos(phiy);
  double yform[3][3] = {cy,  0, sy,
                        0,   1, 0,
                        -sy, 0, cy};
  // positive phiy maps positive z to positive x

  double tform[3][3];
  matmul(yform, xform, tform);

  auto map = [tform](PointF p) {
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
  double scale = .9*(xrat < yrat ? xrat : yrat);
  double x0 = .05*w;
  double y0 = .05*h;

  PointF px = .05*w*map(PointF(1, 0, 0));
  PointF py = .05*w*map(PointF(0, 1, 0));
  PointF pz = .05*w*map(PointF(0, 0, 1));

  QPainter p;
  p.begin(this);
  
  if (pz.L2xy() < py.L2xy() && pz.L2xy() < px.L2xy()) {
    // show x and y labels
    p.drawText(.1*w - px.x, .1*w - px.y, xneg);
    p.drawText(.1*w + px.x, .1*w + px.y, xpos);
    p.drawText(.1*w - py.x, .1*w - py.y, yneg);
    p.drawText(.1*w + py.x, .1*w + py.y, ypos);
  } else if (px.L2xy() < py.L2xy() && px.L2xy() < pz.L2xy()) {
    // show y and z labels
    p.drawText(.1*w - pz.x, .1*w - pz.y, zneg);
    p.drawText(.1*w + pz.x, .1*w + pz.y, zpos);
    p.drawText(.1*w - py.x, .1*w - py.y, yneg);
    p.drawText(.1*w + py.x, .1*w + py.y, ypos);
  } else {
    // show x and z labels
    p.drawText(.1*w - px.x, .1*w - px.y, xneg);
    p.drawText(.1*w + px.x, .1*w + px.y, xpos);
    p.drawText(.1*w - pz.x, .1*w - pz.y, zneg);
    p.drawText(.1*w + pz.x, .1*w + pz.y, zpos);
  }

  for (int k: xformedtrees.keys()) {
    QVector<LineF> const &v = xformedtrees[k];
    QColor cnear = nearColor[k];
    QColor cfar = farColor[k];
    for (auto const &l: v) {
      double z = ((l.p1.z + l.p2.z)/2 - zmin) / (1e-9 + zmax - zmin);
      p.setPen(QPen(cnear, 1.5)); // hmm
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

