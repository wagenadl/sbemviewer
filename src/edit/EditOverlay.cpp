// EditOverlay.cpp

#include "EditOverlay.h"
#include <QDebug>

EditOverlay::EditOverlay(SBEMDB *db, QWidget *parent):
  Overlay(parent), db(db), parent(parent) {
  presspt = Point(-1000, -1000, -1000); // i.e., not real
  tid = 0; // i.e., none
}

EditOverlay::~EditOverlay() {
}

void EditOverlay::paint(QPainter *p,
			QRect const &, class ViewInfo const &vi) {
  qDebug() << "EditOverlay::paint" << p << vi.x0 << vi.y0 << vi.w << vi.h
	   << vi.z << vi.a;
}

bool EditOverlay::mousePress(Point const &p,
			     Qt::MouseButton b, Qt::KeyboardModifiers m,
			     int a) {
  qDebug() << "EditOverlay::press" << p << b << m << a;
  presspt = p;
  return false;
}

bool EditOverlay::mouseRelease(Point const &p,
			       Qt::MouseButton b, Qt::KeyboardModifiers m,
			       int a) {
  qDebug() << "EditOverlay::release" << p << b << m << a;
  presspt = Point(-1000, -1000, -1000);
  return false;
}

bool EditOverlay::mouseMove(Point const &p,
			    Qt::MouseButton b, Qt::KeyboardModifiers m,
			    int a) {
  qDebug() << "EditOverlay::move" << p << b << m << a;
  return false;
}

void EditOverlay::setActiveTree(quint64 tid1) {
  tid = tid1;
  if  (parent)
    parent->update();
}
