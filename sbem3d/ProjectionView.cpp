// ProjectionView.cpp

#include "ProjectionView.h"
#include <math.h>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QMouseEvent>
#include <QWheelEvent>

class Obj3D {
public:
  Obj3D(): isline(false) {}
  Obj3D(LineF const &l, QColor c): p1(l.p1), p2(l.p2), c(c), isline(true) {}
  Obj3D(PointF const &p, double r, QColor c): p1(p), c(c), r(r), isline(false) {}
  double z() const { return isline ? (p1.z + p2.z)/2 : p1.z; }
public:
  PointF p1;
  PointF p2;
  QColor c;
  double r;
  bool isline;
};

class ProjectionViewData {
public:
  ProjectionViewData() {
    freezedepth = 0;
    xneg = "V";
    xpos = "D";
    yneg = "R";
    ypos = "L";
    zneg = "A";
    zpos = "P";
    setTransform(Transform3());
    havecenter = false;
    havexf = false;
  }
  void doXForm();
  void setTransform(Transform3 const &tf);
  void drawDirectionNames(QPainter &, double w, double h);
  void findCenter(); // calculates pavg, dmax
  void autoCenter(int x, int y);
  void autoScale(int w, int h);
  void mapCenter();
public:
  Transform3 tform, invtform;
  QPoint presspt, lastpt;
  QMap< int, QVector<LineF> > lines; // in um
  QMap< int, QVector<PointF> > points; // in um
  QMap< int, PointF > nearColor;
  QMap< int, float > pointSize;
  QString xneg, xpos, yneg, ypos, zneg, zpos;
  int freezedepth;
  // following are calced by doXForm
  QMap< int, QVector<LineF> > xformedlines;
  QMap< int, QVector<PointF> > xformedpoints;
  bool havecenter, havexf, resc;
  PointF pavg, xfpavg;
  double dmax, xfdmax;
};

void ProjectionViewData::setTransform(Transform3 const &t) {
  tform = t;
  invtform = tform.inverse();
  havecenter = false;
  havexf = false;
  resc = true;
}

void ProjectionViewData::doXForm() {
  xformedlines.clear();
  xformedpoints.clear();
  
  // Transform all line series
  for (int k: lines.keys()) {
    QVector<LineF> v = lines[k];
    for (LineF &l: v)
      l = tform.apply(l);
    xformedlines[k] = v;
  }

  // Transform all point series
  for (int k: points.keys()) {
    QVector<PointF> v = points[k];
    for (PointF &p: v) 
      p = tform.apply(p);
    xformedpoints[k] = v;
  }

  if (!havecenter)
    findCenter();
  else
    mapCenter();
  havexf = true;
}

void ProjectionViewData::mapCenter() {
  xfpavg = tform.apply(pavg);
  PointF dp(dmax, 0, 0);
  xfdmax = sqrt((tform.apply(pavg+dp) - xfpavg).L2());
}

void ProjectionViewData::autoScale(int w, int h) {
  if (!havecenter)
    findCenter();
  else
    mapCenter();
  int r = (w<h ? w : h) / 2;
  qDebug() << "autoscale pre " << pavg << dmax << xfpavg << xfdmax << r;
  qDebug() << tform;
  tform = Transform3::scaler(r / xfdmax) * tform;
  invtform = tform.inverse();
  mapCenter();
  havexf = false;
  resc = false;
  qDebug() << "autoscale post " << pavg << dmax << xfpavg << xfdmax;
  qDebug() << tform;
}

void ProjectionViewData::autoCenter(int x0, int y0) {
  if (!havecenter)
    findCenter();
  PointF p1(x0, y0, 0);
  qDebug() << "autocenter pre " << pavg << dmax << xfpavg << xfdmax << x0 << y0;
  tform = Transform3::shifter(x0 - xfpavg.x, y0 - xfpavg.y, 0 - xfpavg.z) * tform;
  invtform = tform.inverse();
  mapCenter();
  havexf = false;
  resc = false;
  qDebug() << "autocenter post " << pavg << dmax << xfpavg << xfdmax;
}

