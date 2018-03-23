// EditOverlay.h

#ifndef EDITOVERLAY_H

#define EDITOVERLAY_H

#include "Overlay.h"
#include "SBEMDB.h"

class EditOverlay: public Overlay {
  Q_OBJECT;
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
public slots:
  void setActiveTree(quint64);
private:
  SBEMDB *db;
  QWidget *parent;
  Point presspt;
  quint64 tid;
};

#endif
