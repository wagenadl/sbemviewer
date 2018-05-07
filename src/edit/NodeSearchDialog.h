// NodeSearchDialog.h

#ifndef NODESEARCHDIALOG_H

#define NODESEARCHDIALOG_H

#include <QDialog>
#include "SBEMDB.h"

class NodeSearchDialog: public QDialog {
public:
  NodeSearchDialog(SBEMDB *db, QWidget *parent=0);
  virtual ~NodeSearchDialog();
  QVector<SBEMDB::Node> nodes() const;
  void reset();
  static QVector<SBEMDB::Node> exec1(SBEMDB *db);
private:
  class NSD_Data *d;
  
};

#endif
