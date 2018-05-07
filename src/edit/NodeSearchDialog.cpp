// NodeSearchDialog.cpp

#include "NodeSearchDialog.h"
#include "ui_NodeSearchDialog.h"
#include "NSQuery.h"

#include <QDebug>

class NSD_Data {
public:
  NSD_Data(NodeSearchDialog *w, SBEMDB *db): w(w), db(db) {
    ui = new Ui_NodeSearchDialog;
  }
  void recount();
public:
  NodeSearchDialog *w;
  Ui_NodeSearchDialog *ui;
  SBEMDB *db;
};

void NSD_Data::recount() {
  if (!db)
    return;
  NSQuery qq(db, ui);
  int n = qq.count();
  ui->count->setText(QString::number(n));
}

NodeSearchDialog::NodeSearchDialog(SBEMDB *db, QWidget *parent):
  QDialog(parent), d(new NSD_Data(this, db)) {
  d->ui->setupUi(this);
  
  connect(d->ui->treeAll, &QRadioButton::toggled,
          [this]() { d->recount(); });
  connect(d->ui->treeSelected, &QRadioButton::toggled,
          [this]() { d->recount(); });
  connect(d->ui->treeVisible, &QRadioButton::toggled,
          [this]() { d->recount(); });

  connect(d->ui->nodeType, &QCheckBox::toggled,
          [this]() { d->recount(); });
  connect(d->ui->typeBranchpoint, &QCheckBox::toggled,
          [this]() { d->recount(); });
  connect(d->ui->typeEndpoint, &QCheckBox::toggled,
          [this]() { d->recount(); });
  connect(d->ui->typeExit, &QCheckBox::toggled,
          [this]() { d->recount(); });
  connect(d->ui->typeSoma, &QCheckBox::toggled,
          [this]() { d->recount(); });
  connect(d->ui->typePostPartner, &QCheckBox::toggled,
          [this]() { d->recount(); });
  connect(d->ui->typePrePartner, &QCheckBox::toggled,
          [this]() { d->recount(); });
  connect(d->ui->typePostsyn, &QCheckBox::toggled,
          [this]() { d->recount(); });
  connect(d->ui->typePresyn, &QCheckBox::toggled,
          [this]() { d->recount(); });
  
  connect(d->ui->memo, &QCheckBox::toggled,
          [this]() { d->recount(); });
  connect(d->ui->memoEquals, &QRadioButton::toggled,
          [this]() { d->recount(); });
  connect(d->ui->memoAndAlso, &QCheckBox::toggled,
          [this]() { d->recount(); });
  connect(d->ui->memoButNot, &QCheckBox::toggled,
          [this]() { d->recount(); });
  connect(d->ui->memoText, &QLineEdit::textEdited,
          [this]() { d->recount(); });
  connect(d->ui->memoAndText, &QLineEdit::textEdited,
          [this]() { d->recount(); });
  connect(d->ui->memoNotText, &QLineEdit::textEdited,
          [this]() { d->recount(); });
  
}

NodeSearchDialog::~NodeSearchDialog() {
  delete d;
}

QVector<SBEMDB::Node> NodeSearchDialog::nodes() const {
  NSQuery qq(d->db, d->ui);
  return qq.nodes();
}

void NodeSearchDialog::reset() {
}

QVector<SBEMDB::Node> NodeSearchDialog::exec1(SBEMDB *db) {
  NodeSearchDialog nsd(db);
  if (nsd.exec()) {
    return nsd.nodes();
  } else {
    return QVector<SBEMDB::Node>();
  }
}

