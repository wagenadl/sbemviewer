// TreeView.cpp

#include "TreeView.h"
#include "ui_TreeView.h"
#include "TreeModel.h"

TreeView::TreeView(QWidget *parent): QWidget(parent) {
  ui = new Ui_TreeView();
  ui->setupUi(this);
}

TreeView::~TreeView() {
  delete ui;
}

void TreeView::setModel(TreeModel *tm) {
  ui->table->setModel(tm);
}
