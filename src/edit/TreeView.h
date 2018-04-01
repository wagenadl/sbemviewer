// TreeView.h

#ifndef TREEVIEW_H

#define TREEVIEW_H

#include <QWidget>
#include "ui_TreeView.h"

class TreeView: public QWidget {
  Q_OBJECT;
public:
  TreeView(QWidget *parent=0);
  virtual ~TreeView();
  void setModel(class TreeModel *);
  void updateAfterChangingDB();
signals:
  void activeTreeChanged(quint64);
  void activeNodeChanged(quint64);
public slots:
  void setActiveTree(quint64);
  void actNewTree();
  void actDeleteTree();
  void actShowAll();
  void actHideAll();
public:
  class Ui_TreeView *ui;
private:
  class TreeModel *tm;
};

#endif
