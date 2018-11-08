// Projection.h

#ifndef PROJECTIONWIDGET_H

#define PROJECTIONWIDGET_H

#include "SBEMDB.h"
#include <QMainWindow>

class ProjectionWidget: public QMainWindow {
  Q_OBJECT;
public:
  ProjectionWidget(class ServerInfo *info, SBEMDB *db, QWidget *parent=0);
  virtual ~ProjectionWidget();
  void addTree(quint64 tid);
  void addVisibleTrees();
  void addSelectedTree();
  void clear();
public slots:
  void updateShownTrees();
private:
  class ProjectionData *d;
};

#endif
