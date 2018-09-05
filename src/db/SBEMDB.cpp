// SBEMDB.cpp

#include "SBEMDB.h"
#include <QFile>
#include "PDebug.h"
#include "SqlFile.h"
#include "Point.h"

SBEMDB::SBEMDB(QString id): Database(id) {
}

void SBEMDB::create(QString fn) {
  QFile f(fn);
  if (f.exists())
    CRASH("Could not create now SBEMDB: File exists: " + fn);

  Database db;
  db.open(fn);
  SqlFile sql(":/create.sql");
  {
    Transaction t(&db);
    for (auto c: sql)
      db.query(c);
    t.commit();
  }
  db.close();
}

SBEMDB::Synapse SBEMDB::synapse(quint64 sid) const {
  auto lst
    = synapses(constQuery("select * from synapses where sid==:a",
                                sid));
  return lst.isEmpty() ? Synapse() : lst.first();
}

SBEMDB::SynCon SBEMDB::synCon(quint64 scid) const {
  auto lst
    = synCons(constQuery("select * from syncon where scid==:a",
                            scid));
  return lst.isEmpty() ? SynCon() : lst.first();
}

SBEMDB::Tree SBEMDB::tree(quint64 tid) const {
  auto lst
    = trees(constQuery("select * from tree where tid==:a",
                       tid));
  return lst.isEmpty() ? Tree() : lst.first();  
}

SBEMDB::Node SBEMDB::node(quint64 nid) const {
  auto lst
    = nodes(constQuery("select * from nodes where nid==:a",
                       nid));
  return lst.isEmpty() ? Node() : lst.first();
}

SBEMDB::Tag SBEMDB::tag(quint64 tagid) const {
  auto lst
    = tags(constQuery("select * from tags where tagid==:a",
                        tagid));
  return lst.isEmpty() ? Tag() : lst.first();
}

SBEMDB::NodeCon SBEMDB::nodeCon(quint64 ncid) const {
  auto lst
    = nodeCons(constQuery("select * from nodecons where ncid==:a",
                          ncid));
  return lst.isEmpty() ? NodeCon() : lst.first();
}

QVector<SBEMDB::Synapse> SBEMDB::synapses(QSqlQuery q) const {
  QVector<Synapse> lst;
  while (q.next()) 
    lst << Synapse{q.value(0).toULongLong()};
  return lst;
}

QVector<SBEMDB::SynCon> SBEMDB::synCons(QSqlQuery q) const {
  QVector<SynCon> lst;
  while (q.next()) {
    lst << SynCon{q.value(0).toULongLong(),
        q.value(1).toULongLong(), q.value(2).toULongLong()};
  }
  return lst;
}

QVector<SBEMDB::Tree> SBEMDB::trees(QSqlQuery q) const {
  QVector<Tree> lst;
  while (q.next()) {
    lst << Tree{q.value(0).toULongLong(),
        q.value(1).toString()};
  }
  return lst;
}

QMap<quint64, QString> SBEMDB::treeNames(QSqlQuery q) const {
  QMap<quint64, QString> map;
  while (q.next())
    map[q.value(0).toULongLong()] = q.value(1).toString();
  return map;
}

QVector<SBEMDB::Node> SBEMDB::nodes(QSqlQuery q) const {
  QVector<Node> lst;
  while (q.next()) {
    lst << Node{q.value(0).toULongLong(),
        q.value(1).toULongLong(), NodeType(q.value(2).toInt()),
        q.value(3).toInt(), q.value(4).toInt(), q.value(5).toInt()};
  }
  return lst;
}

QVector<SBEMDB::Tag> SBEMDB::tags(QSqlQuery q) const {
  QVector<Tag> lst;
  while (q.next()) {
    lst << Tag{q.value(0).toULongLong(),
        q.value(1).toULongLong(),
        q.value(2).toString()};
  }
  return lst;
}

QVector<SBEMDB::NodeCon> SBEMDB::nodeCons(QSqlQuery q) const {
  QVector<NodeCon> lst;
  while (q.next()) {
    lst << NodeCon{q.value(0).toULongLong(),
        q.value(1).toULongLong(), q.value(2).toULongLong()};
  }
  return lst;
}

