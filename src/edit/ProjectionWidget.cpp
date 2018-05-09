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
  ui->widget->setTree(tid, ll);

  if (isSelected)
    ui->widget->setColors(tid, QColor(255, 0, 255), QColor(40, 0, 40));
  else
    ui->widget->setColors(tid, QColor(0, 255, 0), QColor(0, 40, 0));
}
  
ProjectionWidget::ProjectionWidget(ServerInfo *info, SBEMDB *db,
                                   QWidget *parent):
  QWidget(parent), d(new ProjectionData(info, db)) {
  d->ui->setupUi(this);
  d->ui->widget->setXRotation(d->ui->verticalSlider->value());
  d->ui->widget->setYRotation(d->ui->horizontalSlider->value());
}

ProjectionWidget::~ProjectionWidget() {
  delete d;
}

void ProjectionWidget::addTree(quint64 tid) {
  d->buildTree(tid);
}

void ProjectionWidget::addVisibleTrees() {
  QSqlQuery q = d->db->constQuery("select tid from trees where visible");
  while (q.next())
    addTree(q.value(0).toULongLong());
}

void ProjectionWidget::addSelectedTree() {
  addTree(d->db->selectedTree());
}

