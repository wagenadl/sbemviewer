// EditOverlay.h

#ifndef EDITOVERLAY_H

#define EDITOVERLAY_H

#include "Overlay.h"
#include "SBEMDB.h"

class EditOverlay: public Overlay {
  Q_OBJECT;
public:
  EditOverlay(SBEMDB *db);
  ~EditOverlay();
  void paint(QPainter *, QRect const &, class ViewInfo const &) override;
  bool mousePress(Point, Qt::MouseButton, Qt::KeyboardModifiers) override;
  bool mouseRelease(Point, Qt::MouseButton, Qt::KeyboardModifiers) override;
  bool mouseMove(Point, Qt::MouseButton, Qt::KeyboardModifiers) override;
public slots:
  void setActiveTree(quint64);
private:
  SBEMDB *db;
  Point presspt;
  quint64 tid;
};

#endif
