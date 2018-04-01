// EditOverlay.cpp

#include "EditOverlay.h"
#include <QDebug>
#include <QPainter>
#include <QKeyEvent>

EditOverlay::EditOverlay(SBEMDB *db, QWidget *parent):
  Overlay(parent), db(db) {
  presspt = Point(-1000, -1000, -1000); // i.e., not real
  tid = 0; // i.e., none
  nid = 0;
  aux_nid = 0;
  mode = Mode_View;
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

void EditOverlay::paint(QPainter *p,
			QRect const &, class ViewInfo const &vi) {
  if (!db->isOpen())
    return;
  drawOtherTrees(p, vi);
  drawActiveTree(p, vi);
  drawAuxNid(p, vi);
}

void EditOverlay::drawOtherTrees(QPainter *p, ViewInfo const &vi) {
  int nr = nodeSBEMRadius(vi.a);
  int sr = nodeScreenRadius(vi.a);
  
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

  for (auto const &c: viscons) {
    auto n1 = db->node(c.nid1);
    auto n2 = db->node(c.nid2);
    int dz = n1.z + n2.z - 2*vi.z;
    p->setPen(QPen(otherEdgeColor(dz), 5));
    p->drawLine(QPoint((n1.x - vi.xl)>>vi.a, (n1.y - vi.yt)>>vi.a),
                QPoint((n2.x - vi.xl)>>vi.a, (n2.y - vi.yt)>>vi.a));
  }

  p->setPen(QPen(Qt::NoPen));
  for (auto const &n: visnodes) {
    int dz = n.z - vi.z;
    p->setBrush(QBrush(otherNodeColor(dz)));
    int r = (dz==0) ? sr : sr*3/4;
    p->drawEllipse(QPoint((n.x - vi.xl)>>vi.a, (n.y - vi.yt)>>vi.a), r, r);
  }
  
  db->query("drop table visnodes");
}

void EditOverlay::drawAuxNid(QPainter *p, ViewInfo const &vi) {
  if (!aux_nid)
    return;
  auto n = db->node(aux_nid);
  if (!n.nid)
    return; // can't find the node?? this shouldn't happen
  
  int dz = n.z - vi.z;
  p->setBrush(QBrush(n.tid==tid ? nodeColor(dz) : otherNodeColor(dz)));
  int sr = nodeScreenRadius(vi.a);
  int r = dz==0 ? 3*sr/2 : sr;
  p->setPen(QPen(QColor(255, 0, 0), 5));
  QPoint pc((n.x - vi.xl)>>vi.a, (n.y - vi.yt)>>vi.a);
  QPoint dpx(r, 0);
  QPoint dpy(0, r);
  p->drawPolygon(QPolygon() << pc + dpx << pc + dpy << pc - dpx << pc - dpy);
}                   
  
void EditOverlay::drawActiveTree(QPainter *p, ViewInfo const &vi) {
  int nr = nodeSBEMRadius(vi.a);
  int sr = nodeScreenRadius(vi.a);
  
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

  for (auto const &c: viscons) {
    auto n1 = db->node(c.nid1);
    auto n2 = db->node(c.nid2);
    int dz = n1.z + n2.z - 2*vi.z;
    p->setPen(QPen(edgeColor(dz), 5));
    p->drawLine(QPoint((n1.x - vi.xl)>>vi.a, (n1.y - vi.yt)>>vi.a),
                QPoint((n2.x - vi.xl)>>vi.a, (n2.y - vi.yt)>>vi.a));
  }

  p->setPen(QPen(Qt::NoPen));
  for (auto const &n: visnodes) {
    qDebug() << "paint node" << n.nid;
    int dz = n.z - vi.z;
    p->setBrush(QBrush(nodeColor(dz)));
    int r = dz==0 ? sr : 3*sr/4;
    if (n.nid==nid) 
      p->setPen(QPen(QColor(255, 0, 0), 5));
    p->drawEllipse(QPoint((n.x - vi.xl)>>vi.a, (n.y - vi.yt)>>vi.a), r, r);
    if (n.nid==nid)
      p->setPen(QPen(Qt::NoPen));
  }
  
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
  qDebug() << "EditOverlay::press" << p << b << m << a;
  if (tid==0) {
    qDebug() << "no tree";
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
  qDebug() << "node" << n.nid;
  if (n.nid>0) {
    if (n.tid==tid) {
      nid = n.nid;
      forceUpdate();
    } else {
      emit otherTreePressed(n.tid, n.nid);
    }
  } else if (mode==Mode_Edit) {
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
  //  qDebug() << "EditOverlay::release" << p << b << m << a;
  presspt = Point(-1000, -1000, -1000);
  return false;
}

bool EditOverlay::mouseMove(Point const &p,
			    Qt::MouseButtons bb, Qt::KeyboardModifiers,
			    int) {
  if (bb & Qt::LeftButton && presspt.x>=0 && nid>0) {
    if (mode==Mode_Edit) {
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
  qDebug() << "setactivetree" << tid;
  nid = 0;
  aux_nid = 0;
  forceUpdate();
}

void EditOverlay::setActiveNode(quint64 nid1) {
  nid = nid1;
  aux_nid = 0;
  qDebug() << "setactivenode" << nid;
  if (nid) {
    auto n = db->node(nid);
    tid = n.tid;
  }
  forceUpdate();
}

bool EditOverlay::keyPress(QKeyEvent *e) {
  if (mode != Mode_Edit)
    return false;
  qDebug() << e->key() << Qt::Key_Insert << Qt::Key_Delete;
  switch (e->key()) {
  case Qt::Key_Delete:
    if (aux_nid && nid)
      deleteSelectedConnection();
    else if (nid)
      deleteSelectedNode();
    return true;
  case Qt::Key_Insert:
    if (aux_nid && nid)
      insertSelectedConnection();
    return true;
  default:
    break;
  }
  return false;
}

void EditOverlay::deleteSelectedConnection() {
}

void EditOverlay::insertSelectedConnection() {
  auto n1 = db->node(nid);
  auto n2 = db->node(aux_nid);
  qDebug() <<"insert" << n1.nid << n1.tid << n2.nid << n2.tid;
  if (!n1.nid || !n2.nid)
    return;
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

void EditOverlay::deleteSelectedNode() {
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

void EditOverlay::setMode(Mode m) {
  mode = m;
}
