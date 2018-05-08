// ProjectionWidget.cpp

#include "ProjectionWidget.h"
#include "ui_ProjectionWidget.h"

class ProjectionData {
public:
  ProjectionData(SBEMDB *db): db(db) {
    ui = new Ui_ProjectionWidget;
    haveSelectedTree = false;
  }
  ~ProjectionData() {
    delete ui;
  }
public:
  SBEMDB *db;
  Ui_ProjectionWidget *ui;
  bool haveSelectedTree;
};
  
ProjectionWidget::ProjectionWidget(SBEMDB *db, QWidget *parent):
  QWidget(parent), d(new ProjectionData(db)) {
  d->ui->setupUi(this);
}

ProjectionWidget::~ProjectionWidget() {
  delete d;
}

void ProjectionWidget::addTree(int tid) {
  bool isSelected = tid==db->selectedTree();
  if (isSelected)
    d->haveSelectedTree = true;
}

void ProjectionWidget::addVisibleTrees() {
}

void ProjectionWidget::addSelectedTree() {
}

