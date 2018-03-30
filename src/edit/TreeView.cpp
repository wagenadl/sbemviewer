// TreeView.cpp

#include "TreeView.h"
#include "ui_TreeView.h"
#include "TreeModel.h"
#include <QDebug>

TreeView::TreeView(QWidget *parent): QWidget(parent) {
  tm = 0;
  ui = new Ui_TreeView();
  ui->setupUi(this);
  ui->add->setDefaultAction(new QAction("+"));
  ui->del->setDefaultAction(new QAction("–"));
  connect(ui->add, &QToolButton::triggered,
          [this]() { if (!tm) return;
            tm->newTree();
            ui->table->selectRow(tm->rowCount()-1);
          });
  connect(ui->del, &QToolButton::triggered,
          [this]() { if (!tm) return;
            tm->deleteTree(tm->database()->selectedTree());
          });
}

TreeView::~TreeView() {
  delete ui;
}

void TreeView::updateAfterChangingDB() {
  quint64 tid = tm->database()->selectedTree();
  int row = tm->rowForTreeID(tid);
  if (row>=0) {
    ui->table->selectRow(row);
  }
  emit activeTreeChanged(tid);
}

void TreeView::setModel(TreeModel *tm1) {
  // the model may very well not have a db attached yet!
  tm = tm1;
  ui->table->setModel(tm);
  ui->table->resizeColumnToContents(0);
  ui->table->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->table->setSelectionMode(QAbstractItemView::SingleSelection);

  connect(ui->table->selectionModel(), &QItemSelectionModel::selectionChanged,
          [this](const QItemSelection &sel, const QItemSelection &) {
            qDebug() << "selection changed";
            if (!tm) return;
            if (sel.isEmpty()) {
              qDebug() << "empty";
              tm->database()->selectTree(0);
              activeTreeChanged(0);
            } else {
              QModelIndex idx = sel.indexes().first();
              qDebug() << "sel row" << idx.row();
              quint64 tid = tm->treeIDAt(idx.row());
              tm->database()->selectTree(tid);
              activeTreeChanged(tid);
            }
          });
  
}
