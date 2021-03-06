// MovieRender.cpp

#include "MovieRender.h"
#include "SomaLabel.h"
#include "DistinctColors.h"
#include <QImage>
#include <QMap>
#include <QList>
#include <QTime>
#include <QPainter>
#include <QRegularExpression>
#include <cmath>

static QString nicelen(double um) {
  QString num = QString("%1").arg(um, 0, 'f', 1);
  int L = num.length();
  if (L>5)
    num = num.left(L-5) + "," + num.right(5);
  return num + " μm";
}

struct MRD_Segment {
  MRD_Segment(quint64 from=0, quint64 to=0): from(from), to(to) { }
  quint64 from, to; // indices into nodes
};

struct MRD_Tree {
public:
  QMap<quint64, PointF> nodes;
  QVector<quint64> presyn, postsyn, soma; // index into nodes
  QVector<QVector<MRD_Segment>> segments; // in build-up order
  QString somalabel;
  PointF color;
};

class MR_Data {
public:
  MR_Data(SBEMDB const *db, SomaLabel const *sm): db(db), sm(sm), info(db->serverInfo()) {
    dx = info.real("dx");
    dy = info.real("dy");
    dz = info.real("dz");
    setSomaLabels();
  }
  void reset(); // trees are scanned on request
  void setSomaLabels(); // really ugly hack!
  QImage render(int);
  int keyBuildupLength();
  int partnerBuildupLength();
  MRD_Tree const &tree(quint64 tid, quint64 fromnid=0); // 0 means from soma
  QMap<quint64, QVector<quint64>> presynapticPartners(quint64 tid);
  // return is map of partner tid to list of presynaptic nodes in that partner
  // by construction, the vector is not empty.
  QMap<quint64, QVector<quint64>> postsynapticPartners(quint64 tid);
  // return is map of partner tid to list of postsynaptic nodes in that tree
  // by construction, the vector is not empty.
public:
  SBEMDB const *db;
  SomaLabel const *sm;
  ServerInfo info;
  MMSettings s;
public:
  double dx, dy, dz;
private:
  void scanPartners(quint64 tid, bool prenotpost);
private:
  QMap<quint64, MRD_Tree> trees; // tid to tree
  QMap<quint64, QMap<quint64, QVector<quint64>>> prepartners; // tid to map
  // ... as from presynapticPartners.
  QMap<quint64, QMap<quint64, QVector<quint64>>> postpartners; // tid to map
  // ... as from postsynapticPartners.
  QMap<quint64, QString> soma; // soma labels map from tid
};

void MR_Data::setSomaLabels() {
  // what a hack!
  soma.clear();
  if (sm) {
    QSqlQuery q{db->constQuery("select tid, tname from trees")};
    while (q.next())
      soma[q.value(0).toULongLong()] = sm->lookup(q.value(1).toString());
  }
}

void MR_Data::reset() {
  trees.clear();
  prepartners.clear();
  postpartners.clear();
}

void MR_Data::scanPartners(quint64 tid, bool prenotpost) {
  QMap<quint64, QVector<quint64>> map;
  SBEMDB::NodeType typ(prenotpost ? SBEMDB::NodeType::PresynTerm
		       : SBEMDB::NodeType::PostsynTerm);
  QSqlQuery q = db->constQuery("select n.nid, n.tid from syncons as sc"
			       " inner join nodes as n on sc.nid==n.nid"
			       " inner join synapses as s on sc.sid==s.sid"
			       " inner join syncons as sc2 on sc2.sid==s.sid"
			       " inner join nodes as n2 on sc2.nid==n2.nid"
			       " where n2.tid=:a and n.typ==:b",
			       tid, int(typ));
  while (q.next())
    map[q.value(1).toULongLong()] << q.value(0).toULongLong();
  map.remove(tid);
  if (prenotpost)
    prepartners[tid] = map;
  else
    postpartners[tid] = map;
}

QMap<quint64, QVector<quint64>> MR_Data::presynapticPartners(quint64 tid) {
  if (!prepartners.contains(tid))
    scanPartners(tid, true);
  return prepartners[tid];
}

