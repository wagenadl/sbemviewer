// ProjectionWidget.cpp

#include "ProjectionWidget.h"
#include "ProjectionView.h"
#include "LineF.h"
#include "ServerInfo.h"
#include "DistinctColors.h"

#include <QRegularExpression>

class ProjectionData {
public:
  ProjectionData(SBEMDB *db): db(db), info(db->serverInfo()) {
    haveSelectedTree = false;
    dx = info.real("dx");
    dy = info.real("dy");
    dz = info.real("dz");
    QVariantList gs = info.value("gapshift").toList();
    while (gs.size() >= 3) {
      double z = gs.takeFirst().toDouble();
      double dx = gs.takeFirst().toDouble();
      double dy = gs.takeFirst().toDouble();
      gapshifts << PointF(dx, dy, z);
    }
    qDebug() << "gapshifts: " << gapshifts;
    if (!gs.isEmpty())
      qDebug() << "uneven gapshift data";
  }
  ~ProjectionData() {
  }
  void buildTree(quint64 tid);
  void applyGapShift(PointF &p) {
    for (PointF const &gs: gapshifts) {
      if (p.z>=gs.z) {
	p.x += gs.x;
	p.y += gs.y;
      }
    }
  }
public:
  SBEMDB *db;
  ServerInfo info;
  class ProjectionView *view;
  bool haveSelectedTree;
  double dx, dy, dz;
  QList<PointF> gapshifts;
};

void ProjectionData::buildTree(quint64 tid) {
  view->freeze();
  bool isSelected = tid==db->selectedTree();
  if (isSelected)
    haveSelectedTree = true;

  QString tname = db->simpleQuery("select tname from trees where tid==:a", tid)
    .toString();

  view->setName(tid, tname);
  
  int uctid = 0;
  auto mtch = QRegularExpression("^(\\d+)[^\\.]").match(tname);
  if (mtch.hasMatch()) 
    uctid = mtch.captured(1).toInt();
  if (uctid==0) {
    auto mtch = QRegularExpression("1\\.pre[sp]\\.p\\.(\\d+)").match(tname);
    if (mtch.hasMatch())
      uctid = mtch.captured(1).toInt() + 400;
  }

  QVector<LineF> ll;
  QSqlQuery q = db->constQuery("select n1.x, n1.y, n1.z, n2.x, n2.y, n2.z"
                               " from nodecons as nc"
                               " inner join nodes as n1 on n1.nid==nc.nid1"
                               " inner join nodes as n2 on n2.nid==nc.nid2"
                               " where n1.tid==:a and n1.nid<n2.nid", tid);
  while (q.next()) {
    PointF p1(q.value(0).toInt()*dx,
	      q.value(1).toInt()*dy,
	      q.value(2).toInt()*dz);
    PointF p2(q.value(3).toInt()*dx,
                       q.value(4).toInt()*dy,
	      q.value(5).toInt()*dz);
    applyGapShift(p1);
    applyGapShift(p2);
    ll << LineF(p1, p2);
  }
  view->setLines(tid, ll);

  //  if (isSelected)
  //    view->setColor(tid, PointF(.5, -.5, .5));
  //  else
  //    view->setColor(tid, PointF(-.25, .5, -.25));
  uint32_t c = DistinctColors::instance().color(uctid);
  PointF col((c&0xff0000)/65536/255.*1.5-.5,
	     (c&0x00ff00)/256/255.*1.5-.5,
	     (c&0x0000ff)/1/255.*1.5-.5);
  view->setColor(tid, isSelected ? PointF(-.5, -.5, -.5) : col);
  if (isSelected) {
    // decorate presynaptic nodes
    QVector<PointF> ll;
    QSqlQuery q = db->constQuery("select x, y, z from nodes"
                                 " where tid==:a and typ==:b",
                                 tid, SBEMDB::PresynTerm);
    while (q.next()) {
      PointF p(q.value(0).toInt()*dx,
	       q.value(1).toInt()*dy,
	       q.value(2).toInt()*dz);
      applyGapShift(p);
      ll << p;
    }
    view->setPoints(100000000+tid, ll);
    view->setColor(100000000+tid, isSelected ? PointF(.5, -.25, -.25) : col);
    view->setPointSize(100000000+tid, 4);
  }
  if (isSelected) {
    // decorate postsynaptic nodes
    QVector<PointF> ll;
    QSqlQuery q = db->constQuery("select x, y, z from nodes"
                                 " where tid==:a and typ==:b",
                                 tid, SBEMDB::PostsynTerm);
    while (q.next()) {
      PointF p(q.value(0).toInt()*dx,
	       q.value(1).toInt()*dy,
	       q.value(2).toInt()*dz);
      applyGapShift(p);
      ll << p;
    }
    view->setPoints(200000000+tid, ll);
    view->setColor(200000000+tid, isSelected ? PointF(-.25, -.25, .5) : col);
    view->setPointSize(200000000+tid, 4);
  }
  { // decorate somata
    QVector<PointF> ll;
    QSqlQuery q = db->constQuery("select x, y, z from nodes"
                                 " where tid==:a and typ==:b",
                                 tid, SBEMDB::Soma);
    while (q.next()) {
      PointF p(q.value(0).toInt()*dx,
	       q.value(1).toInt()*dy,
	       q.value(2).toInt()*dz);
      applyGapShift(p);
      ll << p;
    }
    view->setPoints(300000000+tid, ll);
    view->setColor(300000000+tid, view->color(tid));
    view->setPointSize(300000000+tid, 20);
  }
  { // decorate exit points
    QVector<PointF> ll;
    QSqlQuery q = db->constQuery("select x, y, z from nodes"
                                 " where tid==:a and typ==:b",
                                 tid, SBEMDB::ExitPoint);
    while (q.next()) {
      PointF p(q.value(0).toInt()*dx,
	       q.value(1).toInt()*dy,
	       q.value(2).toInt()*dz);
      applyGapShift(p);
      ll << p;
    }
    view->setPoints(400000000+tid, ll);
    view->setColor(400000000+tid, view->color(tid));
    view->setPointSize(400000000+tid, 12);
  }
    
  view->thaw();
}
  
ProjectionWidget::ProjectionWidget(SBEMDB *db, QWidget *parent):
  QMainWindow(parent), d(new ProjectionData(db)) {
  d->view = new ProjectionView(this);
  setCentralWidget(d->view);
  connect(d->view, &ProjectionView::doubleClickOnTree,
	  [this](int tid, PointF pos) {
	    emit doubleClickOnTree(tid, pos.x, pos.y, pos.z);
	  });
  setWindowTitle("SBEM Viewer 3D Projection");
}

ProjectionWidget::~ProjectionWidget() {
  delete d;
}

void ProjectionWidget::addTree(quint64 tid) {
  d->buildTree(tid);
}

void ProjectionWidget::updateShownTrees() {
  d->view->freeze();
  clear();
  addVisibleTrees();
  addSelectedTree();
  d->view->thaw();
}

void ProjectionWidget::clear() {
  d->view->clear();
}

void ProjectionWidget::addVisibleTrees() {
  d->view->freeze();
  QSqlQuery q = d->db->constQuery("select tid from trees where visible");
  while (q.next())
    addTree(q.value(0).toULongLong());
  d->view->thaw();
}

void ProjectionWidget::addSelectedTree() {
  addTree(d->db->selectedTree());
}

