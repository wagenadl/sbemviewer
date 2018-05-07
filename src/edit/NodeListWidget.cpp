// NodeListWidget.cpp

#include "NodeListWidget.h"
#include <QTableWidget>
#include <QHBoxLayout>
#include "ServerInfo.h"
#include <QDebug>

class NLW_Data {
public:
  NLW_Data(ServerInfo *info, QVector<SBEMDB::Node> nodes):
    info(info), nodes(nodes) {
    dx = info->contains("dx") ? info->real("dx") : 0.0055;
    dy = info->contains("dy") ? info->real("dy") : 0.0055;
    dz = info->contains("dz") ? info->real("dz") : 0.050;
  }
public:
  ServerInfo *info;
  QVector<SBEMDB::Node> nodes;
  QTableWidget *tw;
  double dx, dy, dz;
};

NodeListWidget::NodeListWidget(ServerInfo *info,
                               QVector<SBEMDB::Node> nodes,
                               QWidget *parent):
  QWidget(parent), d(new NLW_Data(info, nodes)) {
  // setDeleteOnClose();
  QHBoxLayout *lay = new QHBoxLayout(this);
  lay->setContentsMargins(6, 6, 6, 6);
  d->tw = new QTableWidget(this);
  lay->addWidget(d->tw);
  setLayout(lay);

  d->tw->setColumnCount(4);
  d->tw->setRowCount(nodes.size());
  d->tw->setHorizontalHeaderLabels(QStringList() << "Type" << "X" << "Y" << "Z");

  for (int k=0; k<nodes.size(); k++) {
    SBEMDB::Node const &n(nodes[k]);
    QString typ = SBEMDB::nodeTypeName(n.typ);
    d->tw->setItem(k, 0, new QTableWidgetItem(typ));
    d->tw->setItem(k, 1, new QTableWidgetItem(QString("%1").arg(n.x*d->dx)));
    d->tw->setItem(k, 2, new QTableWidgetItem(QString("%1").arg(n.y*d->dy)));
    d->tw->setItem(k, 3, new QTableWidgetItem(QString("%1").arg(n.z*d->dz)));
  }
  setAttribute(Qt::WA_DeleteOnClose);
  d->tw->setSelectionMode(QAbstractItemView::SingleSelection);
  d->tw->setSelectionBehavior(QAbstractItemView::SelectRows);
  connect(d->tw, &QTableWidget::itemSelectionChanged,
          [this]() { auto *m = d->tw->selectionModel();
            qDebug() << "selection model " << m;
            if (m) {
              auto idx = m->currentIndex();
              qDebug() << "idx" << idx.isValid() << idx.row();
              if (idx.isValid()) 
                emit selected(idx.row());
            }});
}

NodeListWidget::~NodeListWidget() {
  delete d;
}

