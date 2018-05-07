// NSQuery.h

#ifndef NSQUERY_H

#define NSQUERY_H

#include "SBEMDB.h"

class NSQuery {
public:
  NSQuery(SBEMDB *db, class Ui_NodeSearchDialog *ui);
  ~NSQuery();
  int count() const;
  QVector<SBEMDB::Node> nodes() const;
private:
  void createTempTable();
  void dropTempTable();
  void makeTreeClauses();
  void makeNodeTypeClauses();
  void makeMemoClauses(); 
  QString nextArgRef();
private:
  SBEMDB *db;
  class Ui_NodeSearchDialog *ui;
private:
  QStringList joins;
  QStringList wheres;
  QList<QVariant> args;
private:
  bool onlySelectedTree;
  bool onlyVisibleTrees;
  bool useNodeType;
  bool useBranchPoints;
  bool useEndPoints;
  bool useExitPoint;
  bool useSoma;
  bool usePresyn;
  bool usePostsyn;
  bool usePrePartner;
  bool usePostPartner;
  bool useMemo;
  bool memoMustEqual;
  bool memoMustContain;
  QString memoText;
  QString memoAndText;
  QString memoNotText;
  QString tempTableName;
};

#endif