QMap<quint64, QVector<quint64>> MR_Data::postsynapticPartners(quint64 tid) {
  if (!postpartners.contains(tid))
    scanPartners(tid, false);
  return postpartners[tid];
}


MRD_Tree const &MR_Data::tree(quint64 tid, quint64 fromnid) {
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
  if (tree.nodes.isEmpty()) {
    trees[tid] = tree;
    return trees[tid];
  }

 
  q = db->constQuery("select n1.nid, n2.nid"
		     " from nodecons as nc"
		     " inner join nodes as n1 on n1.nid==nc.nid1"
		     " inner join nodes as n2 on n2.nid==nc.nid2"
		     " where n1.tid==:a and n1.nid<n2.nid", tid);
  if (s.buildup) {
    if (fromnid==0) {
      if (tree.soma.isEmpty())
	fromnid = tree.nodes.begin().key();
      else
	fromnid = tree.soma.first(); // there ought to be precisely one
    }
    QMap<quint64, QVector<quint64>> segmap; // bidirectional
    while (q.next()) {
      quint64 nid1 = q.value(0).toULongLong();
      quint64 nid2 = q.value(1).toULongLong();
      segmap[nid1] << nid2;
      segmap[nid2] << nid1;
    }
    QSet<quint64> frontier;
    QSet<quint64> interior;
    frontier << fromnid;
    while (!frontier.isEmpty()) {
      QVector<MRD_Segment> outreach;
      for (quint64 from: frontier) {
	for (quint64 to: segmap[from])
	  if (!interior.contains(to))
	    outreach << MRD_Segment(from, to);
	interior << from;
      }
      tree.segments << outreach;
      frontier.clear();
      for (MRD_Segment const &seg: outreach)
	frontier << seg.to;
    }
  } else {
    QVector<MRD_Segment> seg;
    while (q.next()) 
      seg << MRD_Segment(q.value(0).toULongLong(), q.value(1).toULongLong());
    tree.segments << seg;
  }

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
  int somaid; // tid, actually
};

double MR_Object::totalLength(QList<MR_Object> const &lst) {
  double l = 0;
  for (MR_Object const &obj: lst) 
    if (obj.isline)
      l += std::sqrt((obj.p1 - obj.p2).L2());
  return l;
}

int MR_Data::keyBuildupLength() {
  if (!s.buildup)
    return 0;
  int maxlen = 0;
  for (quint64 tid: s.keyTrees) {
    MRD_Tree const &t{tree(tid)};
    if (t.segments.size() > maxlen)
      maxlen = t.segments.size();
  }
  return maxlen;
}

int MR_Data::partnerBuildupLength() {
  if (!s.buildup)
    return 0;
  int maxlen = 0;
  for (quint64 tid: s.keyTrees) {
    if (s.alsoPresynaptic) {
      auto const &map = presynapticPartners(tid);
      for (auto it=map.begin(); it!=map.end(); it++) {
	quint64 t1 = it.key();
	QVector<quint64> const &nids = it.value();
	MRD_Tree const &t{tree(t1, nids.first())};
	if (t.segments.size() > maxlen)
	  maxlen = t.segments.size();
      }
    }
    if (s.alsoPostsynaptic) {
      auto const &map = postsynapticPartners(tid);
      for (auto it=map.begin(); it!=map.end(); it++) {
	quint64 t1 = it.key();
	QVector<quint64> const &nids = it.value();
	MRD_Tree const &t{tree(t1, nids.first())};
	if (t.segments.size() > maxlen)
	  maxlen = t.segments.size();
      }
    }
  }
  return maxlen;
}

