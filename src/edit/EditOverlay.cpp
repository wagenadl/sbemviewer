// EditOverlay.cpp

#include "EditOverlay.h"
#include <QDebug>
#include <QPainter>
#include <QKeyEvent>
#include <QInputDialog>
#include "TileViewer.h"
#include "Miniball.h"

EditOverlay::EditOverlay(SBEMDB *db, QWidget *parent):
  Overlay(parent), db(db) {
  presspt = Point(-1000, -1000, -1000); // i.e., not real
  tid = 0; // i.e., none
  nid = 0;
  aux_nid = 0;
}

EditOverlay::~EditOverlay() {
}

static QColor edgeColor(int dz) {
  // dz can be up to ±20
  if (dz==0)
    return QColor(255, 235, 0);
  else if (dz>0)
    return QColor(200-3*(dz>20?20:dz), 255-1*(dz>20?20:dz), 0);
  else 
    return QColor(255, 200-5*(dz<-20?20:-dz), 0);
} 

static QColor otherEdgeColor(int dz) {
  // dz can be up to ±20
  if (dz==0)
    return QColor(0, 255, 255);
  else if (dz>0)
    return QColor(0, 200-10*(dz>20?20:dz), 255);
  else 
    return QColor(0, 255-5*(dz<-20?20:-dz), 200-8*(dz<-20?20:-dz));
}

static QColor nodeColor(int dz) {
  return edgeColor(2*dz);
}

static QColor otherNodeColor(int dz) {
  return otherEdgeColor(2*dz);
}

void EditOverlay::drawCons(QPainter *p, ViewInfo const &vi,
                           QVector<SBEMDB::NodeCon> const &cons,
                           QColor (*colorfn)(int)) {
  QSet<QPair<quint64, quint64>> seen;
  for (auto const &c: cons) {
    if (seen.contains(QPair<quint64, quint64>(c.nid1, c.nid2)))
      continue;
    seen << QPair<quint64, quint64>(c.nid2, c.nid1);
    auto n1 = db->node(c.nid1);
    auto n2 = db->node(c.nid2);
    int dz = n1.z + n2.z - 2*vi.z;
    p->setPen(QPen(colorfn(dz), 5));
    p->drawLine(QPoint((n1.x - vi.xl)>>vi.a, (n1.y - vi.yt)>>vi.a),
                QPoint((n2.x - vi.xl)>>vi.a, (n2.y - vi.yt)>>vi.a));
  }
}

static void drawNode(QPainter *p, ViewInfo const &vi, int r,
                     SBEMDB::Node const &n) {
  QPoint pc((n.x - vi.xl)>>vi.a, (n.y - vi.yt)>>vi.a);
  QPoint rx(r, 0);
  QPoint ry(0, r);
  switch (n.typ) {
  case SBEMDB::TreeNode:
    p->drawEllipse(pc, r, r);
    break;
  case SBEMDB::PresynTerm:
    p->drawPolygon(QPolygon() << pc - rx - ry << pc - rx + ry << pc + rx);
    break;
  case SBEMDB::PostsynTerm:
    p->drawPolygon(QPolygon() << pc + rx - ry << pc + rx + ry << pc - rx);
    break;
  case SBEMDB::Soma:
    p->drawEllipse(pc, 3*r, 3*r);
    break;
  case SBEMDB::ExitPoint:
    p->drawRect(QRect(pc-2*rx-2*ry, pc+2*rx+2*ry));
    break;
  default:
    break;
  }
}  

void EditOverlay::drawNodes(QPainter *p, ViewInfo const &vi,
                           QVector<SBEMDB::Node> const &nodes,
                           QColor (*colorfn)(int)) {
  int sr = nodeScreenRadius(vi.a);
  p->setPen(QPen(Qt::NoPen));
  for (auto const &n: nodes) {
    int dz = n.z - vi.z;
    int r = (dz==0) ? sr : sr*3/4;
    p->setBrush(QBrush(colorfn(dz)));
    if (n.nid==nid) 
      p->setPen(QPen(QColor(255, 0, 0), 5));
    drawNode(p, vi, r, n);
    if (n.nid==nid)
      p->setPen(QPen(Qt::NoPen));
  }
}


