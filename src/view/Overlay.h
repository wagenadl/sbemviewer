// Overlay.h

#ifndef OVERLAY_H

#define OVERLAY_H

#include  "Point.h"
#include "ViewInfo.h"
#include <QMouseEvent>

class Overlay {
public:
  virtual void paint(class QPainter *, QRect const &,
		     ViewInfo const &);
  virtual bool mousePress(Point const &, Qt::MouseButton,
			  Qt::KeyboardModifiers);
  virtual bool mouseMove(Point const &, Qt::MouseButton,
			  Qt::KeyboardModifiers);
  virtual bool mouseRelease(Point const &, Qt::MouseButton,
			    Qt::KeyboardModifiers);
  virtual bool keyPress(class QKeyEvent *);
};

#endif
