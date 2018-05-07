// EditOverlay.h

#ifndef EDITOVERLAY_H

#define EDITOVERLAY_H

#include "Overlay.h"
#include "SBEMDB.h"
#include "Mode.h"

class EditOverlay: public Overlay {
  Q_OBJECT;
public:
  static constexpr int ZTOLERANCE = 10; // number of Z layers to show ± current
public:
  EditOverlay(SBEMDB *db, QWidget *parent=0);
  ~EditOverlay();
  void paint(QPainter *, QRect const &, class ViewInfo const &) override;
  bool mousePress(Point const &,
		  Qt::MouseButton, Qt::KeyboardModifiers,
		  int a) override;
  bool mouseRelease(Point const &,
		    Qt::MouseButton, Qt::KeyboardModifiers,
		    int a) override;
  bool mouseMove(Point const &,
		 Qt::MouseButtons, Qt::KeyboardModifiers,
		 int a) override;
  bool keyPress(QKeyEvent *) override;
  static int nodeScreenRadius(int a); // how large should a node look on screen
                                      // at a given display scale
  static int nodeSBEMRadius(int a);
  bool isTreeVisible(quint64 tid1, ViewInfo const &vi) const;
  Point goodSpotForTree(quint64 tid1, ViewInfo const &vi, bool *ok=0) const;
public slots:
  void setActiveTree(quint64); // resets active node to null
  void setActiveNode(quint64); // if non-null, also sets active tree
  void actEditMemo();
  void actSetNodeType(SBEMDB::NodeType);
  void actDeleteNode();
  void actDisconnectNodes();
  void actConnectNodes();
  void actConnectTerminals();
  void actDissolveSynapse();
  void actCenterNode();
signals:
  void otherTreePressed(quint64 tid, quint64 nid);
  void treeTableAltered();
  void gotoNodeRequest(Point p);
private:
  void drawActiveTree(QPainter *p, ViewInfo const &vi);
  void drawOtherTrees(QPainter *p, ViewInfo const &vi);
  void drawAuxNid(QPainter *p, ViewInfo const &vi);
  void drawSynapses(QPainter *p, ViewInfo const &vi);
  bool plainLeftPress(Point const &p, int a);
  void drawCons(QPainter *p, ViewInfo const &vi,
                QVector<SBEMDB::NodeCon> const &cons,
                QColor (*colorfn)(int), bool thin=false);
  void drawNodes(QPainter *p, ViewInfo const &vi,
                QVector<SBEMDB::Node> const &nodes,
                QColor (*colorfn)(int));
  void drawTags(QPainter *p, ViewInfo const &vi);
private:
  SBEMDB *db;
  QWidget *parent;
  Point presspt;
  Point origpt;
  quint64 tid; // active tree
  quint64 nid; // selected node
  quint64 aux_nid; // shift-clicked node
};

#endif
