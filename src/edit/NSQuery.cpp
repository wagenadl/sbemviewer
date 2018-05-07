// NSQuery.cpp

#include "NSQuery.h"
#include "ui_NodeSearchDialog.h"
#include <stdlib.h>
#include <QDebug>

NSQuery::NSQuery(SBEMDB *db, Ui_NodeSearchDialog *ui):
  db(db), ui(ui) {
  onlySelectedTree = ui->treeSelected->isChecked();
  onlyVisibleTrees = ui->treeVisible->isChecked();
  useNodeType = ui->nodeType->isChecked();
  useBranchPoints = ui->typeBranchpoint->isChecked();
  useEndPoints = ui->typeEndpoint->isChecked();
  useExitPoint = ui->typeExit->isChecked();  
  useSoma = ui->typeSoma->isChecked();
  usePresyn = ui->typePresyn->isChecked();
  usePostsyn = ui->typePostsyn->isChecked();
  usePrePartner = ui->typePrePartner->isChecked();
  usePostPartner = ui->typePostPartner->isChecked();
  useMemo = ui->memo->isChecked();
  memoMustEqual = ui->memoEquals->isChecked();
  memoText = ui->memoText->text();
  memoMustContain = !memoMustEqual && !memoText.isEmpty();
  memoAndText = ui->memoAndAlso->isChecked()
    ? ui->memoAndText->text() : QString();
  memoNotText = ui->memoButNot->isChecked()
    ? ui->memoNotText->text() : QString();

  makeTreeClauses();
  makeNodeTypeClauses();
  makeMemoClauses();
}

NSQuery::~NSQuery() {
  dropTempTable();
}

int NSQuery::count() const {
  QString q = "select count(1) from nodes";
  if (!joins.isEmpty())
    q += " " + joins.join(" ");
  if (!wheres.isEmpty())
    q += " where " + wheres.join(" and ");
  qDebug() << "query" << q;
  qDebug() << "args" << args;
  switch (args.size()) {
  case 0:
    return db->simpleQuery(q).toInt();
  case 1:
    return db->simpleQuery(q, args[0]).toInt();
  case 2:
    return db->simpleQuery(q, args[0], args[1]).toInt();
  case 3:
    return db->simpleQuery(q, args[0], args[1], args[2]).toInt();
  default:
    qDebug() << "NSQuery: Too many arguments";
    return 0;
  }
}

QVector<SBEMDB::Node> NSQuery::nodes() const {
  QString q = "select nid, tid, typ, x, y, z from nodes";
  if (!joins.isEmpty())
    q += " " + joins.join(" ");
  if (!wheres.isEmpty())
    q += " where " + wheres.join(" and ");
  qDebug() << "query" << q;
  switch (args.size()) {
  case 0:
    return db->nodes(db->constQuery(q));
  case 1:
    return db->nodes(db->constQuery(q, args[0]));
  case 2:
    return db->nodes(db->constQuery(q, args[0], args[1]));
  case 3:
    return db->nodes(db->constQuery(q, args[0], args[1], args[2]));
  default:
    qDebug() << "NSQuery: Too many arguments";
    return QVector<SBEMDB::Node>();
  }
}

void NSQuery::createTempTable() {
  tempTableName = QString("edgecount%08x").arg(rand());
  QString q = "create temp table " + tempTableName
    + " as select nid, count(1) as cnt"
    + " from nodecons inner join nodes on nid1==nid";
  if (onlySelectedTree) {
    q += QString(" where tid==%1").arg(db->selectedTree());
  } else if (onlyVisibleTrees) {
    q += " natural join trees where visible";
  }
  q += " group by nid";
  qDebug() << "query" << q;
  db->query(q);
}

void NSQuery::dropTempTable() {
  if (!tempTableName.isEmpty()) {
    QString q = "drop table " + tempTableName;
    qDebug() << "query" << q;
    db->query(q);
  }
}

void NSQuery::makeTreeClauses() {
  if (onlySelectedTree) {
    wheres << QString("tid==%1").arg(db->selectedTree());
  } else if (onlyVisibleTrees) {
    joins << "natural join trees";
    wheres << QString("visible");
  }
}

void NSQuery::makeNodeTypeClauses() {
  if (!useNodeType)
    return;
  
  QStringList clauses; // these will be OR'ed together
  if (useBranchPoints || useEndPoints) {
    createTempTable();
    joins << "natural join " + tempTableName;
    QString clau = "(typ==3 and ";
    if (useBranchPoints && useEndPoints) {
      clau += "cnt!=2";
    } else if (useBranchPoints) {
      clau += "cnt>2";
    } else { // end points
      clau += "cnt<2";
    }
    clau += ")";
    clauses << clau;
  }
  if (useExitPoint)
    clauses << "typ==2";
  if (useSoma)
    clauses << "typ==1";
  if (usePresyn)
    clauses << "typ==5";
  if (usePostsyn)
    clauses << "typ==6";

  if (clauses.isEmpty())
    wheres << "0";
  else
    wheres << "(" + clauses.join(" or ") + ")";
}

QString NSQuery::nextArgRef() {
  return QString(":%1").arg(QChar('a' + args.size()));
}

void NSQuery::makeMemoClauses() {
  if (!useMemo)
    return;
  
  joins << "natural join tags";

  if (memoMustEqual) {
    wheres << "tag==" + nextArgRef();
    args << memoText;
  } else if (memoMustContain) {
    wheres << "tag like " + nextArgRef();
    args << "%" + memoText + "%";
    
    if (!memoAndText.isEmpty()) {
      wheres << "tag like " + nextArgRef();
      args << "%" + memoAndText + "%";
    }
    if (!memoNotText.isEmpty()) {
      wheres << "tag not like " + nextArgRef();
      args << "%" + memoNotText + "%";
    }
  }
}

  
