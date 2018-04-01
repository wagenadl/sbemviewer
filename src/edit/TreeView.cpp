// TreeView.cpp

#include "TreeView.h"
#include "ui_TreeView.h"
#include "TreeModel.h"
#include <QDebug>
#include <QMessageBox>

TreeView::TreeView(QWidget *parent): QWidget(parent) {
  tm = 0;
  ui = new Ui_TreeView();
  ui->setupUi(this);
}

TreeView::~TreeView() {
  delete ui;
}

void TreeView::actNewTree() {
  if (!tm) return;
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
  auto ans = QMessageBox::question(0, "Delete tree?",
                                   "Deleting a tree cannot be undone."
                                   " All tracing work on the tree named\n\n"
                                   "     “" + name + "”\n\n"
                                   "will be"
                                   " irretrievably lost."
                                   " Are you sure you want to proceed?");
  if (ans == QMessageBox::Yes)
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
  int row = tm->rowForTreeID(tid);
  if (row>=0) {
    ui->table->selectRow(row);
  }
  emit activeTreeChanged(tid);
  emit activeNodeChanged(nid);
}

void TreeView::setModel(TreeModel *tm1) {
  // the model may very well not have a db attached yet!
  tm = tm1;
  ui->table->setModel(tm);
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
              QModelIndex idx = sel.indexes().first();
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