void EditOverlay::paint(QPainter *p,
			QRect const &, class ViewInfo const &vi) {
  if (!db->isOpen())
    return;
  drawSynapses(p, vi);
  drawOtherTrees(p, vi);
  drawActiveTree(p, vi);
  drawAuxNid(p, vi);
  drawTags(p, vi);
}

void EditOverlay::drawTags(QPainter *p, ViewInfo const &vi) {
  int nr = nodeSBEMRadius(vi.a);
  int sr = nodeScreenRadius(vi.a);
  
  QSqlQuery q = db->query("select x, y, z, tag, tid, visible from tags"
                          " natural join nodes"
                          " natural join trees"
                          " where z>=:a and z<=:b"
                          " and x>=:c and x<:d"
                          " and y>=:e and y<:f",
                          vi.z - ZTOLERANCE, vi.z + ZTOLERANCE,
                          vi.xl - nr, vi.xr + nr,
                          vi.yt - nr, vi.yb + nr);
  while (q.next()) {
    int x = q.value(0).toInt();
    int y = q.value(1).toInt();
    int z = q.value(2).toInt();
    QString tag = q.value(3).toString();
    quint64 tid1 = q.value(4).toULongLong();
    bool vis = q.value(5).toBool();
    if (tid1==tid || vis) {
      QColor c = tid1==tid ? nodeColor(z - vi.z) : otherNodeColor(z - vi.z);
      p->setPen(QPen(c));
      QPoint pc((x - vi.xl)>>vi.a, (y - vi.yt)>>vi.a);
      p->drawText(pc + QPoint(sr, -sr), tag);
    }
  }
}

void EditOverlay::drawSynapses(QPainter *p, ViewInfo const &vi) {
  int sr = nodeScreenRadius(vi.a);
  int nr = nodeSBEMRadius(vi.a);

  QSqlQuery q = db->query("select distinct sid from syncons"
                          " natural join nodes"
                          " where z>=:a and z<=:b"
                          " and x>=:c and x<:d"
                          " and y>=:e and y<:f",
                          vi.z - ZTOLERANCE, vi.z + ZTOLERANCE,
                          vi.xl - nr, vi.xr + nr,
                          vi.yt - nr, vi.yb + nr);
  QVector<quint64> sids;
  while (q.next())
    sids << q.value(0).toULongLong();

  for (quint64 sid: sids) {
    QVector<Point> nodepos;
    bool isActive = false;
    q = db->query("select x,y,z,tid from nodes natural join syncons"
                  " where sid==:a", sid);
    while (q.next()) {
      nodepos << Point(q.value(0).toInt(),
                       q.value(1).toInt(),
                       q.value(2).toInt());
      if (q.value(3).toULongLong() == tid)
        isActive = true;
    }
    Miniball ball(nodepos);
    int dz = ball.center().z - vi.z;

    QColor c = isActive ? nodeColor(dz) : otherNodeColor(dz);
    p->setPen(QPen(c, 5, Qt::DotLine));
    p->setBrush(Qt::NoBrush);
    int r = (ball.radius()>>vi.a) + sr/2;
    p->drawEllipse(QPoint((ball.center().x - vi.xl)>>vi.a,
                          (ball.center().y - vi.yt)>>vi.a),
                   r, r);
  }
}

void EditOverlay::drawOtherTrees(QPainter *p, ViewInfo const &vi) {
  int nr = nodeSBEMRadius(vi.a);
  
  db->query("create temp table visnodes as select nid from nodes"
            " where tid!=:a"
            " and tid in (select tid from trees where visible>0)"
            " and z>=:b and z<=:c"
            " and x>=:d and x<:e"
            " and y>=:f and y<:g",
            tid,
            vi.z - ZTOLERANCE, vi.z + ZTOLERANCE,
            vi.xl - nr, vi.xr + nr,
            vi.yt - nr, vi.yb + nr);
  
  auto visnodes = db->nodes(db->constQuery("select * from nodes where nid in"
                                           " ( select * from visnodes )"));

  auto viscons = db->nodeCons(db->constQuery("select * from nodecons"
                                             " where nid1 in"
                                             " ( select * from visnodes )"));

  drawCons(p, vi, viscons, &otherEdgeColor);
  drawNodes(p, vi, visnodes, &otherNodeColor);

  db->query("drop table visnodes");
}

