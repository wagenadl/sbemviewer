// TreeSearchDialog.cpp

#include "TreeSearchDialog.h"

#include "ui_TreeSearchDialog.h"

#include <QDebug>

class TSD_Data {
public:
  TSD_Data(TreeSearchDialog *w, SBEMDB *db, TreeModel *tm):
    w(w), db(db), tm(tm) {
    ui = new Ui_TreeSearchDialog;
  }
  void clear();
  void close();
  void help();
  void applyIntersection();
  void applyUnion();
  void applyExclusion();
  void showExisting();
  void showThis();
  void showAll();
  void load();
  void store();
  void updateSearch();
  QString unionWhere() const;
  QString intersectionWhere() const;
  QString exclusionWhere() const;
  int count(QString where) const;
public:
  TreeSearchDialog *w;
  SBEMDB *db;
  TreeModel *tm;
  QString existingWhere;
  QString thisWhere;
};

void TSD_Data::clear() {
}

void TSD_Data::close() {
}

void TSD_Data::help() {
}

void TSD_Data::applyIntersection() {
}

void TSD_Data::applyUnion() {
}

void TSD_Data::applyExclusion() {
}

void TSD_Data::showExisting() {
}

void TSD_Data::showThis() {
}

void TSD_Data::showAll() {
}

void TSD_Data::load() {
}

void TSD_Data::store() {
}

void TSD_Data::updateSearch() {
}

QString TSD_Data::unionWhere() const {
}

QString TSD_Data::intersectionWhere() const {
}

QString TSD_Data::exclusionWhere() const {
}

int TSD_Data::count(QString where) const {
}

