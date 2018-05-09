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
  void setColors(int tid, QColor near, QColor far);
public slots:
  void setYRotation(int); // in degrees
  void setXRotation(int);
  void setXAxisLabels(QString neg, QString pos);
  void setYAxisLabels(QString neg, QString pos);
  void setZAxisLabels(QString neg, QString pos);
protected:
  void paintEvent(QPaintEvent *) override;
  void resizeEvent(QResizeEvent *) override;
private:
  double phix, phiy; // in radians
  QMap< int, QVector<LineF> > trees; // in um
  QMap< int, QColor > nearColor;
  QMap< int, QColor > farColor;
  QString xneg, xpos, yneg, ypos, zneg, zpos;
};

#endif
