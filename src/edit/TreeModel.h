// TreeModel.h

#ifndef TREEMODEL_H

#define TREEMODEL_H

#include "SBEMDB.h"
#include <QAbstractTableModel>

class TreeModel: public QAbstractTableModel {
  Q_OBJECT;
public:
  enum Columns {
    Col_Id = 0,
    Col_Visible,
    Col_Name,
    COLUMNS
  };
public:
  TreeModel(SBEMDB *db, QObject *parent=0);
  ~TreeModel();
  int rowCount(const QModelIndex &parent=QModelIndex()) const override;
  int columnCount(const QModelIndex &parent=QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role=Qt::DisplayRole) const override;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role=Qt::EditRole) override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role=Qt::DisplayRole) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  quint64 newTree();
  bool deleteTree(quint64); // true if successful
  void beginReset(); // call before changing the db
  void concludeReset(); // call after changing the db
  SBEMDB *database() const { return db; }
  int rowForTreeID(quint64 tid) const; // -1 if invalid
  quint64 treeIDAt(int row) const; // 0 if invalid
signals:
  void visibilityChanged();
private:
  SBEMDB *db;
};

#endif
