// NodeListWidget.cpp

#include "NodeListWidget.h"
#include <QTableWidget>
#include <QHBoxLayout>
#include "ServerInfo.h"
#include "SBEMDB.h"
#include <QHeaderView>
#include <QDebug>

class NLW_Data {
public:
  NLW_Data(ServerInfo *info, SBEMDB *db, QVector<SBEMDB::Node> nodes):
    info(info), db(db), nodes(nodes) {
    dx = info->contains("dx") ? info->real("dx") : 0.0055;
    dy = info->contains("dy") ? info->real("dy") : 0.0055;
    dz = info->contains("dz") ? info->real("dz") : 0.050;
  }
public:
  ServerInfo *info;
  SBEMDB *db;
  QVector<SBEMDB::Node> nodes;
  QTableWidget *tw;
  double dx, dy, dz;
};

NodeListWidget::NodeListWidget(ServerInfo *info, SBEMDB *db,
                               QVector<SBEMDB::Node> nodes,
                               QWidget *parent):
  QWidget(parent), d(new NLW_Data(info, db, nodes)) {
  QHBoxLayout *lay = new QHBoxLayout(this);
  lay->setContentsMargins(6, 6, 6, 6);
  d->tw = new QTableWidget(this);
  lay->addWidget(d->tw);
  setLayout(lay);
  setWindowTitle("Search Results");

  d->tw->setColumnCount(7);
  d->tw->setColumnHidden(6, true);
  d->tw->setRowCount(nodes.size());
  d->tw->setHorizontalHeaderLabels(QStringList() << "Type" << "Tree" << "X (μm)" << "Y (μm)" << "Z (μm)" << "Memo" << "#");
  d->tw->verticalHeader()->hide();

  QMap<int, QString> treeNames;
  
  for (int k=0; k<nodes.size(); k++) {
    SBEMDB::Node const &n(nodes[k]);
    QString typ = SBEMDB::nodeTypeName(n.typ);
    if (n.typ==SBEMDB::TreeNode) {
      int c = d->db->simpleQuery("select count(1) from nodecons where nid1==:a",
                                 n.nid).toInt();
      switch (c) {
      case 0: typ = "Dangling"; break;
      case 1: typ = "Endpoint"; break;
      case 2: typ = "Node"; break;
      default: typ = QString("Branch pt. (%1)").arg(c); break;
      }
    }

    if (!treeNames.contains(n.tid)) {
      QString tname = QString("(%1)").arg(n.tid);
      QSqlQuery q = db->constQuery("select tname from trees where tid==:a",
                                   n.tid);
      if (q.next()) {
        QString t = q.value(0).toString();
        if (t != "-")
          tname = t;
      }
      treeNames[n.tid] = tname;
    }
    
    d->tw->setItem(k, 0, new QTableWidgetItem(typ));
    d->tw->setItem(k, 1, new QTableWidgetItem(treeNames[n.tid]));
    d->tw->setItem(k, 2, new QTableWidgetItem(QString("%1")
                                              .arg(n.x*d->dx, 0, 'f', 2)));
    d->tw->setItem(k, 3, new QTableWidgetItem(QString("%1")
                                              .arg(n.y*d->dy, 0, 'f', 2)));
    d->tw->setItem(k, 4, new QTableWidgetItem(QString("%1")
                                              .arg(n.z*d->dz, 0, 'f', 2)));

    QSqlQuery q = db->constQuery("select tag from tags where nid==:a",
                                 n.nid);
    if (q.next())
      d->tw->setItem(k, 5, new QTableWidgetItem(q.value(0).toString()));
    d->tw->setItem(k, 6, new QTableWidgetItem(QString::number(k)));
      
  }

  d->tw->setSortingEnabled(true);
  d->tw->setSelectionMode(QAbstractItemView::SingleSelection);
  d->tw->setSelectionBehavior(QAbstractItemView::SelectRows);
  d->tw->resizeColumnsToContents();
  connect(d->tw, &QTableWidget::itemSelectionChanged,
          [this]() { auto *m = d->tw->selectionModel();
            qDebug() << "selection model " << m;
            if (m) {
              auto idx = m->currentIndex();
              qDebug() << "idx" << idx.isValid() << idx.row();
              if (idx.isValid()) {
                int row = idx.row();
                int k = d->tw->item(row, 6)->text().toInt();
                emit selected(k);
              }
            }});
}

NodeListWidget::~NodeListWidget() {
  delete d;
}