void EditOverlay::drawAuxNid(QPainter *p, ViewInfo const &vi) {
  if (!aux_nid)
    return;
  auto n = db->node(aux_nid);
  if (!n.nid)
    return; // can't find the node?? this shouldn't happen
  
  int dz = n.z - vi.z;
  p->setBrush(QColor(255, 0, 0));
  int sr = nodeScreenRadius(vi.a);
  int r = dz==0 ? sr : 3*sr/4;
  p->setPen(QPen(Qt::NoPen));
  drawNode(p, vi, r, n);
}

bool EditOverlay::isTreeVisible(quint64 tid1, ViewInfo const &vi) const {
  int nr = nodeSBEMRadius(vi.a);
  
  int n = db->simpleQuery("select count(1) from nodes"
                          " where tid==:a"
                          " and z>=:b and z<=:c"
                          " and x>=:d and x<:e"
                          " and y>=:f and y<:g",
                          tid1,
                          vi.z - ZTOLERANCE, vi.z + ZTOLERANCE,
                          vi.xl - nr, vi.xr + nr,
                          vi.yt - nr, vi.yb + nr).toInt();
  return n>0;
}

Point EditOverlay::goodSpotForTree(quint64 tid1,
                                   ViewInfo const &vi, bool *ok) const {
  // First, make sure the tree has points, otherwise this won't work.
  int n  = db->simpleQuery("select count(1) from nodes"
                           " where tid==:a", tid1).toInt();
  if (ok)
    *ok = n>0;
  if (n==0) {
    qDebug() << "goodSpotForTree: empty tree";
    return Point();
  }
  
  // Then, find median Z
  int z0 = db->simpleQuery("select z from nodes where tid==:a order by z"
                           " limit 1 offset :b", tid1, int(n/2)).toInt();
  // Now, find all points that are visible if we focus on that plane
  db->query("create temp table visz as select x,y from nodes"
            " where tid==:a"
            " and z>=:b and z<=:c",
            tid1, z0 - ZTOLERANCE, z0 + ZTOLERANCE);
  n = db->simpleQuery("select count(1) from visz").toInt();
  // Now, find median X
  int x0 = db->simpleQuery("select x from visz order by x"
                           " limit 1 offset :a", int(n/2)).toInt();
  int dx = vi.xr - vi.xl;
  // Now, find all points that are within a screen width of X
  db->query("create temp table visy as select y from visz"
            " where x>=:a and x<=:b",
            x0 - dx/2, x0 + dx/2);
  n = db->simpleQuery("select count(1) from visy").toInt();
  // and find median Y
  int y0 = db->simpleQuery("select y from visy order by y"
                           " limit 1 offset :a", int(n/2)).toInt();
  db->query("drop table visy");
  db->query("drop table visz");
  return Point(x0, y0, z0);
}  
  
void EditOverlay::drawActiveTree(QPainter *p, ViewInfo const &vi) {
  int nr = nodeSBEMRadius(vi.a);
  
  db->query("create temp table visnodes as select nid from nodes"
            " where tid==:a"
            " and z>=:b and z<=:c"
            " and x>=:d and x<:e"
            " and y>=:f and y<:g",
            tid,
            vi.z - ZTOLERANCE, vi.z + ZTOLERANCE,
            vi.xl - nr, vi.xr + nr,
            vi.yt - nr, vi.yb + nr);
  auto visnodes = db->nodes(db->constQuery("select * from nodes where nid in"
                                           " ( select * from visnodes )"));

  auto viscons = db->nodeCons(db->constQuery("select * from nodecons"
                                             " where nid1 in"
                                             " ( select * from visnodes )"));

  drawCons(p, vi, viscons, &edgeColor);
  drawNodes(p, vi, visnodes, &nodeColor);
  
  db->query("drop table visnodes");
}

int EditOverlay::nodeScreenRadius(int) {
  return 12;
  //  int r = 20 >> a;
  //if (r<5)
  //  r = 5;
  //return r;
}

int EditOverlay::nodeSBEMRadius(int a) {
  return nodeScreenRadius(a) << a;
}

