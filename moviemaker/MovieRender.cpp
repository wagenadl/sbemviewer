// MovieRender.cpp

#include "MovieRender.h"
#include "DistinctColors.h"
#include <QImage>
#include <QMap>
#include <QList>
#include <QTime>
#include <QPainter>
#include <QRegularExpression>
#include <cmath>

static void applyGapShift(PointF &p) {
  if (p.z>=328.7) {
    p.x -= 4.285;
    p.y += 25.857;
  }
}

struct MRD_Tree {
public:
  QMap<quint64, PointF> nodes;
  QVector<quint64> presyn, postsyn, soma; // index into nodes
  QVector<QPair<quint64,quint64>> segments; // indices into nodes
  QString somalabel;
  PointF color;
};


class MR_Data {
public:
  MR_Data(SBEMDB const *db): db(db), info(db->serverInfo()) {
    dx = info.real("dx");
    dy = info.real("dy");
    dz = info.real("dz");
    setSomaLabels();
  }
  void reset(); // trees are scanned on request
  void setSomaLabels(); // really ugly hack!
  QImage render(int);
  MRD_Tree const &tree(quint64 tid);
  QList<quint64> presynapticPartners(quint64 tid);
  QList<quint64> postsynapticPartners(quint64 tid);
public:
  SBEMDB const *db;
  ServerInfo info;
  MMSettings s;
public:
  double dx, dy, dz;
private:
  void scanPartners(quint64 tid, bool prenotpost);
private:
  QMap<quint64, MRD_Tree> trees; // tid to tree
  QMap<quint64, QList<quint64>> prepartners; // tid to tids
  QMap<quint64, QList<quint64>> postpartners; // tid to tids
  QMap<int, QString> soma; // soma labels
};

void MR_Data::setSomaLabels() {
  // what a hack!
  #include "somalabel.h"
}

void MR_Data::reset() {
  trees.clear();
  prepartners.clear();
  postpartners.clear();
}

void MR_Data::scanPartners(quint64 tid, bool prenotpost) {
  QList<quint64> tids;
  SBEMDB::NodeType typ(prenotpost ? SBEMDB::NodeType::PresynTerm
		       : SBEMDB::NodeType::PostsynTerm);
  QSqlQuery q = db->constQuery("select n.tid from syncons as sc"
			       " inner join nodes as n on sc.nid==n.nid"
			       " inner join synapses as s on sc.sid==s.sid"
			       " inner join syncons as sc2 on sc2.sid==s.sid"
			       " inner join nodes as n2 on sc2.nid==n2.nid"
			       " where n2.tid=:a and n.typ==:b group by n.tid",
			       tid, int(typ));
  while (q.next())
    tids << q.value(0).toULongLong();
  tids.removeAll(tid);
  if (prenotpost)
    prepartners[tid] = tids;
  else
    postpartners[tid] = tids;
}

QList<quint64> MR_Data::presynapticPartners(quint64 tid) {
  if (!prepartners.contains(tid))
    scanPartners(tid, true);
  return prepartners[tid];
}

QList<quint64> MR_Data::postsynapticPartners(quint64 tid) {
  if (!postpartners.contains(tid))
    scanPartners(tid, false);
  return postpartners[tid];
}


MRD_Tree const &MR_Data::tree(quint64 tid) {
  if (trees.contains(tid))
    return trees[tid];
  
  MRD_Tree tree;
  QSqlQuery q = db->constQuery("select nid, x, y, z, typ from nodes"
			       " where tid==:a", tid);
  while (q.next()) {
    quint64 nid = q.value(0).toULongLong();
    PointF p(q.value(1).toInt()*dx,
	     q.value(2).toInt()*dy,
	     q.value(3).toInt()*dz);
    applyGapShift(p);
    tree.nodes[nid] = p;
    switch (SBEMDB::NodeType(q.value(4).toInt())) {
    case SBEMDB::PresynTerm:
      tree.presyn << nid;
      break;
    case SBEMDB::PostsynTerm:
      tree.postsyn << nid;
      break;
    case SBEMDB::Soma:
      tree.soma << nid;
    default:
      break;
    }
  }
						  
  q = db->constQuery("select n1.nid, n2.nid"
                               " from nodecons as nc"
                               " inner join nodes as n1 on n1.nid==nc.nid1"
                               " inner join nodes as n2 on n2.nid==nc.nid2"
                               " where n1.tid==:a and n1.nid<n2.nid", tid);
  while (q.next()) 
    tree.segments << QPair<quint64, quint64>(q.value(0).toULongLong(),
					     q.value(1).toULongLong());

  q = db->constQuery("select tname from trees where tid==:a", tid);
  QString tname = q.next() ? q.value(0).toString() : "";

  tree.somalabel = tname;
    
  int uctid = 0;
  auto mtch = QRegularExpression("^(\\d+)[^\\.]").match(tname);
  if (mtch.hasMatch()) 
    uctid = mtch.captured(1).toInt();
  if (uctid==0) {
    auto mtch = QRegularExpression("1\\.pre[sp]\\.p\\.(\\d+)").match(tname);
    if (mtch.hasMatch())
      uctid = mtch.captured(1).toInt() + 400;
  }
  uint32_t c = DistinctColors::instance().color(uctid);
  tree.color = PointF((c&0xff0000)/65536/255.*1.5-.5,
		      (c&0x00ff00)/256/255.*1.5-.5,
		      (c&0x0000ff)/1/255.*1.5-.5);
  
  trees[tid] = tree;
  return trees[tid];
}

