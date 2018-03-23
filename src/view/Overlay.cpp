// Overlay.cpp

#include "Overlay.h"

void Overlay::paint(class QPainter *, QRect const &, ViewInfo const &) {
}

bool Overlay::mousePress(Point const &, Qt::MouseButton,
			 Qt::KeyboardModifiers, int) {
  return false;
}

bool Overlay::mouseMove(Point const &, Qt::MouseButton,
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
