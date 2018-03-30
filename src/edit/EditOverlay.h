// EditOverlay.h

#ifndef EDITOVERLAY_H

#define EDITOVERLAY_H

#include "Overlay.h"
#include "SBEMDB.h"

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
		 Qt::MouseButton, Qt::KeyboardModifiers,
		 int a) override;
  static int nodeScreenRadius(int a); // how large should a node look on screen
                                      // at a given display scale
  static int nodeSBEMRadius(int a);
public slots:
  void setActiveTree(quint64); // resets active node to null
  void setActiveNode(quint64); // if non-null, also sets active tree
private:
  SBEMDB *db;
  QWidget *parent;
  Point presspt;
  quint64 tid; // active tree
  quint64 nid; // selected node
};

#endif
