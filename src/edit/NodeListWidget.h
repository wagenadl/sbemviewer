// NodeListWidget.h

#ifndef NODELISTWIDGET_H

#define NODELISTWIDGET_H

#include <QWidget>
#include "SBEMDB.h"

class NodeListWidget: public QWidget {
  Q_OBJECT;
public:
  NodeListWidget(class ServerInfo *info,
                 QVector<SBEMDB::Node> nodes, QWidget *parent=0);
  virtual ~NodeListWidget();
signals:
  void selected(int k);
private:
  class NLW_Data *d;
};

#endif
