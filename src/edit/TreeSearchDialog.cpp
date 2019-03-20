// TreeSearchDialog.cpp

#include "TreeSearchDialog.h"

#include "ui_TreeSearchDialog.h"

#include <QFileInfo>
#include <QDebug>
#include <sqlite3.h>

class TSD_Data {
public:
  TSD_Data(TreeSearchDialog *w, SBEMDB *db, TreeModel *tm):
    w(w), db(db), tm(tm) {
    ui = new Ui_TreeSearchDialog;
  }
  void import(); // from current
  void clear();
  void close();
  void help();
  void applyIntersection();
  void applyUnion();
  void applyExclusion();
  void showExisting();
  void showThis();
  void load();
  void store();
  void updateSearch();
  void updateExisting();
  QString unionWhere() const;
  QString intersectionWhere() const;
  QString exclusionWhere() const;
  int count(QString where) const;
public:
  Ui_TreeSearchDialog *ui;
  TreeSearchDialog *w;
  SBEMDB *db;
  TreeModel *tm;
  QString existingWhere;
  QString thisWhere;
};

void TSD_Data::import() {
  QStringList visids;
  QSqlQuery q(db->constQuery("select tid from trees where visible>0"));
  while (q.next()) 
    visids << QString::number(q.value(0).toInt());
  existingWhere = "tid in (" + visids.join(",") + ")";
  updateExisting();
  ui->searchTerm->setText("");
  updateSearch();
}

void TSD_Data::clear() {
  db->query("update trees set visible=0");
  tm->updateVisibilityFromDatabase();
  updateExisting();
  updateSearch();
}

void TSD_Data::close() {
  w->close();
}

void TSD_Data::help() {
  qDebug() << "testing regexp" << db->canDoRegExps();
}

void TSD_Data::applyUnion() {
  if (thisWhere=="")
    return;
  db->query("update trees set visible=1 where " + thisWhere);
  tm->updateVisibilityFromDatabase();
  ui->searchTerm->setText("");
  updateExisting();
}

void TSD_Data::applyIntersection() {
  if (thisWhere=="")
    return;
  db->query("update trees set visible=0 where not " + thisWhere);
  tm->updateVisibilityFromDatabase();
  ui->searchTerm->setText("");
  updateExisting();
}

void TSD_Data::applyExclusion() {
  if (thisWhere=="")
    return;
  db->query("update trees set visible=0 where " + thisWhere);
  tm->updateVisibilityFromDatabase();
  ui->searchTerm->setText("");
  updateExisting();
}

void TSD_Data::showExisting() {
  updateExisting();
}

void TSD_Data::showThis() {
  updateSearch();
}

void TSD_Data::load() {
}

void TSD_Data::store() {
}

void TSD_Data::updateSearch() {
  QString term = ui->searchTerm->text();
  qDebug() << "updatsearch" << term;
  if (term.isEmpty()) {
    ui->thisCount->setText("");
    ui->intersectionCount->setText("");
    ui->unionCount->setText("");
    ui->exclusionCount->setText("");
    ui->applyIntersection->setEnabled(false);
    ui->applyUnion->setEnabled(false);
    ui->applyExclusion->setEnabled(false);
    ui->thisNames->setText("");
  } else {
    constexpr bool REGEXP = false;
    if (REGEXP) {
      term.replace("\"", ".");
      bool atStart = term.startsWith("^");
      bool atEnd = term.endsWith("$");
      bool endB = !term.endsWith(".");
      bool startB = !term.startsWith(".");
      if (atStart)
	term = term.mid(1);
      if (atEnd)
	term = term.left(term.size()-1);
      term.replace(" ", "\\b\\s*\\b");
      term.replace("*", "×");
      term.replace(".", "[^A-Za-z0-9]*");
      term.replace("×", "[A-Za-z0-9]*");
      if (endB)
	term += "\\b";
      if (startB)
	term = "\\b" + term;
      thisWhere = QString("tname regexp \"%1\"").arg(term);
    } else {
      thisWhere = QString("tname like \"%1\"").arg("%" + term + "%");
    }
    qDebug() << "thiswhere" << thisWhere;
    int cnt = count(thisWhere);
    ui->thisCount->setText(QString::number(cnt));
    ui->unionCount->setText(QString::number(count(unionWhere())));
    ui->intersectionCount->setText(QString::number(count(intersectionWhere())));
    ui->exclusionCount->setText(QString::number(count(exclusionWhere())));
    ui->applyUnion->setEnabled(true);
    ui->applyIntersection->setEnabled(true);
    ui->applyExclusion->setEnabled(true);

    QSqlQuery q(db->constQuery("select tname from trees where "
			       + thisWhere
			       + " order by tname"));
    QStringList lst;
    while (q.next())
      lst << q.value(0).toString();
    QString txt = lst.join("\n");
    ui->thisNames->setText(txt);
  }
}

void TSD_Data::updateExisting() {
  int cnt = count("visible>0 and " + existingWhere);
  ui->existingCount->setText(QString::number(cnt));
  QSqlQuery q(db->constQuery("select tname from trees where visible>0 and "
			     + existingWhere
			     + " order by tname"));
  QStringList lst;
  while (q.next())
    lst << q.value(0).toString();
  QString txt = lst.join("\n");
  ui->existingNames->setText(txt);
}


QString TSD_Data::unionWhere() const {
  if (existingWhere=="")
    return thisWhere;
  else
    return "(visible>0 and " + existingWhere + ") or " + thisWhere;
}

QString TSD_Data::intersectionWhere() const {
  if (existingWhere=="")
    return "1==0";
  else
    return "(visible>0 and " + existingWhere + ") and " + thisWhere;
}

QString TSD_Data::exclusionWhere() const {
  if (existingWhere=="")
    return "1==0";
  else
    return "(visible>0 and " + existingWhere + ") and not " + thisWhere;
}

int TSD_Data::count(QString where) const {
  return db->simpleQuery("select count (*) from trees where " + where).toInt();  
}

TreeSearchDialog::TreeSearchDialog(SBEMDB *db, TreeModel *treemodel,
				   QWidget *parent):
  QDialog(parent), d{new TSD_Data(this, db, treemodel)} {
  d->ui->setupUi(this);

  connect(d->ui->clear, &QAbstractButton::clicked,
	  [this]() { d->clear(); });
  connect(d->ui->close, &QAbstractButton::clicked,
	  [this]() { d->close(); });
  connect(d->ui->load, &QAbstractButton::clicked,
	  [this]() { d->load(); });
  connect(d->ui->store, &QAbstractButton::clicked,
	  [this]() { d->store(); });
  connect(d->ui->help, &QAbstractButton::clicked,
	  [this]() { d->help(); });
  connect(d->ui->applyIntersection, &QAbstractButton::clicked,
	  [this]() { d->applyIntersection(); });
  connect(d->ui->applyUnion, &QAbstractButton::clicked,
	  [this]() { d->applyUnion(); });
  connect(d->ui->applyExclusion, &QAbstractButton::clicked,
	  [this]() { d->applyExclusion(); });
  connect(d->ui->searchTerm, &QLineEdit::textChanged,
	  [this]() { d->updateSearch(); });
}

TreeSearchDialog::~TreeSearchDialog() {
  delete d;
}

void TreeSearchDialog::open() {
  if (!d->db)
    return;
  d->import();
  show();
  /* Hide part of UI that is not yet implemented */
  d->ui->help->setVisible(false);
  d->ui->load->setVisible(false);
  d->ui->store->setVisible(false);
}
