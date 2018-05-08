// ProjectionView.h

#ifndef PROJECTIONVIEW_H

#define PROJECTIONVIEW_H

#include <QWidget>
#include <QMap>
#include <QVector>
#include "LineF.h"

class ProjectionView: public QWidget {
  Q_OBJECT;
public:
  ProjectionView(QWidget *parent=0);
  virtual ~ProjectionView();
  void setTree(int tid, QVector<LineF>);
  void setColor(int tid, QColor);
public slots:
  void setYRotation(double); // in degrees
  void setXRotation(double);
protected:
  void paintEvent(QPaintEvent *) override;
  void resizeEvent(QResizeEvent *) override;
private:
  double phix, phiy; // in radians
  QMap< int, QVector<LineF> > trees; // in um
  QMap< int, QColor > colors;
};

#endif
