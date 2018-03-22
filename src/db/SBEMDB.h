// SBEMDB.h

#ifndef SBEMDB_H

#define SBEMDB_H

#include "Database.h"

class SBEMDB: public Database {
public:
  enum NodeType {
    Invalid = 0,
    Soma = 1,
    ExitPoint = 2,
    TreeNode = 3,
    PresynTerm = 5,
    PostsynTerm = 6,
  };
  struct Tree {
    quint64 tid;
    QString tname;
    Tree(quint64 tid=0, QString tname=""): tid(tid), tname(tname) {}
  };
  struct Node {
    quint64 nid;
    quint64 tid;
    NodeType typ;
    int x, y, z;
    Node(quint64 nid=0, quint64 tid=0, NodeType typ=Invalid,
         int x=0, int y=0, int z=0):
      nid(nid), tid(tid), typ(typ), x(x), y(y), z(z) {}
  };
  struct Tag {
    quint64 tagid;
    int x, y, z;
    QString tag;
    Tag(quint64 tagid=0, int x=0, int y=0, int z=0, QString tag=""):
      tagid(tagid), x(x), y(y), z(z), tag(tag) {}
  };
  struct SimpleSynapse {
    quint64 sid;
    quint64 prenid;
    quint64 postnid;
    SimpleSynapse(quint64 sid=0, quint64 prenid=0, quint64 postnid=0):
      sid(sid), prenid(prenid), postnid(postnid) {}
  };
  struct NodeCon {
    quint64 ncid;
    quint64 nid1;
    quint64 nid2;
    NodeCon(quint64 ncid=0, quint64 nid1=0, quint64 nid2=0):
      ncid(ncid), nid1(nid1), nid2(nid2) {}
  };
  struct PolySynapse {
    quint64 sid;
    PolySynapse(quint64 sid=0): sid(sid) {}
  };
  struct SynCon {
    quint64 scid;
    quint64 sid;
    quint64 nid;
    SynCon(quint64 scid=0, quint64 sid=0, quint64 nid=0):
      scid(scid), sid(sid), nid(nid) {}
  };
  struct Synapse {
    quint64 sid;
    QMap<quint64, quint64> pre; // keys are scid or 0, values are nids
    QMap<quint64, quint64> post; // ditto
    Synapse(quint64 sid=0): sid(sid) {}    
  };
public:
  SBEMDB(QString id="");
  static void create(QString fn);
public:
  SimpleSynapse simpleSynapse(quint64 sid) const;
  PolySynapse polySynapse(quint64 sid) const;
  SynCon preSynCon(quint64 scid) const;
  SynCon postSynCon(quint64 scid) const;
  Tree tree(quint64 tid) const;
  Node node(quint64 nid) const;
  Tag tag(quint64 tagid) const;
  NodeCon nodeCon(quint64 ncid) const;
public:
  QVector<SimpleSynapse> simpleSynapses(QSqlQuery q) const;
  /* q must be of the form "select * from simplesynapses" followed by
     WHERE and/or JOIN clauses. */
  QVector<PolySynapse> polySynapses(QSqlQuery q) const;
  QVector<SynCon> preSynCons(QSqlQuery q) const;
  QVector<SynCon> postSynCons(QSqlQuery q) const;
  QVector<Tree> trees(QSqlQuery q) const;
  QVector<Node> nodes(QSqlQuery q) const;
  QVector<Tag> tags(QSqlQuery q) const;
  QVector<NodeCon> nodeCons(QSqlQuery q) const;
public:
  Synapse synapse(quint64 sid) const;

};

#endif
