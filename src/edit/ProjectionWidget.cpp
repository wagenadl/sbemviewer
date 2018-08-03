// ProjectionWidget.cpp

#include "ProjectionWidget.h"
#include "ui_ProjectionWidget.h"
#include "LineF.h"
#include "ServerInfo.h"

class ProjectionData {
public:
  ProjectionData(ServerInfo *info, SBEMDB *db): info(info), db(db) {
    ui = new Ui_ProjectionWidget;
    haveSelectedTree = false;
    dx = info->contains("dx") ? info->real("dx") : 0.0055;
    dy = info->contains("dy") ? info->real("dy") : 0.0055;
    dz = info->contains("dz") ? info->real("dz") : 0.050;
  }
  ~ProjectionData() {
    delete ui;
  }
  void buildTree(quint64 tid);
public:
  ServerInfo *info;
  SBEMDB *db;
  Ui_ProjectionWidget *ui;
  bool haveSelectedTree;
  double dx, dy, dz;
};

void ProjectionData::buildTree(quint64 tid) {
  ui->widget->freeze();
  bool isSelected = tid==db->selectedTree();
  if (isSelected)
    haveSelectedTree = true;

  QVector<LineF> ll;
  QSqlQuery q = db->constQuery("select n1.x, n1.y, n1.z, n2.x, n2.y, n2.z"
                               " from nodecons as nc"
                               " inner join nodes as n1 on n1.nid==nc.nid1"
                               " inner join nodes as n2 on n2.nid==nc.nid2"
                               " where n1.tid==:a and n1.nid<n2.nid", tid);
  while (q.next())
    ll << LineF(PointF(q.value(0).toInt()*dx,
                       q.value(1).toInt()*dy,
                       q.value(2).toInt()*dz),
                PointF(q.value(3).toInt()*dx,
                       q.value(4).toInt()*dy,
                       q.value(5).toInt()*dz));
  ui->widget->setLines(tid, ll);

  if (isSelected)
    ui->widget->setColor(tid, PointF(.5, -.5, .5));
  else
    ui->widget->setColor(tid, PointF(-.25, .5, -.25));

  if (isSelected) {
    // decorate presynaptic nodes
    QVector<PointF> ll;
    QSqlQuery q = db->constQuery("select x, y, z from nodes"
                                 " where tid==:a and typ==:b",
                                 tid, SBEMDB::PresynTerm);
    while (q.next())
      ll << PointF(q.value(0).toInt()*dx,
                   q.value(1).toInt()*dy,
                   q.value(2).toInt()*dz);
    ui->widget->setPoints(100000000+tid, ll);
    ui->widget->setColor(100000000+tid, PointF(.5, -.25, -.25));
    ui->widget->setPointSize(100000000+tid, 4);
  }
  if (isSelected) {
    // decorate postsynaptic nodes
    QVector<PointF> ll;
    QSqlQuery q = db->constQuery("select x, y, z from nodes"
                                 " where tid==:a and typ==:b",
                                 tid, SBEMDB::PostsynTerm);
    while (q.next())
      ll << PointF(q.value(0).toInt()*dx,
                   q.value(1).toInt()*dy,
                   q.value(2).toInt()*dz);
    ui->widget->setPoints(200000000+tid, ll);
    ui->widget->setColor(200000000+tid, PointF(-.25, -.25, .5));
    ui->widget->setPointSize(200000000+tid, 4);
  }
  { // decorate somata
    QVector<PointF> ll;
    QSqlQuery q = db->constQuery("select x, y, z from nodes"
                                 " where tid==:a and typ==:b",
                                 tid, SBEMDB::Soma);
    while (q.next())
      ll << PointF(q.value(0).toInt()*dx,
                   q.value(1).toInt()*dy,
                   q.value(2).toInt()*dz);
    ui->widget->setPoints(300000000+tid, ll);
    ui->widget->setColor(300000000+tid, ui->widget->color(tid));
    ui->widget->setPointSize(300000000+tid, 20);
  }
  { // decorate exit points
    QVector<PointF> ll;
    QSqlQuery q = db->constQuery("select x, y, z from nodes"
                                 " where tid==:a and typ==:b",
                                 tid, SBEMDB::ExitPoint);
    while (q.next())
      ll << PointF(q.value(0).toInt()*dx,
                   q.value(1).toInt()*dy,
                   q.value(2).toInt()*dz);
    ui->widget->setPoints(400000000+tid, ll);
    ui->widget->setColor(400000000+tid, ui->widget->color(tid));
    ui->widget->setPointSize(400000000+tid, 12);
  }
    
  ui->widget->thaw();
}
  
ProjectionWidget::ProjectionWidget(ServerInfo *info, SBEMDB *db,
                                   QWidget *parent):
  QWidget(parent), d(new ProjectionData(info, db)) {
  d->ui->setupUi(this);
}

ProjectionWidget::~ProjectionWidget() {
  delete d;
}

void ProjectionWidget::addTree(quint64 tid) {
  d->buildTree(tid);
}

void ProjectionWidget::addVisibleTrees() {
  d->ui->widget->freeze();
  QSqlQuery q = d->db->constQuery("select tid from trees where visible");
  while (q.next())
    addTree(q.value(0).toULongLong());
  d->ui->widget->thaw();
}

void ProjectionWidget::addSelectedTree() {
  addTree(d->db->selectedTree());
}
