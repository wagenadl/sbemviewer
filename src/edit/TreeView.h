// TreeView.h

#ifndef TREEVIEW_H

#define TREEVIEW_H

#include <QWidget>

class TreeView: public QWidget {
  Q_OBJECT;
public:
  TreeView(QWidget *parent=0);
  virtual ~TreeView();
  void setModel(class TreeModel *);
  void updateAfterChangingDB();
signals:
  void activeTreeChanged(quint64);
public slots:
  void setActiveTree(quint64);
private:
  class Ui_TreeView *ui;
  class TreeModel *tm;
};

#endif
