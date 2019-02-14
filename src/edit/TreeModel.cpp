// TreeModel.cpp

#include "TreeModel.h"
#include <QDateTime>

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
    return COLUMNS;
}

QVariant TreeModel::data(const QModelIndex &index,
                         int role) const {
  int r = index.row();
  int c = index.column();
  if (r<0 || c<0 || c>=COLUMNS)
    return QVariant();

  bool iseditdisp = role==Qt::EditRole || role==Qt::DisplayRole;
  bool ischeck = role==Qt::CheckStateRole;
  bool istip = role==Qt::ToolTipRole;

  if (istip) {
    if (c==Col_Visible)
      return "Visible";
    else
      return QVariant();
  } else if (ischeck) {
    if (c==Col_Visible) {
    QSqlQuery q = db->constQuery("select visible from trees"
                                 " order by tid asc"
                                 " limit 1 offset :a", r);
    if (q.next())
      return q.value(0).toBool() ?  Qt::Checked : Qt::Unchecked;
    else
      return QVariant();
    } else {
      return QVariant();
    }
  } else if (iseditdisp) {
    QSqlQuery q = db->constQuery("select tid,tname from trees"
                                 " order by tid asc"
                                 " limit 1 offset :a", r);
    if (!q.next())
      return QVariant();
    switch (c) {
    case Col_Id: return q.value(0);
    case Col_Name: return q.value(1);
    default: return QVariant();
    }
  } else {
    return QVariant();
  }
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value,
                        int role) {
  int r = index.row();
  int c = index.column();
  if (r<0)
    return false;
  if (!(role==Qt::EditRole || role==Qt::CheckStateRole))
    return false;

  // let's find our TID
  QSqlQuery q = db->constQuery("select tid from trees order by tid asc"
                               " limit 1 offset :a", r);
  if (!q.next())
    return false;
  QVariant tid = q.value(0);
  if (c==Col_Name && role==Qt::EditRole) {
    db->query("update trees set tname=:a where tid==:b",
              value, tid);
    emit dataChanged(index, index);
    return true;
  } else if (c==Col_Visible && role==Qt::CheckStateRole) {
    db->query("update trees set visible=:a where tid==:b",
              value.toInt() != Qt::Unchecked, tid);
    emit dataChanged(index, index);
    emit visibilityChanged();
    return true;
  } else {
    return false;
  }
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const {
  if (role!=Qt::DisplayRole)
    return QVariant();
  if (orientation==Qt::Horizontal) {
    switch (section) {
    case Col_Id: return QString("Id");
    case Col_Visible: return QString("V");
    case Col_Name: return QString("Name");
    default: return QVariant();
    }
  } else {
    return QVariant();
  }
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const {
  int c = index.column();
  Qt::ItemFlags f = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
  if (c==Col_Name)
    f |= Qt::ItemIsEditable;
  else if (c==Col_Visible)
    f |= Qt::ItemIsUserCheckable;
  return f;
}

quint64 TreeModel::newTree() {
  int R = rowCount();
  beginInsertRows(QModelIndex(), R, R);
  quint64 tid
    = db->query("insert into trees(visible,tname,cdate) values(1,:a,:b)",
		QVariant("-"), QVariant(QDateTime::currentDateTime()))
    .lastInsertId().toULongLong();
  endInsertRows();
  return tid;
}

QModelIndex TreeModel::indexForTreeID(quint64 tid) const {
  int row = rowForTreeID(tid);
  if (row<0)
    return QModelIndex();
  else
    return createIndex(row, 0);
}

int TreeModel::rowForTreeID(quint64 tid) const {
  int n = db->simpleQuery("select count(1) from trees where tid==:a",
                          qulonglong(tid)).toInt();
  if (!n)
    return -1;
  int n0 = db->simpleQuery("select count(1) from trees where tid<:a",
                           tid).toInt();
  return n0;
}

quint64 TreeModel::treeIDAt(int row) const {
  return data(index(row, 0)).toULongLong();
}

bool TreeModel::deleteTree(quint64 tid) {
  if (tid==0)
    return false;
  int n = db->simpleQuery("select count(1) from trees where tid==:a",
                          qulonglong(tid)).toInt();
  if (!n)
    return false;
  int n0 = db->simpleQuery("select count(1) from trees where tid<:a",
                           tid).toInt();
  db->begin();
  db->query("delete from nodes where tid==:a", tid);
  db->query("delete from trees where tid==:a", tid);
  beginRemoveRows(QModelIndex(), n0, n0);
  db->commit();
  endRemoveRows();
  return true;
}

void TreeModel::beginReset() {
  beginResetModel();
}

void TreeModel::concludeReset() {
  endResetModel();
}

void TreeModel::setAllVisible(bool v) {
  db->query("update trees set visible=:a", v);
  emit visibilityChanged();
  emit dataChanged(index(0, Col_Visible),
                  index(rowCount()-1, Col_Visible));
}
