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
  void setLines(int tid, QVector<LineF>);
  void setPoints(int tid, QVector<PointF>);
  void setColor(int tid, PointF near); // rgb [0,1]
  void setPointSize(int tid, float r);
  void freeze();
  void thaw();
  bool frozen() const;
  PointF color(int tid) const; // rgb [0,1]
public slots:
  void setXAxisLabels(QString neg, QString pos);
  void setYAxisLabels(QString neg, QString pos);
  void setZAxisLabels(QString neg, QString pos);
protected:
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void keyPressEvent(QKeyEvent *) override;
  void paintEvent(QPaintEvent *) override;
  void resizeEvent(QResizeEvent *) override;
private:
  double tform[3][3];
  QPoint presspt;
  QMap< int, QVector<LineF> > lines; // in um
  QMap< int, QVector<PointF> > points; // in um
  QMap< int, PointF > nearColor;
  QMap< int, float > pointSize;
  QString xneg, xpos, yneg, ypos, zneg, zpos;
  int freezedepth;
};

#endif
