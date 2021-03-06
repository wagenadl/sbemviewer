// ProjectionView.h

#ifndef PROJECTIONVIEW_H

#define PROJECTIONVIEW_H

#include <QWidget>
#include <QMap>
#include <QVector>
#include "LineF.h"
#include "Transform3.h"

class ProjectionView: public QWidget {
  Q_OBJECT;
public:
  ProjectionView(QWidget *parent=0);
  virtual ~ProjectionView();
  void setLines(int tid, QVector<LineF>);
  void setPoints(int tid, QVector<PointF>);
  void setColor(int tid, PointF near); // rgb [0,1]
  void setPointSize(int tid, float r);
  void setName(int tid, QString tname);
  void togglePointGroup(int tidgroup);
  void freeze();
  void thaw();
  bool frozen() const;
  PointF color(int tid) const; // rgb [0,1]
  void clear();
public slots:
  void setXAxisLabels(QString neg, QString pos);
  void setYAxisLabels(QString neg, QString pos);
  void setZAxisLabels(QString neg, QString pos);
signals:
  void hoveringOnTree(int tid);
  void doubleClickOnTree(int tid, PointF pos);
protected:
  void wheelEvent(QWheelEvent *) override;
  void mousePressEvent(QMouseEvent *) override;
  void mouseDoubleClickEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void keyPressEvent(QKeyEvent *) override;
  void paintEvent(QPaintEvent *) override;
  void resizeEvent(QResizeEvent *) override;
private:
  class ProjectionViewData *d;
};

#endif
