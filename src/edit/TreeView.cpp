// TreeView.cpp

#include "TreeView.h"
#include "ui_TreeView.h"
#include "TreeModel.h"
#include <QDebug>
#include <QMessageBox>
#include <QSortFilterProxyModel>

TreeView::TreeView(QWidget *parent): QWidget(parent) {
  tm = 0;
  ui = new Ui_TreeView();
  ui->setupUi(this);
  sortproxy = new QSortFilterProxyModel(ui->table);
}

TreeView::~TreeView() {
  delete ui;
}

void TreeView::actNewTree() {
  if (!tm)
    return;
  tm->newTree();
  ui->table->selectRow(tm->rowCount()-1);
}

void TreeView::actDeleteTree() {
  if (!tm)
    return;
  quint64 tid = tm->database()->selectedTree();
  if (!tid)
    return;
  QString name = tm->database()->simpleQuery("select tname from trees"
                                             " where tid==:a", tid)
    .toString();
  int n = tm->database()->simpleQuery("select count(1) from nodes"
                                      " where tid==:a", tid).toInt();
  if (n>=10) {
    auto ans = QMessageBox::question(0, "Delete tree?",
                                     "Deleting a tree cannot be undone."
                                     " The tree named\n\n"
                                     "     “" + name + "”\n\n"
                                     "contains " + QString::number(n) +
                                     " nodes which will be"
                                     " irretrievably lost."
                                     " Are you sure you want to proceed?");
    if (ans != QMessageBox::Yes)
      return;
  }
  tm->deleteTree(tid);
}

void TreeView::actShowAll() {
  tm->setAllVisible(true);

}

void TreeView::actHideAll() {
  tm->setAllVisible(false);
}

void TreeView::updateAfterChangingDB() {
  quint64 tid = tm->database()->selectedTree();
  quint64 nid = tm->database()->selectedNode();
  QModelIndex idx = tm->indexForTreeID(tid);
  if (idx.isValid()) {
    QModelIndex sidx = sortproxy->mapFromSource(idx);
    qDebug() << "click" << idx.row() << sidx.row();
    int row = sidx.row();
    ui->table->selectRow(row);
  }
  emit activeTreeChanged(tid);
  emit activeNodeChanged(nid);
}

void TreeView::setModel(TreeModel *tm1) {
  // the model may very well not have a db attached yet!
  tm = tm1;
  sortproxy->setSourceModel(tm);
  ui->table->setModel(sortproxy);
  ui->table->setSortingEnabled(true);
  ui->table->sortByColumn(TreeModel::Col_Id, Qt::AscendingOrder);
  ui->table->resizeColumnToContents(TreeModel::Col_Id);
  ui->table->resizeColumnToContents(TreeModel::Col_Visible);
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
              QModelIndex sidx = sel.indexes().first();
	      QModelIndex idx = sortproxy->mapToSource(sidx);
              qDebug() << "sel row" << idx.row();
              quint64 tid = tm->treeIDAt(idx.row());
              tm->database()->selectTree(tid);
              activeTreeChanged(tid);
            }
          });
  
}

void TreeView::setActiveTree(quint64 tid) {
  if (!tm)
    return;
  int row = tm->rowForTreeID(tid);
  tm->database()->selectTree(tid);
  ui->table->selectRow(row);
}
