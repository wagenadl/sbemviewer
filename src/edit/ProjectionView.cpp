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
  freezedepth = 0;
  setFocusPolicy(Qt::StrongFocus);
  QPalette pal = palette();
  pal.setColor(QPalette::Background, Qt::white);
  setPalette(pal);
  setAutoFillBackground(true);
  xneg = "V";
  xpos = "D";
  yneg = "R";
  ypos = "L";
  zneg = "A";
  zpos = "P";

  double xform[3][3] = {0, 1, 0,
                        0, 0, 1,
                        1, 0, 0};
  cpmat(xform, tform);
}

ProjectionView::~ProjectionView() {
}

void ProjectionView::freeze() {
  freezedepth++;
}

void ProjectionView::thaw() {
  freezedepth--;
}

bool ProjectionView::frozen() const {
  return freezedepth>0;
}

void ProjectionView::setXAxisLabels(QString xn, QString xp) {
  xneg = xn;
  xpos = xp;
  if (!frozen())
    update();
}

void ProjectionView::setYAxisLabels(QString yn, QString yp) {
  yneg = yn;
  ypos = yp;
  if (!frozen())
    update();
}

void ProjectionView::setZAxisLabels(QString zn, QString zp) {
  zneg = zn;
  zpos = zp;
  if (!frozen())
    update();
}

void ProjectionView::setLines(int tid, QVector<LineF> l) {
  lines[tid] = l;
  if (!frozen())
    update();
}

void ProjectionView::setPoints(int tid, QVector<PointF> p) {
  points[tid] = p;
  if (!frozen())
    update();
}

PointF ProjectionView::color(int tid) const {
  if (nearColor.contains(tid))
    return nearColor[tid];
  else
    return PointF();
}

void ProjectionView::setColor(int tid, PointF cnear) {
  nearColor[tid] = cnear;
  if (!frozen())
    update();
}     

void ProjectionView::setPointSize(int tid, float ps) {
  pointSize[tid] = ps;
}

void ProjectionView::mousePressEvent(QMouseEvent *e) {
  presspt = e->pos();
  e->accept();
}

void ProjectionView::keyPressEvent(QKeyEvent *e) {
  double yform[3][3] = {-1, 0, 0,
                         0, 0, 1,
                         0, 1, 0};
  double zform[3][3] = {0, -1, 0,
                        -1, 0, 0,
                        0, 0, -1};
  double xform[3][3] = {0, -1, 0,
                        0,  0, 1,
                        -1, 0, 0};
  switch (e->key()) {
  case Qt::Key_X: cpmat(xform, tform); update(); break;
  case Qt::Key_Y: cpmat(yform, tform); update(); break;
  case Qt::Key_Z: cpmat(zform, tform); update(); break;
  }
}
 