bool EditOverlay::mousePress(Point const &p,
			     Qt::MouseButton b, Qt::KeyboardModifiers m,
			     int a) {
  if (tid==0) {
    return false;
  }
  presspt = p;
  if (b==Qt::LeftButton && m==Qt::NoModifier) {
    return plainLeftPress(p, a);
  } else if (b==Qt::LeftButton && m==Qt::ShiftModifier) {
    SBEMDB::Node n = db->nodeAt(p, 2*nodeSBEMRadius(a), ZTOLERANCE, tid);
    aux_nid = n.nid;
    forceUpdate();
  }
  return false;
}

bool EditOverlay::plainLeftPress(Point const &p, int a) {
  if (aux_nid) {
    aux_nid = 0;
    forceUpdate();
  }
  SBEMDB::Node n = db->nodeAt(p, 2*nodeSBEMRadius(a), ZTOLERANCE, tid);
  if (n.nid>0) {
    if (n.tid==tid) {
      nid = n.nid;
      db->selectNode(nid);
      forceUpdate();
    } else {
      emit otherTreePressed(n.tid, n.nid);
    }
  } else if (mode()==Mode_Edit) {
    // create new node
    if (nid==0) { // no current selection
      int nnodes = db->simpleQuery("select count(*) from nodes"
                                   " where tid==:a", tid).toInt();
      if (nnodes==0) {
        // create first node for tree
        nid = db->query("insert into nodes(tid,typ,x,y,z)"
                        " values(:a,:b,:c,:d,:e)",
                        tid, SBEMDB::TreeNode,
                        p.x, p.y, p.z).lastInsertId().toULongLong();
        db->selectNode(nid);
        forceUpdate();
      } else {
        qDebug() << "no node selected. Won't expand.";
      }
    } else {
      // have current selection; connect to it.
      quint64 nid1 = db->query("insert into nodes(tid,typ,x,y,z)"
                               " values(:a,:b,:c,:d,:e)",
                               tid, SBEMDB::TreeNode,
                               p.x, p.y, p.z).lastInsertId().toULongLong();
      db->query("insert into nodecons(nid1,nid2) values(:a,:b)",
                nid, nid1);
      db->query("insert into nodecons(nid1,nid2) values(:a,:b)",
                nid1, nid);
      nid = nid1;
      db->selectNode(nid);
      forceUpdate();
    }
  }
  if (nid) {
    auto n = db->node(nid);
    origpt = Point(n.x, n.y, n.z);
  }
  return true;
}

bool EditOverlay::mouseRelease(Point const &,
			       Qt::MouseButton, Qt::KeyboardModifiers,
			       int) {
  presspt = Point(-1000, -1000, -1000);
  return false;
}

bool EditOverlay::mouseMove(Point const &p,
			    Qt::MouseButtons bb, Qt::KeyboardModifiers,
			    int) {
  if (bb & Qt::LeftButton && presspt.x>=0 && nid>0) {
    if (mode()==Mode_Edit) {
      Point dp = p - presspt;
      Point newpt = origpt + dp;
      db->query("update nodes set x=:a, y=:b, z=:c where nid==:d",
                newpt.x, newpt.y, newpt.z, nid);
      forceUpdate();
    }
    return true;
  }
    
  return false;
}

void EditOverlay::setActiveTree(quint64 tid1) {
  tid = tid1;
  nid = 0;
  aux_nid = 0;

  TileViewer *tv = dynamic_cast<TileViewer *>(parentWidget());
  if (tv) {
    // can see if our tree is visible
    ViewInfo vi = tv->currentView();
    if (!isTreeVisible(tid, vi)) {
      bool ok;
      Point p = goodSpotForTree(tid, vi, &ok);
      if (ok)
        emit gotoNodeRequest(p);
    }
  }
  
  forceUpdate();
}

void EditOverlay::setActiveNode(quint64 nid1) {
  nid = nid1;
  aux_nid = 0;
  if (nid) {
    auto n = db->node(nid);
    tid = n.tid;
  }
  forceUpdate();
}

