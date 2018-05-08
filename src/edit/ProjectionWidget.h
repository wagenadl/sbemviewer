// Projection.h

#ifndef PROJECTIONWIDGET_H

#define PROJECTIONWIDGET_H

#include "SBEMDB.h"
#include <QWidget>

class ProjectionWidget: public QWidget {
public:
  ProjectionWidget(SBEMDB *db, QWidget *parent=0);
  virtual ~ProjectionWidget();
  void addTree(int tid);
  void addVisibleTrees();
  void addSelectedTree();
private:
  class ProjectionData *d;
};

#endif
