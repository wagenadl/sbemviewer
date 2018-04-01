// Overlay.cpp

#include "Overlay.h"

void Overlay::paint(class QPainter *, QRect const &, ViewInfo const &) {
}

bool Overlay::mousePress(Point const &, Qt::MouseButton,
			 Qt::KeyboardModifiers, int) {
  return false;
}

bool Overlay::mouseMove(Point const &, Qt::MouseButtons,
			Qt::KeyboardModifiers, int) {
  return false;
}

bool Overlay::mouseRelease(Point const &, Qt::MouseButton,
			   Qt::KeyboardModifiers, int) {
  return false;
}

bool Overlay::keyPress(class QKeyEvent *) {
  return false;
}

QWidget *Overlay::parentWidget() const {
  return parentw;
}

void Overlay::forceUpdate() {
  if (parentw)
    parentw->update();
}

Mode Overlay::mode() const {
  return mode_;
}

void Overlay::setMode(Mode m) {
  mode_ = m;
}