void ProjectionViewData::findCenter() {
  // Calculate center based on line series only
  pavg = PointF();
  int N = 0;
  for (int k: lines.keys()) {
    QVector<LineF> const &v = lines[k];
    for (LineF const &l: v) {
      pavg += l.p1;
      pavg += l.p2;
    }
    N += 2*v.size();
  }
  pavg /= N;

  // Calculate extent based on line series only
  double d2max = 0;
  for (int k: lines.keys()) {
    QVector<LineF> const &v = lines[k];
    for (LineF const &l: v) {
      double d2 = (l.p1 - pavg).L2();
      if (d2>d2max)
        d2max = d2;
      d2 = (l.p2 - pavg).L2();
      if (d2>d2max)
        d2max = d2;
    }
  }
  dmax = sqrt(d2max);

  mapCenter();
  havecenter = true;
}

void ProjectionViewData::drawDirectionNames(QPainter &p, double w, double h) {
  double r = w<h ? w : h;
  
  // Draw direction names
  PointF px = .08*r*tform.apply(PointF(1, 0, 0));
  PointF py = .08*r*tform.apply(PointF(0, 1, 0));
  PointF pz = .08*r*tform.apply(PointF(0, 0, 1));
  auto penForZ = [r](double z) {
    z = (z/(.08*r))/2 + .5;
    qreal g = .9*z;
    return QPen(QColor::fromRgbF(g,g,g));
  };

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
}

ProjectionView::ProjectionView(QWidget *parent): QWidget(parent),
						 d(new ProjectionViewData) {
  setFocusPolicy(Qt::StrongFocus);
  QPalette pal = palette();
  pal.setColor(QPalette::Background, Qt::white);
  setPalette(pal);
  setAutoFillBackground(true);
}

ProjectionView::~ProjectionView() {
  delete d;
}

void ProjectionView::freeze() {
  d->freezedepth++;
}

void ProjectionView::thaw() {
  d->freezedepth--;
}

bool ProjectionView::frozen() const {
  return d->freezedepth>0;
}

void ProjectionView::setXAxisLabels(QString xn, QString xp) {
  d->xneg = xn;
  d->xpos = xp;
  if (!frozen())
    update();
}

void ProjectionView::setYAxisLabels(QString yn, QString yp) {
  d->yneg = yn;
  d->ypos = yp;
  if (!frozen())
    update();
}

void ProjectionView::setZAxisLabels(QString zn, QString zp) {
  d->zneg = zn;
  d->zpos = zp;
  if (!frozen())
    update();
}

void ProjectionView::setLines(int tid, QVector<LineF> l) {
  d->havecenter = false;
  d->havexf = false;
  d->lines[tid] = l;
  d->resc = true;
  if (!frozen()) 
    update();
}

void ProjectionView::setPoints(int tid, QVector<PointF> p) {
  d->havexf = false;
  d->points[tid] = p;
  d->resc = true;
  if (!frozen()) 
    update();
}

PointF ProjectionView::color(int tid) const {
  if (d->nearColor.contains(tid))
    return d->nearColor[tid];
  else
    return PointF();
}

void ProjectionView::setColor(int tid, PointF cnear) {
  d->nearColor[tid] = cnear;
  if (!frozen())
    update();
}     

void ProjectionView::setPointSize(int tid, float ps) {
  d->pointSize[tid] = ps;
}

void ProjectionView::mousePressEvent(QMouseEvent *e) {
  d->lastpt = d->presspt = e->pos();
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
  case Qt::Key_X: d->setTransform(Transform3(xform)); update(); break;
  case Qt::Key_Y: d->setTransform(Transform3(yform)); update(); break;
  case Qt::Key_Z: d->setTransform(Transform3(zform)); update(); break;
  }
}

