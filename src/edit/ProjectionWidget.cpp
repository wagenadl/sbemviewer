// ProjectionWidget.cpp

#include "ProjectionWidget.h"
#include "ui_ProjectionWidget.h"

class ProjectionData {
public:
  ProjectionData(SBEMDB *db): db(db) {
    ui = new Ui_ProjectionWidget;
  }
  ~ProjectionData() {
    delete ui;
  }
public:
  SBEMDB *db;
  Ui_ProjectionWidget *ui;
};
  
ProjectionWidget::ProjectionWidget(SBEMDB *db, QWidget *parent): QWidget(parent),
                                                     d(new ProjectionData(db)) {
  d->ui->setupUi(this);
}

ProjectionWidget::~ProjectionWidget() {
  delete d;
}

void ProjectionWidget::addTree(int tid) {
}

void ProjectionWidget::addVisibleTrees() {
}

void ProjectionWidget::addSelectedTree() {
}

