// Overlay.h

#ifndef OVERLAY_H

#define OVERLAY_H

#include  "Point.h"
#include "ViewInfo.h"
#include "Mode.h"
#include <QMouseEvent>
#include <QObject>
#include <QWidget>

class Overlay: public QObject {
  Q_OBJECT;
public:
  Overlay(QWidget *parent=0): QObject(parent), parentw(parent) {}
  virtual ~Overlay() {}
public:
  virtual void paint(class QPainter *, QRect const &,
		     ViewInfo const &);
  virtual bool mousePress(Point const &,
			  Qt::MouseButton, Qt::KeyboardModifiers,
			  int a);
  virtual bool mouseRelease(Point const &,
			    Qt::MouseButton, Qt::KeyboardModifiers,
			    int a);
  virtual bool mouseMove(Point const &,
			 Qt::MouseButtons, Qt::KeyboardModifiers,
			 int a);
  virtual bool keyPress(class QKeyEvent *);
  QWidget *parentWidget() const;
public slots:
  void setMode(Mode);
protected:
  Mode mode() const;
  void forceUpdate();
private:
  QWidget *parentw;
  Mode mode_;
};

#endif
