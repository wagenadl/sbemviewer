// Projection.h

#ifndef PROJECTIONWIDGET_H

#define PROJECTIONWIDGET_H

#include "SBEMDB.h"
#include <QMainWindow>

class ProjectionWidget: public QMainWindow {
  Q_OBJECT;
public:
  ProjectionWidget(SBEMDB *db, QWidget *parent=0);
  virtual ~ProjectionWidget();
  void addTree(quint64 tid);
  void addVisibleTrees();
  void addSelectedTree();
  void clear();
signals:
  void doubleClickOnTree(int tid, double x, double y, double z);
public slots:
  void updateShownTrees();
private:
  class ProjectionData *d;
};

#endif