void EditOverlay::actEditMemo() {
  if (nid) {
    auto tags = db->tags(db->constQuery("select * from tags where nid==:a",
                                        nid));
    QString tag = tags.isEmpty() ? "" : tags.first().tag;
    bool ok;
    tag = QInputDialog::getText(parentWidget(),
                                "Memo",
                                "Set memo for node:",
                                QLineEdit::Normal,
                                tag,
                                &ok);
    if (ok) {
      if (tag.isEmpty()) {
        if (!tags.isEmpty())
          db->query("delete from tags where tagid==:a", tags.first().tagid);
      } else {
        if (tags.isEmpty())
          db->query("insert into tags (nid, tag) values(:a, :b)",
                    nid, tag);
        else
          db->query("update tags set tag=:a where tagid==:b",
                    tag, tags.first().tagid);
      }
    }
  }
}

void EditOverlay::actSetNodeType(SBEMDB::NodeType typ) {
  if (nid) {
    // Probably we should check that a synaptic terminal isn't being
    // changed into something else while it is still part of a synapse.
    // For now, this issue is not critical.
    db->query("update nodes set typ=:a where nid==:b",
              typ, nid);
    forceUpdate();
  }
}

void EditOverlay::actDeleteNode() {
  if (!nid || aux_nid)
    return;
  // delete nid
  auto cons = db->nodeCons(db->constQuery("select * from nodecons"
                                          " where nid1==:a", nid));
  bool candelete = nid>0;
  switch (cons.size()) {
  case 0: // can simply delete this thing without consequence
    break;
  case 1: // must delete one pair of segments
    break;
  case 2: // must delete two pairs of segments; may have to reconnect
    if (db->simpleQuery("select count(1) from nodecons"
                        " where nid1==:a"
                        " and nid2==:b",
                        cons.first().nid2,
                        cons.last().nid2) == 0) {
      // must reconnect
      db->query("insert into nodecons(nid1,nid2) values(:a,:b)",
                cons.first().nid2, cons.last().nid2);
      db->query("insert into nodecons(nid1,nid2) values(:a,:b)",
                cons.last().nid2, cons.first().nid2);
    }
    break;
  default:
    candelete = false;
  }
  if (candelete) {
    db->query("delete from nodes where nid==:a", nid);
    // this propagates deleting the connections
    nid = 0;
    db->selectNode(0);
    forceUpdate();
  }
}

void EditOverlay::actDisconnectNodes() {
  if (!aux_nid || !nid)
    return;
  auto n1 = db->node(nid);
  auto n2 = db->node(aux_nid);
  if (!n1.nid || !n2.nid) {
    qDebug() << "Nodes not found while trying to delete connection";
    return;
  }
  if (n1.tid != n2.tid) {
    qDebug() << "Nodes not in same tree while trying to delete connection";
    return;
  }
  
  auto cons = db->nodeCons(db->constQuery("select * from nodecons"
                                          " where nid1==:a and nid2==:b",
                                          n1.nid, n2.nid));
  if (cons.isEmpty()) {
    qDebug() << "No connection to delete";
    return;
  }

  /* Our strategy will be to first remove the connection, then find all
     nodes connected to n2 and create a new tree for them.
  */
  db->begin();
  db->query("delete from nodecons where nid1==:a and nid2==:b",
            n1.nid, n2.nid);
  db->query("delete from nodecons where nid1==:a and nid2==:b",
            n2.nid, n1.nid);
  QSet<quint64> seennodes;
  QSet<quint64> newnodes;
  newnodes << n2.nid;
  while (!newnodes.isEmpty()) {
    quint64 n = *newnodes.begin();
    newnodes.erase(newnodes.begin());
    seennodes << n;
    auto cons = db->nodeCons(db->constQuery("select * from nodecons"
                                            " where nid1==:a", n));
    for (auto c: cons) 
      if (!seennodes.contains(c.nid2))
        newnodes << c.nid2;
  }
  QString name = db->simpleQuery("select tname from trees where tid==:a",
                                 n1.tid).toString();
  quint64 newtid = db->query("insert into trees (tname, visible)"
                             " values (:a, :b)", name + "'", true)
    .lastInsertId().toULongLong();
  for (quint64 n: seennodes) 
    db->query("update nodes set tid=:a where nid==:b", newtid, n);
  db->commit();
  emit treeTableAltered();
  emit otherTreePressed(n1.tid, n1.nid);
  forceUpdate();
}

