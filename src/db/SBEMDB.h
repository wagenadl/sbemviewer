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
    SynContour = 7,
  };
  struct Tree {
    quint64 tid;
    QString tname;
    bool visible;
    Tree(quint64 tid=0, QString tname="", bool visible=true):
      tid(tid), tname(tname), visible(visible) {}
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
    quint64 nid;
    QString tag;
    Tag(quint64 tagid=0, quint64 nid=0, QString tag=""):
      tagid(tagid), nid(nid), tag(tag) {}
  };

  struct NodeCon {
    quint64 ncid;
    quint64 nid1;
    quint64 nid2;
    NodeCon(quint64 ncid=0, quint64 nid1=0, quint64 nid2=0):
      ncid(ncid), nid1(nid1), nid2(nid2) {}
  };
  struct Synapse {
    quint64 sid;
    Synapse(quint64 sid=0): sid(sid) {}
  };
  struct SynCon {
    quint64 scid;
    quint64 sid;
    quint64 nid;
    SynCon(quint64 scid=0, quint64 sid=0, quint64 nid=0):
      scid(scid), sid(sid), nid(nid) {}
  };
  struct FullSynapse {
    quint64 sid;
    QMap<quint64, quint64> pre; // keys are scid or 0, values are nids
    QMap<quint64, quint64> post; // ditto
    FullSynapse(quint64 sid=0): sid(sid) {}    
  };
public:
  SBEMDB(QString id="");
  void open(QString filename) override;
  static void create(QString fn);
public:
  Synapse synapse(quint64 sid) const;
  SynCon synCon(quint64 scid) const;
  Tree tree(quint64 tid) const;
  Node node(quint64 nid) const;
  Tag tag(quint64 tagid) const;
  NodeCon nodeCon(quint64 ncid) const;
  void selectTree(quint64 tid);
  quint64 selectedTree() const;
  void selectNode(quint64 nid);
  quint64 selectedNode() const;
  quint64 uid() const; // user ID
public:
  QVector<Synapse> synapses(QSqlQuery q) const;
  /* q must be of the form "select * from synapses" followed by
     WHERE and/or JOIN clauses. */
  QVector<SynCon> synCons(QSqlQuery q) const;
  QVector<Tree> trees(QSqlQuery q) const;
  QVector<Node> nodes(QSqlQuery q) const;
  QVector<Tag> tags(QSqlQuery q) const;
  QVector<NodeCon> nodeCons(QSqlQuery q) const;
  QMap<quint64, QString> treeNames(QSqlQuery q) const;
  // q must be from "select tid, tname from trees" or equivalent
public:
  FullSynapse synapseDetails(quint64 sid) const;
  Node nodeAt(class Point const &p, int xytol, int ztol, quint64 tid) const;
  // Prefer to return from tid, but will accept other tree.
  // If all else fails, will select from tid with ztol ignored.
  Node somaAt(class Point const &p, int xytol, int ztol, quint64 tid) const;
  static QString nodeTypeName(NodeType);
public:
  quint64 createNodeCon(quint64 nid1, quint64 nid2); // returns id
  void createNodeConPair(quint64 nid1, quint64 nid2); // bidrectional
  quint64 createSynCon(quint64 sid, quint64 nid); // returns id
  quint64 createNode(quint64 tid, NodeType typ, Point const &p); // returns id
  quint64 createTag(quint64 nid, QString tag); // returns id
  quint64 createTree(QString tname="-", bool vis=true);
  quint64 createSynapse();
  void updateTag(quint64 tagid, QString tag);
private:
  quint64 uid_;
};

#endif
