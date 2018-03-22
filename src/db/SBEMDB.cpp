// SBEMDB.cpp

#include "SBEMDB.h"
#include <QFile>
#include "PDebug.h"
#include "SqlFile.h"

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

SBEMDB::SimpleSynapse SBEMDB::simpleSynapse(quint64 sid) const {
  auto lst
    = simpleSynapses(constQuery("select * from simplesynapses where sid==:a",
                                sid));
  return lst.isEmpty() ? SimpleSynapse() : lst.first();
}

SBEMDB::PolySynapse SBEMDB::polySynapse(quint64 sid) const {
  auto lst
    = polySynapses(constQuery("select * from polysynapses where sid==:a",
                              sid));
  return lst.isEmpty() ? PolySynapse() : lst.first();
}  

SBEMDB::SynCon SBEMDB::preSynCon(quint64 scid) const {
  auto lst
    = preSynCons(constQuery("select * from presyncon where scid==:a",
                            scid));
  return lst.isEmpty() ? SynCon() : lst.first();
}

SBEMDB::SynCon SBEMDB::postSynCon(quint64 scid) const {
  auto lst
    = postSynCons(constQuery("select * from postsyncon where scid==:a",
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

QVector<SBEMDB::SimpleSynapse> SBEMDB::simpleSynapses(QSqlQuery q) const {
  QVector<SimpleSynapse> lst;
  while (q.next()) {
    lst << SimpleSynapse{q.value(0).toULongLong(),
        q.value(1).toULongLong(), q.value(2).toULongLong()};
  }
  return lst;
}

QVector<SBEMDB::PolySynapse> SBEMDB::polySynapses(QSqlQuery q) const {
  QVector<PolySynapse> lst;
  while (q.next()) {
    lst << PolySynapse{q.value(0).toULongLong()};
  }
  return lst;
}

QVector<SBEMDB::SynCon> SBEMDB::preSynCons(QSqlQuery q) const {
  QVector<SynCon> lst;
  while (q.next()) {
    lst << SynCon{q.value(0).toULongLong(),
        q.value(1).toULongLong(), q.value(2).toULongLong()};
  }
  return lst;
}

QVector<SBEMDB::SynCon> SBEMDB::postSynCons(QSqlQuery q) const {
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
        q.value(1).toInt(), q.value(2).toInt(), q.value(3).toInt(),
        q.value(4).toString()};
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

SBEMDB::Synapse SBEMDB::synapse(quint64 sid) const {
  SBEMDB::Synapse res;
  QSqlQuery q = constQuery("select * from simplesynapses where sid=:a", sid);
  if (q.next()) {
    res.sid = q.value(0).toULongLong();
    res.pre[0] = q.value(1).toULongLong();
    res.post[0] = q.value(2).toULongLong();
    return res;
  }
  if (!constQuery("select * from polysynapses where sid=:a", sid).next())
    return res;
  q = constQuery("select * from presyncon where sid==:a", sid);
  while (q.next())
    res.pre[q.value(0).toULongLong()] = q.value(2).toULongLong();
  q = constQuery("select * from postsyncon where sid==:a", sid);
  while (q.next())
    res.post[q.value(0).toULongLong()] = q.value(2).toULongLong();
  return res;
}