QImage MR_Data::render(int n) {
  QImage img(s.resolution, QImage::Format_ARGB32_Premultiplied);
  img.fill(QColor(0,0,0));
  QList<MR_Object> objs;
  QList<MR_Object> allobjs;
  double l0 = 0;
  int keylen = keyBuildupLength();
  int partnerlen = partnerBuildupLength();

  QMap<quint64, double> objlen;
  double prelen=0, postlen=0;
  
  for (quint64 tid: s.keyTrees) {
    PointF color(1, 1, 1);
    MRD_Tree const &t{tree(tid)};
    for (quint64 nid: t.soma) {
      objs << MR_Object(t.nodes[nid], color, s.somaDiameter);
      objs.last().somaid = tid;
    }
    int N = s.buildup ? (n>keylen ? keylen : n) : t.segments.size();
    Q_ASSERT(N<=t.segments.size());
    for (int n=0; n<N; n++) {
      auto const &seglist{t.segments[n]};
      for (auto const &seg: seglist) {
	objs << MR_Object(LineF(t.nodes[seg.from], t.nodes[seg.to]),
			  color, s.keyWidth);
	if (t.presyn.contains(seg.to) || t.postsyn.contains(seg.to))
	  objs << MR_Object(t.nodes[seg.to], color, s.synapseDiameter);
	  
      }
    }
    for (PointF const &p: t.nodes)
      allobjs << MR_Object(p, color, 1);
    double l1 = MR_Object::totalLength(objs);
    qDebug() << "Neurite length in object" << tid << "is" << l1 - l0;
    objlen[tid] = l1 - l0;
    l0 = l1;
  }

  if (s.alsoPresynaptic) {
    int ppcount = 0;
    QList<PointF> somapos;
    for (quint64 tid: s.keyTrees) {
      auto const &map = presynapticPartners(tid);
      for (auto it=map.begin(); it!=map.end(); it++) {
	quint64 t1 = it.key();
	QVector<quint64> const &nids = it.value();
	ppcount++;
	MRD_Tree const &t{tree(t1, nids.first())};
	int N = t.segments.size();
	if (s.buildup) {
	  if (n<=keylen)
	    N = 0;
	  else if (n <= keylen + N)
	    N = n - keylen;
	}
	for (int n=0; n<N; n++) {
	  auto const &seglist{t.segments[n]};
	  for (auto const &seg: seglist)
	    objs << MR_Object(LineF(t.nodes[seg.from], t.nodes[seg.to]),
			      t.color, s.keyWidth);
	}
	if (N == t.segments.size()) {
	  for (quint64 nid: t.soma) {
	    PointF p(t.nodes[nid]);
	    somapos << p;
	    objs << MR_Object(p, t.color, s.somaDiameter);
	    objs.last().somaid = t1;
	  }
	}
      }
    }
    qDebug() << "Found" << ppcount << "presynaptic partners";
    double l1 = MR_Object::totalLength(objs);
    qDebug() << "Neurite length in presynaptic partners is" << l1 - l0;
    prelen = l1 - l0;
    l0 = l1;
  }
  
  if (s.alsoPostsynaptic) {
    int ppcount = 0;
    for (quint64 tid: s.keyTrees) {
      auto const &map = postsynapticPartners(tid);
      for (auto it=map.begin(); it!=map.end(); it++) {
	quint64 t1 = it.key();
	QVector<quint64> const &nids = it.value();
	ppcount++;
	MRD_Tree const &t{tree(t1, nids.first())};
	for (auto const &seglist: t.segments)
	  for (auto const &seg: seglist)
	    objs << MR_Object(LineF(t.nodes[seg.from], t.nodes[seg.to]),
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
    postlen = l1 - l0;
    l0 = l1;
  }

  PointF p0(0,0,0);
  for (MR_Object const &obj: allobjs) {
    p0 += obj.p1;
  }
  p0 /= allobjs.size();
  double L2 = 0;
  for (MR_Object const &obj: allobjs) {
    double L2a = (obj.p1-p0).L2();
    if (L2a > L2)
      L2 = L2a;
  }

  Transform3 xf;
  xf.shift(s.resolution.width()/2, s.resolution.height()/2, 0);
  xf.flipy();
  xf.scale(s.resolution.width()/std::sqrt(L2)/2, 0, 0);

  QPointF hundredum = (xf.apply(PointF(100,0,0)) - xf.apply(PointF(0,0,0)))
    .toQPointF();

  if (n>keylen + partnerlen)
    xf.rotate((n-keylen-partnerlen) * 2 * 3.141592 / s.frameCount, 0, 0, 0);
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

  if (s.fontsize>0) {
    double fs = s.fontsize;
    QPointF right = QPointF(s.resolution.width(), s.resolution.height())
      - QPointF(20, 20 + fs*1.5);
    QPointF left = right - hundredum;
    ptr.setPen(QPen(QColor(255,255,64), s.keyWidth));
    ptr.drawLine(QLineF(left, right));
    QFont f(ptr.font());
    f.setPixelSize(fs);
    ptr.setFont(f);
    ptr.drawText(QRectF(left,right + QPointF(0, fs*1.5)),
                 Qt::AlignHCenter | Qt::AlignBottom, "100 μm");

    ptr.setPen(QPen(QColor(255,255,255), s.keyWidth));

    QPointF l = left - QPointF(0, 4*s.somaDiameter);
    QPointF c = l + QPointF(s.somaDiameter, 0);
    QPointF r = right - QPointF(0, 4*s.somaDiameter);
    QPointF m = l + (r-l)*.4;
    if (objlen.contains(444)) {
      ptr.drawLine(c, m);
      ptr.drawText(QRectF(m + QPointF(fs, -fs),
                          r + QPointF(0, fs)),
                   Qt::AlignLeft | Qt::AlignVCenter, "DE-3R");
      double len = objlen[444];
      if (len > 0) {
        ptr.drawText(QRectF(c + QPointF(0,fs),
                            c + (r-c) + QPointF(0,2.2*fs)),
                     Qt::AlignRight | Qt::AlignVCenter,
                     nicelen(len));
      }
      if (len>1000) {
        QPointF l = left - QPointF(0, 6.5*s.somaDiameter);
        QPointF r = right - QPointF(0, 6.5*s.somaDiameter);
        QPointF m = l + (r-l)*.4;
        ptr.drawLine(l, m);
        ptr.drawText(QRectF(m + QPointF(fs, -fs*1.5),
                            r + QPointF(0, fs*1.5)),
                     Qt::AlignLeft | Qt::AlignVCenter, "Input\nsynapses");
      }
    }
    if (prelen>0) {
      ptr.setPen(QPen(QColor(128,192,255), s.lineWidth));
      QPointF l = left - QPointF(0, 10.5*s.somaDiameter);
      QPointF c = l + QPointF(s.somaDiameter, 0);
      QPointF r = right - QPointF(0, 10.5*s.somaDiameter);
      QPointF m = l + (r-l)*.4;
      ptr.drawLine(c, m);
      ptr.drawText(QRectF(m + QPointF(fs, -fs),
                          r + QPointF(0,fs)),
                   Qt::AlignLeft | Qt::AlignVCenter, "Partners");
      ptr.drawText(QRectF(l + QPointF(0, fs),
                          r + QPointF(0, 2.2*fs)),
                   Qt::AlignRight | Qt::AlignVCenter,
                   nicelen(prelen));
      ptr.setPen(QPen(Qt::NoPen));
      ptr.setBrush(QColor(128,192,255));
      ptr.drawEllipse(c, s.somaDiameter, s.somaDiameter);
    }
    if (objlen.contains(444)) {
      ptr.setPen(QPen(Qt::NoPen));
      ptr.setBrush(QColor(255,255,255));
      ptr.drawEllipse(c, s.somaDiameter, s.somaDiameter);
      double len = objlen[444];
      if (len>1000) {
        QPointF c = (l+m)/2 + QPointF(0, -2.5*fs);
        ptr.drawEllipse(c, s.synapseDiameter, s.synapseDiameter);
      }
    }
  }
  
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

MovieRender::MovieRender(SBEMDB const *db, SomaLabel const *sm, QObject *parent):
  QObject(parent), d(new MR_Data(db, sm)) {
}

MovieRender::~MovieRender() {
  delete d;
}

void MovieRender::rereadDatabase() {
  d->reset();
  d->setSomaLabels();
}

void MovieRender::setSettings(MMSettings const &s) {
  d->s = s;
}

QImage MovieRender::render(int n) {
  return d->render(n);
}

int MovieRender::buildupFrameCount() {
  qDebug() << "buildupframecount"
	   <<  d->keyBuildupLength() << d->partnerBuildupLength();
  return d->keyBuildupLength() + d->partnerBuildupLength();
}
