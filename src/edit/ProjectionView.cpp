// ProjectionView.cpp

#include "ProjectionView.h"

ProjectionView::ProjectionView(QWidget *parent): QWidget(parent) {
  phix = 0;
  phiy = 0;
}

ProjectionView::~ProjectionView() {
}

void ProjectionView::setTree(int tid, QVector<LineF> l) {
  trees[tid] = l;
  update();
}

void ProjectionView::setColor(int tid, QColor c) {
  colors[tid] = c;
  update();
}     

void ProjectionView::setYRotation(double p) {
  phiy = p*3.141592/180;
  update();
}

void ProjectionView::setXRotation(double p) {
  phix = p*3.141592/180;
  update();
}

void ProjectionView::paintEvent(QPaintEvent *) {
  // fun...
}

void ProjectionView::resizeEvent(QResizeEvent *) {
  update();
}