void ProjectionView::wheelEvent(QWheelEvent *e) {
  QPoint delta = e->angleDelta();
  QPointF pos = e->pos();
  d->invtform.scale(exp(-delta.y()/200.), pos.x(), pos.y());
  d->tform = d->invtform.inverse();
  d->havexf = false;
  e->accept();
  if (!frozen())
    update();
}

void ProjectionView::mouseMoveEvent(QMouseEvent *e) {
  double dx = (e->pos().x() - d->lastpt.x()) * 4.0 / width();
  double dy = -(e->pos().y() - d->lastpt.y()) * 4.0 / height();

  d->havexf = false;
  //  d->invtform.rotate(dx, dy, d->presspt.x(), d->presspt.y());
  d->invtform.rotate(dx, dy, width()/2, height()/2);
  d->tform = d->invtform.inverse();
  d->lastpt = e->pos();
  e->accept();
  if (!frozen())
    update();
}

void ProjectionView::paintEvent(QPaintEvent *) {
  if (!d->havecenter)
    d->findCenter();
  if (d->resc) {
    d->autoScale(width(), height());
    d->autoCenter(width()/2, height()/2);
  }
  if (!d->havexf)
    d->doXForm();

  double zmin = d->xfpavg.z - d->xfdmax;
  double zmax = d->xfpavg.z + d->xfdmax;

  QPainter p;
  p.begin(this);

  d->drawDirectionNames(p, width(), height());

  // CLIP clips a (color) value to [0, 1]
  auto clip = [](qreal x) { return x<0 ? 0 : x>1 ? 1 : x; };

  QList<Obj3D> objs;
  
  // Draw all line series
  for (int k: d->xformedlines.keys()) {
    QVector<LineF> const &v = d->xformedlines[k];
    PointF cn = d->nearColor[k];
    for (auto const &l: v) {
      double zf = ((l.p1.z + l.p2.z)/2 - zmin) / (1e-9 + zmax - zmin) * .8;
      double zn = 1 - zf;
      qreal r = clip(cn.x*zn + zf);
      qreal g = clip(cn.y*zn + zf);
      qreal b = clip(cn.z*zn + zf);
      QColor c(QColor::fromRgbF(r,g,b));
      //      c.setAlphaF(.5);
      objs << Obj3D(l,c); 
    }
  }

  // Draw all point series
  for (int k: d->xformedpoints.keys()) {
    QVector<PointF> const &v = d->xformedpoints[k];
    PointF cn = d->nearColor[k];
    float radius = d->pointSize.contains(k) ? d->pointSize[k] : 3;
    for (auto const &q: v) {
      double zf = (q.z - zmin) / (1e-9 + zmax - zmin) * .8;
      double zn = 1 - zf;
      qreal r = clip(cn.x*zn + zf);
      qreal g = clip(cn.y*zn + zf);
      qreal b = clip(cn.z*zn + zf);
      QColor c(QColor::fromRgbF(r,g,b));
      if (radius>3)
	c.setAlphaF(.5);
      objs << Obj3D(q, radius*d->xfdmax/d->dmax, c);
    }
  }
  
  auto objorderfoo = [this](Obj3D const &a, Obj3D const &b) {
    return a.z() > b.z();
  };

  qSort(objs.begin(), objs.end(), objorderfoo);


  for (Obj3D const &obj: objs) {
    if (obj.isline) {
      p.setPen(QPen(obj.c, 2));
      p.drawLine(QPointF(obj.p1.x, obj.p1.y),
                 QPointF(obj.p2.x, obj.p2.y));
    } else {
      p.setPen(QPen(Qt::NoPen));
      p.setBrush(obj.c);
      p.drawEllipse(QPointF(obj.p1.x, obj.p1.y),
                    obj.r, obj.r);
    }
  }
}

void ProjectionView::resizeEvent(QResizeEvent *) {
  update();
}

