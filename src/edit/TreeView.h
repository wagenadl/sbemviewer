// TreeView.h

#ifndef TREEVIEW_H

#define TREEVIEW_H

#include <QWidget>

class TreeView: public QWidget {
public:
  TreeView(QWidget *parent=0);
  virtual ~TreeView();
  void setModel(class TreeModel *);
private:
  class Ui_TreeView *ui;
};

#endif
