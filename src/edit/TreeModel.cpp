// TreeModel.cpp

#include "TreeModel.h"

TreeModel::TreeModel(SBEMDB *db, QObject *parent):
  QAbstractTableModel(parent), db(db) {
}

TreeModel::~TreeModel() {
}

int TreeModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;
  else if (db->isOpen())
    return db->simpleQuery("select count(1) from trees").toInt();
  else
    return 0;
}

int TreeModel::columnCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;
  else
    return 2;
}

QVariant TreeModel::data(const QModelIndex &index,
                         int role) const {
  int r = index.row();
  int c = index.column();
  if (r<0 || c<0 || c>=2)
    return QVariant();
  if (!(role==Qt::DisplayRole || (r==1 && role==Qt::EditRole)))
    return QVariant();
  QSqlQuery q = db->constQuery("select * from trees order by tid asc"
                               " limit 1 offset :a", r);
  if (!q.next())
    return QVariant();
  return q.value(c); // returns ID for c=0, name for c=1
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value,
                        int role) {
  int r = index.row();
  int c = index.column();
  if (r<0 || c!=1)
    return false;
  if (role!=Qt::EditRole)
    return false;

  // let's find our TID
  QSqlQuery q = db->constQuery("select * from trees order by tid asc"
                               " limit 1 offset :a", r);
  if (!q.next())
    return false;
  db->query("update trees set tname=:a where tid==:b", value, q.value(0));
  emit dataChanged(index, index);
  return true;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const {
  if (role!=Qt::DisplayRole)
    return QVariant();
  if (orientation==Qt::Horizontal) {
    switch (section) {
    case 0: return QString("Id");
    case 1: return QString("Name");
    default: return QVariant();
    }
  } else {
    return QVariant();
  }
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const {
  int c = index.column();
  Qt::ItemFlags f = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
  if (c==1)
    f |= Qt::ItemIsEditable;
  return f;
}

quint64 TreeModel::newTree() {
  int R = rowCount();
  beginInsertRows(QModelIndex(), R, R);
  quint64 tid = db->query("insert into versions(tname) values(:a)", "")
    .lastInsertId().toULongLong();
  endInsertRows();
  return tid;
}

bool TreeModel::deleteTree(quint64 tid) {
  int n = db->simpleQuery("select count(*) from trees where tid==:a",
                          qulonglong(tid)).toInt();
  if (!n)
    return false;
  int n0 = db->simpleQuery("select count(*) from trees where tid<:a",
                           tid).toInt();
  beginRemoveRows(QModelIndex(), n0, n0);
  db->query("delete from versions where tid==:a", tid);
  endRemoveRows();
  return true;
}

void TreeModel::beginReset() {
  beginResetModel();
}

void TreeModel::concludeReset() {
  endResetModel();
}