class MR_Object {
public:
  MR_Object() { radius=0; isline = 0; }
  MR_Object(PointF p, PointF color, double r):
    p1(p), color(color), radius(r), isline(false) { somaid = 0; }
  MR_Object(LineF l, PointF color, double r):
    p1(l.p1), p2(l.p2), color(color), radius(r), isline(true) { somaid = 0; }
  double z() const {
    return isline ? (p1.z + p2.z)/2 : p1.z;
  }
  static double totalLength(QList<MR_Object> const &lst);
public:
  PointF p1;
  PointF p2;
  PointF color;
  double radius;
  bool isline;
  int somaid;
};

double MR_Object::totalLength(QList<MR_Object> const &lst) {
  double l = 0;
  for (MR_Object const &obj: lst) 
    if (obj.isline)
      l += std::sqrt((obj.p1 - obj.p2).L2());
  return l;
}

QImage MR_Data::render(int n) {
  QImage img(s.resolution, QImage::Format_ARGB32_Premultiplied);
  img.fill(QColor(0,0,0));
  QList<MR_Object> objs;
  double l0 = 0;
  for (quint64 tid: s.keyTrees) {
    PointF color(1, 1, 1);
    MRD_Tree const &t{tree(tid)};
    for (quint64 nid: t.presyn)
      objs << MR_Object(t.nodes[nid], color, s.synapseDiameter);
    for (quint64 nid: t.postsyn)
      objs << MR_Object(t.nodes[nid], color, s.synapseDiameter);
    for (quint64 nid: t.soma) {
      objs << MR_Object(t.nodes[nid], color, s.somaDiameter);
      objs.last().somaid = tid;
    }
    for (auto seg: t.segments)
      objs << MR_Object(LineF(t.nodes[seg.first], t.nodes[seg.second]),
			color, s.keyWidth);
    double l1 = MR_Object::totalLength(objs);
    qDebug() << "Neurite length in object" << tid << "is" << l1 - l0;
    l0 = l1;
  }

  if (s.alsoPresynaptic) {
    int ppcount = 0;
    QList<PointF> somapos;
    for (quint64 tid: s.keyTrees) {
      for (quint64 t1: presynapticPartners(tid)) {
	ppcount++;
	MRD_Tree const &t{tree(t1)};
	for (auto seg: t.segments)
	  objs << MR_Object(LineF(t.nodes[seg.first], t.nodes[seg.second]),
			    t.color, s.keyWidth);
	for (quint64 nid: t.soma) {
	  PointF p(t.nodes[nid]);
	  somapos << p;
	  objs << MR_Object(p, t.color, s.somaDiameter);
	  objs.last().somaid = t1;
	}
      }
    }
    qDebug() << "Found" << ppcount << "presynaptic partners";
    double l1 = MR_Object::totalLength(objs);
    qDebug() << "Neurite length in presynaptic partners is" << l1 - l0;
    l0 = l1;
  }
  
  if (s.alsoPostsynaptic) {
    int ppcount = 0;
    for (quint64 tid: s.keyTrees) {
      for (quint64 t1: postsynapticPartners(tid)) {
	ppcount++;
	MRD_Tree const &t{tree(t1)};
	for (auto seg: t.segments)
	  objs << MR_Object(LineF(t.nodes[seg.first], t.nodes[seg.second]),
			    t.color, s.keyWidth);
	for (quint64 nid: t.soma) {
	  PointF p(t.nodes[nid]);
	  objs << MR_Object(p, t.color, s.somaDiameter);
	  objs.last().somaid = t1;
	}
      }
    }
    qDebug() << "Found" << ppcount << "postsynaptic partners";
    double l1 = MR_Object::totalLength(objs);
    qDebug() << "Neurite length in postsynaptic partners is" << l1 - l0;
    l0 = l1;
  }

  PointF p0(0,0,0);
  for (MR_Object const &obj: objs) {
    p0 += obj.p1;
  }
  p0 /= objs.size();
  double L2 = 0;
  for (MR_Object const &obj: objs) {
    double L2a = (obj.p1-p0).L2();
    if (L2a > L2)
      L2 = L2a;
  }

  Transform3 xf;
  xf.shift(s.resolution.width()/2, s.resolution.height()/2, 0);
  xf.flipy();
  xf.scale(s.resolution.width()/std::sqrt(L2)/2, 0, 0);
  xf.rotate(n * 2 * 3.141592 / s.frameCount, 0, 0, 0);
  xf.rotate(3.141592/180 * s.theta, 3.141592/180 * s.phi, 0, 0);
  xf.shift(-p0.x, -p0.y, -p0.z);
  for (MR_Object &obj: objs) {
    obj.p1 = xf.apply(obj.p1);
    if (obj.isline)
      obj.p2 = xf.apply(obj.p2);
  }

  std::sort(objs.begin(), objs.end(),
	    [](MR_Object const &a, MR_Object const &b) {
	      return a.z() < b.z();
	    });

  double Z = s.resolution.width();
  auto getcolor = [Z](PointF const &p, double z) {
    z = 1 + z/Z/2;
    float r = p.x*255;
    float g = p.y*255;
    float b = p.z*255;
    float sum = r+g+b;
    if (sum<100) {
      float delta = 100 - sum;
      r += delta;
      g += delta;
      b += delta;
    }
    r *= z;
    g *= z;
    b *= z;
    return QColor(r<0 ? 0 : r>255 ? 255 : r,
		  g<0 ? 0 : g>255 ? 255 : g,
		  b<0 ? 0 : b>255 ? 255 : b);
  };

  
  QPainter ptr(&img);
  for (MR_Object const &obj: objs) {
    if (obj.isline) {
      if (s.shadow>0) {
	ptr.setPen(QPen(QColor(0,0,0,s.shadow*2), obj.radius+2));
	ptr.drawLine(QLineF(QPointF(obj.p1.x, obj.p1.y),
			    QPointF(obj.p2.x, obj.p2.y)));
	ptr.setPen(QPen(QColor(0,0,0,s.shadow), obj.radius+4));
	ptr.drawLine(QLineF(QPointF(obj.p1.x, obj.p1.y),
			    QPointF(obj.p2.x, obj.p2.y)));
      }
      ptr.setPen(QPen(getcolor(obj.color, obj.z()), obj.radius));
      ptr.drawLine(QLineF(QPointF(obj.p1.x, obj.p1.y),
			  QPointF(obj.p2.x, obj.p2.y)));
    } else {
      ptr.setPen(QPen(Qt::NoPen));
      if (s.shadow>0) {
	ptr.setBrush(QBrush(QColor(0,0,0,s.shadow*2)));
	ptr.drawEllipse(QPointF(obj.p1.x, obj.p1.y),
			obj.radius + 2, obj.radius + 2);
	ptr.setBrush(QBrush(QColor(0,0,0,s.shadow)));
	ptr.drawEllipse(QPointF(obj.p1.x, obj.p1.y),
			obj.radius + 4, obj.radius + 4);
      }
      ptr.setBrush(QBrush(getcolor(obj.color, obj.p1.z)));
      ptr.drawEllipse(QPointF(obj.p1.x, obj.p1.y),
		      obj.radius, obj.radius);
      if (obj.somaid && soma.contains(obj.somaid)) {
	QString txt = soma[obj.somaid];
	txt.replace("_R", "");
	txt.replace("_L", "");
	if (!txt.isEmpty() && txt!="-") {
	  ptr.setPen(getcolor(obj.color, obj.p1.z));
	  ptr.drawText(QPointF(obj.p1.x + 2*obj.radius, obj.p1.y), txt);
	}
      }
    }
  }
  return img;
}

MovieRender::MovieRender(SBEMDB const *db, QObject *parent):
  QObject(parent), d(new MR_Data(db)) {
}

MovieRender::~MovieRender() {
  delete d;
}

void MovieRender::rereadDatabase() {
  d->reset();
}

void MovieRender::setSettings(MMSettings const &s) {
  d->s = s;
}

QImage MovieRender::render(int n) {
  return d->render(n);
}