SBEMDB::FullSynapse SBEMDB::synapseDetails(quint64 sid) const {
  SBEMDB::FullSynapse res;
  QSqlQuery q = constQuery("select * from synapses where sid=:a", sid);
  if (!q.next())
    return res;

  res.sid = q.value(0).toULongLong();

  q = constQuery("select scid, nid, typ from syncons"
                 " natural join nodes where sid==:a", sid);
  while (q.next()) {
    quint64 scid = q.value(0).toULongLong();
    quint64 nid = q.value(1).toULongLong();
    NodeType typ = (NodeType)(q.value(2).toInt());
    if (typ==PresynTerm)
      res.pre[scid] = nid;
    else if (typ==PostsynTerm)
      res.post[scid] = nid;
  }
  return res;
}

void SBEMDB::selectNode(quint64 nid) {
  qDebug() << "selectnode" << nid;
  if (nid)
    query("update selectednode set nid = :a", nid);
  else
    query("update selectednode set nid = null");
}

quint64 SBEMDB::selectedNode() const {
  return simpleQuery("select nid from selectednode").toULongLong();
}

void SBEMDB::selectTree(quint64 tid) {
  if (tid)
    query("update selectedtree set tid = :a", tid);
  else
    query("update selectedtree set tid = null");
}

quint64 SBEMDB::selectedTree() const {
  return simpleQuery("select tid from selectedtree").toULongLong();
}

SBEMDB::Node SBEMDB::nodeAt(Point const &p,
                            int xytol, int ztol, quint64 tid) const {
  // Prefer to return from tid, but will accept other tree.
  // If all else fails, will select from tid with ztol ignored.
  QVector<Node> nn = nodes(constQuery("select * from nodes where tid==:a"
                                      " and z>=:b and z<=:c"
                                      " and x>=:d and x<=:e"
                                      " and y>=:f and y<=:g",
                                 tid,
                                 p.z-ztol, p.z+ztol,
                                 p.x-xytol, p.x+xytol,
                                 p.y-xytol, p.y+xytol));
  Node nbest;
  double dbest = 0;
  auto sq = [](int x) { return x*x; };
  for (Node const &n: nn) {
    double d = sq(n.x - p.x) + sq(n.y - p.y) + sq(n.z - p.z);
    if (nbest.nid==0 || d<dbest) {
      nbest = n;
      dbest = d;
    }
  }
  if (nbest.nid)
    return nbest;

  nn = nodes(constQuery("select * from nodes where"
                        " z>=:a and z<=:b"
                        " and x>=:c and x<=:d"
                        " and y>=:e and y<=:f",
                        p.z-ztol, p.z+ztol,
                        p.x-xytol, p.x+xytol,
                        p.y-xytol, p.y+xytol));
  for (Node const &n: nn) {
    double d = sq(n.x - p.x) + sq(n.y - p.y) + sq(n.z - p.z);
    if (nbest.nid==0 || d<dbest) {
      nbest = n;
      dbest = d;
    }
  }
  if (nbest.nid)
    return nbest;

  nn = nodes(constQuery("select * from nodes where tid==:a"
                        " and x>=:b and x<=:c"
                        " and y>=:d and y<=:e",
                        tid,
                        p.x-xytol, p.x+xytol,
                        p.y-xytol, p.y+xytol));
  for (Node const &n: nn) {
    double d = sq(n.x - p.x) + sq(n.y - p.y) + sq(n.z - p.z)/1000000.0;
    if (nbest.nid==0 || d<dbest) {
      nbest = n;
      dbest = d;
    }
  }

  return nbest;
}

QString SBEMDB::nodeTypeName(NodeType nt) {
  switch (nt) {
  case Invalid: return "Invalid";
  case Soma: return "Soma";
  case ExitPoint: return "Exit point";
  case TreeNode: return "Tree node";
  case PresynTerm: return "Presyn.";
  case PostsynTerm: return "Postsyn.";
  default: return "Unknown";
  }
}