void ProjectionView::mouseMoveEvent(QMouseEvent *e) {
  double dx = -(e->pos().x() - presspt.x()) * 4.0 / width();
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
  QMap< int, QVector<LineF> > xformedlines;
  QMap< int, QVector<PointF> > xformedpoints;

  // MAP maps from source space to rotated space
  auto map = [this](PointF p) {
    PointF q;
    q.x = tform[0][0]*p.x + tform[0][1]*p.y + tform[0][2]*p.z;
    q.y = tform[1][0]*p.x + tform[1][1]*p.y + tform[1][2]*p.z;
    q.z = tform[2][0]*p.x + tform[2][1]*p.y + tform[2][2]*p.z;
    return q;
  };

  // Transform all line series
  for (int k: lines.keys()) {
    QVector<LineF> v = lines[k];
    for (LineF &l: v) {
      l.p1 = map(l.p1);
      l.p2 = map(l.p2);
    }
    xformedlines[k] = v;
  }

  // Transform all point series
  for (int k: points.keys()) {
    QVector<PointF> v = points[k];
    for (PointF &p: v) 
      p = map(p);
    xformedpoints[k] = v;
  }

  // Calculate center based on line series only
  PointF pavg;
  int N = 0;
  for (int k: lines.keys()) {
    QVector<LineF> const &v = xformedlines[k];
    for (LineF const &l: v) {
      pavg += l.p1;
      pavg += l.p2;
      N++;
      N++;
    }
  }
  pavg /= N;

  // Calculate extent based on line series only
  double d2max = 0;
  for (int k: lines.keys()) {
    QVector<LineF> const &v = xformedlines[k];
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

  // Calculate appropriate scaling for widget size
  int w = width();
  int h = height();
  double xrat = w / (xmax - xmin);
  double yrat = h / (ymax - ymin);
  double scale = .98*(xrat < yrat ? xrat : yrat);
  int r = w<h ? w : h;
  double x0 = .5*w - .49*r;
  double y0 = .5*h - .49*r;

  // Draw direction names
  PointF px = .08*r*map(PointF(1, 0, 0));
  PointF py = .08*r*map(PointF(0, 1, 0));
  PointF pz = .08*r*map(PointF(0, 0, 1));
  auto penForZ = [r](double z) {
    z = (z/(.08*r))/2 + .5;
    qreal g = .9*z;
    return QPen(QColor::fromRgbF(g,g,g));
  };
  QPainter p;
  p.begin(this);
  p.setPen(penForZ(-px.z));
  p.drawText(.12*w - px.x, .12*h - px.y, xneg);
  p.setPen(penForZ(px.z));
  p.drawText(.12*w + px.x, .12*h + px.y, xpos);
  p.setPen(penForZ(-py.z));
  p.drawText(.12*w - py.x, .12*h - py.y, yneg);
  p.setPen(penForZ(py.z));
  p.drawText(.12*w + py.x, .12*h + py.y, ypos);
  p.setPen(penForZ(-pz.z));
  p.drawText(.12*w - pz.x, .12*h - pz.y, zneg);
  p.setPen(penForZ(pz.z));
  p.drawText(.12*w + pz.x, .12*h + pz.y, zpos);

  // CLIP clips a (color) value to [0, 1]
  auto clip = [](qreal x) { return x<0 ? 0 : x>1 ? 1 : x; };

  // Draw all line series
  for (int k: xformedlines.keys()) {
    QVector<LineF> const &v = xformedlines[k];
    PointF cn = nearColor[k];
    for (auto const &l: v) {
      double zf = ((l.p1.z + l.p2.z)/2 - zmin) / (1e-9 + zmax - zmin) * .8;
      double zn = 1 - zf;
      qreal r = clip(cn.x*zn + zf);
      qreal g = clip(cn.y*zn + zf);
      qreal b = clip(cn.z*zn + zf);
      p.setPen(QPen(QColor::fromRgbF(r,g,b), 1.5));
      p.drawLine(QPointF(x0+scale*(l.p1.x - xmin),
                         y0+scale*(l.p1.y - ymin)),
                 QPointF(x0+scale*(l.p2.x - xmin),
                         y0+scale*(l.p2.y - ymin)));
    }
  }

  // Draw all point series
  p.setPen(QPen(Qt::NoPen));
  for (int k: xformedpoints.keys()) {
    QVector<PointF> const &v = xformedpoints[k];
    PointF cn = nearColor[k];
    float radius = pointSize.contains(k) ? pointSize[k] : 3;
    for (auto const &q: v) {
      double zf = (q.z - zmin) / (1e-9 + zmax - zmin) * .8;
      double zn = 1 - zf;
      qreal r = clip(cn.x*zn + zf);
      qreal g = clip(cn.y*zn + zf);
      qreal b = clip(cn.z*zn + zf);
      p.setBrush(QBrush(QColor::fromRgbF(r,g,b)));
      p.drawEllipse(QPointF(x0+scale*(q.x - xmin),
                            y0+scale*(q.y - ymin)),
                    radius, radius);
    }
  }
}

void ProjectionView::resizeEvent(QResizeEvent *) {
  update();
}

