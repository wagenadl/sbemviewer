// Overlay.h

#ifndef OVERLAY_H

#define OVERLAY_H

#include <QObject>

class Overlay: public QObject {
public:
  Overlay(QObject *parent=0): QObject(parent) {}
  virtual ~Overlay() {}
public:
  virtual void paint(class QPainter *);
  virtual bool mousePress(class QMouseEvent *);
  virtual bool mouseMove(class QMouseEvent *);
  virtual bool mouseRelease(class QMouseEvent *);
  virtual bool keyPress(class QMouseEvent *);
};

#endif