void EditOverlay::actConnectNodes() {
  if (!aux_nid || !nid)
    return;

  auto n1 = db->node(nid);
  auto n2 = db->node(aux_nid);
  if (!n1.nid || !n2.nid) {
    qDebug() << "Nodes not found while trying to insert connection";
    return;
  }
  if (n1.tid==n2.tid) {
    // this would create a circular graph
    qDebug() << "Refusing to create a circular graph";
  } else {
    // must merge trees
    db->begin();
    QString name1 = db->simpleQuery("select tname from trees where tid==:a",
                                    n1.tid).toString();
    QString name2 = db->simpleQuery("select tname from trees where tid==:a",
                                    n2.tid).toString();
    db->query("update nodes set tid=:a where tid==:b", n1.tid, n2.tid);
    db->query("delete from trees where tid==:a", n2.tid);
    db->query("insert into nodecons (nid1, nid2) values (:a,:b), (:c,:d)",
              n1.nid, n2.nid,
              n2.nid, n1.nid);
    QStringList nn1 = name1.split(" ");
    QStringList nn2 = name2.split(" ");
    for (QString n: nn1) 
      if (!nn2.isEmpty() && nn2.first()==n)
        nn2.removeFirst();
      else
        break;
    db->query("update trees set tname=:a where tid==:b",
              name1 + " + " + nn2.join(" "), n1.tid);
    db->commit();
    emit treeTableAltered();
    emit otherTreePressed(n1.tid, n1.nid);
    forceUpdate();
  }
}

void EditOverlay::actConnectTerminals() {
  // Connect nid and aux_nid to the same synapse.
  // In future, this could potentially connect nid to nearby synapse
  // even if there is no aux_nid.
  if (!nid || !aux_nid)
    return;

  SBEMDB::Node n1 = db->node(nid);
  SBEMDB::Node n2 = db->node(nid);
  if (!(n1.typ==SBEMDB::PresynTerm || n1.typ==SBEMDB::PostsynTerm)
      || !(n2.typ==SBEMDB::PresynTerm || n2.typ==SBEMDB::PostsynTerm)) {
        // not genuinely plausible partners
    qDebug() << "Selected node are not synaptic terminals";
    return;
  }

  // Let's see if a synapse already exists
  QSqlQuery q = db->query("select sid from syncons where nid==:a", nid);
  quint64 sid = q.next() ? q.value(0).toULongLong() : 0;
  q = db->query("select sid from syncons where nid==:a", aux_nid);
  quint64 aux_sid = q.next() ? q.value(0).toULongLong() : 0;

  if (sid==0 && aux_sid==0) {
    // must make a brand new synapse
    db->begin();
    sid = db->query("insert into synapses values ( null )")
      .lastInsertId().toULongLong();
    db->query("insert into syncons ( sid, nid ) values (:a, :b)", sid, nid);
    db->query("insert into syncons ( sid, nid ) values (:a, :b)", sid, aux_nid);
    db->commit();
  } else if (sid==0) {
    // must insert nid into existing synapse
    db->query("insert into syncons ( sid, nid ) values (:a, :b)", aux_sid, nid);
  } else if (aux_sid==0) {
    // must insert aux_nid into existing synapse
    db->query("insert into syncons ( sid, nid ) values (:a, :b)", sid, aux_nid);
  } else {
    // must merge synapses
    db->begin();
    db->query("update syncons set sid=:a where sid==:b", sid, aux_sid);
    db->query("delete from synapses where sid==:a", aux_sid);
    db->commit();
  }
  forceUpdate();
}

void EditOverlay::actDissolveSynapse() {
  if (!nid)
    return;
  QSqlQuery q = db->query("select sid from syncons where nid==:a", nid);
  if (!q.next())
    return;
  quint64 sid = q.value(0).toULongLong();
  db->query("delete from synapses where sid==:a", sid);
  forceUpdate();
}

void EditOverlay::actCenterNode() {
  if (nid) {
    auto node = db->node(nid);
    if (node.nid)
      emit gotoNodeRequest(Point(node.x, node.y, node.z));
  }
}

bool EditOverlay::keyPress(QKeyEvent *) {
  return false;
}

