// SBEMDB.cpp

#include "SBEMDB.h"
#include <QFile>
#include "PDebug.h"
#include "SqlFile.h"
#include "Point.h"
#include <QDateTime>
#include <QDir>
#include <QUuid>

static QVariant now() {
  return QVariant(QDateTime::currentDateTime());
}

SBEMDB::SBEMDB(QString id): Database(id) {
  uid_ = 0;
}

bool SBEMDB::canDoRegExps() const {
  QSqlQuery q(db);
  q.prepare("select count(*) from info where id regexp \"sbem\"");
  if (q.exec()) 
    return true;
  else
    return false;
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

void SBEMDB::open(QString fn) {
  Database::open(fn);
  if (!isOpen())
    return;
  QString vsn = simpleQuery("select version from info where id==\"sbemviewer\"")
    .toString();
  if (vsn < "0.2") {
    pDebug() << "cdate does not exist";
    Transaction t(this);
    query("alter table trees add column cdate");
    query("alter table nodes add column cdate");
    query("alter table tags add column cdate");
    query("alter table nodecons add column cdate");
    query("alter table synapses add column cdate");
    query("alter table syncons add column cdate");
    query("update info set version = \"0.2\" where id==\"sbemviewer\"");
    t.commit();
  }
  if (vsn < "0.3") {
    pDebug() << "uid does not exist";
    Transaction t(this);
    query("alter table trees add column uid integer");
    query("alter table nodes add column uid integer");
    query("alter table tags add column uid integer");
    query("alter table nodecons add column uid integer");
    query("alter table synapses add column uid integer");
    query("alter table syncons add column uid integer");
    query("create table users ( uid integer, home text )");
    query("update info set version = \"0.3\" where id==\"sbemviewer\"");
    t.commit();
  }
  QString user = QDir::homePath();
  QSqlQuery q = constQuery("select uid from users where home==:a", user);
  if (q.next()) {
    uid_ = q.value(0).toULongLong();
  } else {
    QString uuid = QUuid::createUuid().toString();
    qDebug() << "new uuid" << uuid;
    uuid.replace("-", "");
    uuid.replace("{", "");
    uuid.replace("}", "");
    uid_ = uuid.left(16).toULongLong(0, 16);
    qDebug() << "new uuid" << uuid << uid_;
    query("insert into users (uid, home) values (:a, :b)", uid_, user);
  }
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

  q = constQuery("select scid, nodes.nid, typ from syncons"
                 " inner join nodes on syncons.nid==nodes.nid where sid==:a",
		 sid);
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

SBEMDB::Node SBEMDB::somaAt(Point const &p,
                            int xytol, int ztol, quint64 tid) const {
  // Prefer to return from tid, but will accept other tree.
  // If all else fails, will select from tid with ztol ignored.
  QVector<Node> nn = nodes(constQuery("select * from nodes where tid==:a"
				      " and typ==1"
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
			" and typ==1"
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
			" and typ==1"
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

quint64 SBEMDB::createNodeCon(quint64 nid1, quint64 nid2) {
  return query("insert into nodecons(nid1,nid2,cdate,uid)"
               " values(:a,:b,:c,:d)",
               nid1, nid2,
               now(), uid()).lastInsertId().toULongLong();
}

void SBEMDB::createNodeConPair(quint64 nid1, quint64 nid2) {
  createNodeCon(nid1, nid2);
  createNodeCon(nid2, nid1);
}

quint64 SBEMDB::createSynCon(quint64 sid, quint64 nid) {
  return query("insert into syncons(sid, nid, cdate,uid)"
               " values(:a,:b,:c,:d)",
               sid, nid,
               now(), uid()).lastInsertId().toULongLong();
}

quint64 SBEMDB::createNode(quint64 tid, NodeType typ, Point const &p) {
  return query("insert into nodes(tid,typ,x,y,z,cdate,uid)"
               " values(:a,:b,:c,:d,:e,:f,:g)",
               tid, typ, p.x, p.y, p.z,
               now(), uid()).lastInsertId().toULongLong();
}

quint64 SBEMDB::createTag(quint64 nid, QString tag) {
  return query("insert into tags (nid, tag, cdate, uid)"
               " values(:a, :b, :c, :d)",
               nid, tag, now(), uid()).lastInsertId().toULongLong();
}

void SBEMDB::updateTag(quint64 tagid, QString tag) {
  query("update tags set tag=:a where tagid==:b",
        tag, tagid);
}

quint64 SBEMDB::createTree(QString tname, bool vis) {
  return query("insert into trees (tname, visible, cdate, uid)"
               " values (:a, :b, :c, :d)",
               tname, vis, now(), uid()).lastInsertId().toULongLong();
}

quint64 SBEMDB::createSynapse() {
  return query("insert into synapses (cdate, uid) values ( :a, :b )",
               now(), uid()).lastInsertId().toULongLong();
}


quint64 SBEMDB::uid() const {
  return uid_;
}
