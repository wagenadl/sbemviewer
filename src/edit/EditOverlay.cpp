// EditOverlay.cpp

#include "EditOverlay.h"
#include <QDebug>
#include <QPainter>

EditOverlay::EditOverlay(SBEMDB *db, QWidget *parent):
  Overlay(parent), db(db) {
  presspt = Point(-1000, -1000, -1000); // i.e., not real
  tid = 0; // i.e., none
  nid = 0;
}

EditOverlay::~EditOverlay() {
}

void EditOverlay::paint(QPainter *p,
			QRect const &, class ViewInfo const &vi) {
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
  for (auto const &n: visnodes) {
    qDebug() << "paint node" << n.nid;
    int dz = n.z - vi.z;
    if (n.nid == nid) {
      // paint selected node
      p->setPen(QPen(QColor(255, 0, 0), 2));
    } else if (n.z>vi.z) {
      // paint node in deeper layer
      p->setPen(QPen(QColor(0, 95 + 16*dz, 255), 2));
    } else if (n.z<vi.z) {
      // paint node in more superficial layer
      p->setPen(QPen(QColor(95 - 16*dz, 0, 255), 2));
    } else {
      // paint node in this layer
      p->setPen(QPen(QColor(64, 64, 255), 2));
    }
    p->drawEllipse(QPoint((n.x - vi.xl)>>vi.a, (n.y - vi.yt)>>vi.a), sr, sr);
  }

  auto viscons = db->nodeCons(db->constQuery("select * from nodecons"
                                             " where nid1 in"
                                             " ( select * from visnodes )"));
  for (auto const &c: viscons) {
    auto n1 = db->node(c.nid1);
    auto n2 = db->node(c.nid2);
    int dz = n1.z + n2.z - 2*vi.z;
    if (dz>0) {
      // deeper
      p->setPen(QPen(QColor(0, 95 + 8*dz, 255), 5));
    } else if (dz<0) {
      // more superficial
      p->setPen(QPen(QColor(95 - 8*dz, 0, 255), 5));
    } else {
      // contained in this z
      p->setPen(QPen(QColor(64, 64, 255), 5));
    }
    p->drawLine(QPoint((n1.x - vi.xl)>>vi.a, (n1.y - vi.yt)>>vi.a),
                QPoint((n2.x - vi.xl)>>vi.a, (n2.y - vi.yt)>>vi.a));
  }
  
  db->query("drop table visnodes");
}

int EditOverlay::nodeScreenRadius(int a) {
  int r = 20 >> a;
  if (r<5)
    r = 5;
  return r;
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
    SBEMDB::Node n = db->nodeAt(p, nodeSBEMRadius(a), ZTOLERANCE, tid);
    qDebug() << "node" << n.nid;
    if (n.nid>0) {
      nid = n.nid;
      forceUpdate();
    } else {
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
  }
  return false;
}

bool EditOverlay::mouseRelease(Point const &p,
			       Qt::MouseButton b, Qt::KeyboardModifiers m,
			       int a) {
  //  qDebug() << "EditOverlay::release" << p << b << m << a;
  presspt = Point(-1000, -1000, -1000);
  return false;
}

bool EditOverlay::mouseMove(Point const &p,
			    Qt::MouseButton b, Qt::KeyboardModifiers m,
			    int a) {
  //  qDebug() << "EditOverlay::move" << p << b << m << a;
  return false;
}

void EditOverlay::setActiveTree(quint64 tid1) {
  tid = tid1;
  qDebug() << "setactivetree" << tid;
  nid = 0;
  forceUpdate();
}

 void EditOverlay::setActiveNode(quint64 nid1) {
  nid = nid1;
  qDebug() << "setactivenode" << nid;
  if (nid) {
    auto n = db->node(nid);
    tid = n.tid;
  }
  forceUpdate();
}
