// NodeSearchDialog.cpp

#include "NodeSearchDialog.h"
#include "ui_NodeSearchDialog.h"

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
  
  bool onlySelectedTree = ui->treeSelected->isChecked();
  bool onlyVisibleTrees = ui->treeVisible->isChecked();
  bool useNodeType = ui->nodeType->isChecked();
  bool useBranchPoints = ui->typeBranchpoint->isChecked();
  bool useEndPoints = ui->typeEndpoint->isChecked();
  bool useExitPoint = ui->typeExit->isChecked();  
  bool useSoma = ui->typeSoma->isChecked();
  bool usePresyn = ui->typePresyn->isChecked();
  bool usePostsyn = ui->typePostsyn->isChecked();
  bool usePrePartner = ui->typePrePartner->isChecked();
  bool usePostPartner = ui->typePostPartner->isChecked();
  bool useMemo = ui->memo->isChecked();
  bool memoMustEqual = ui->memoEquals->isChecked();
  QString memoText = ui->memoText->text();
  QString memoAnd = ui->memoAndAlso->isChecked()
    ? ui->memoAndText->text() : QString();
  QString memoNot = ui->memoButNot->isChecked()
    ? ui->memoNotText->text() : QString();

  if (useNodeType && (usePrePartner || usePostPartner)) {
    // too complicated for now
  } else {
    // Let's build some clauses
    QStringList clauses;
    QStringList joins;
    QStringList args;
    QStringList cleanup;

    if (onlySelectedTree) {
      int tid = db->selectedTree();
      clauses << QString("tid==%1").arg(tid);
    } else if (onlyVisibleTrees) {
      clauses << QString("visible");
      joins << "natural join trees";
    }

    if (useNodeType) {
      // let's build some node type clauses
      QStringList clau1;
      if (useBranchpoint || useEndpoint) {
        // must be able to count edges
        joins << "natural join edgecount";
        QString q = "create temp table edgecount as select nid, count(1) as cnt"
          " from nodecons inner join nodes on nid1==nid";
        if (onlySelectedTree) {
          q += QString(" where tid==%1").arg(db->selectedTree());
        } else if (onlyVisibleTrees) {
          q += " natural join trees where visible";
        }
        q += " group by nid";
        db->query(q);
        cleanup << "drop table edgecount";
        QString clau = "(typ==3 and ";
        if (useBranchpoint && useEndpoint) {
          clau += "cnt!=2";
        } else if (useBranchpoint) {
          clau += "cnt>2";
        } else {
          clau += "cnt<2";
        }
        clau += ")";
        clau1 << clau;
      }
      if (useExitpoint)
        clau1 << "typ==2";
      if (useSoma)
        clau1 << "typ==1";
      if (usePresyn)
        clau1 << "typ==5";
      if (usePostsyn)
        clau1 << "typ==6";
      clauses << clau1.join(" or ");
    }
    if (useMemo) {
      joins << "natural join tags";
      if (memoMustEqual) {
        clauses << "tag==:a";
        args << memoText;
      } else {
        clauses << "tag like :a";
        args << ("%" + memoText + "%");
        if (!memoAnd.isEmpty()) {
          clauses << "tag like :b";
          args << ("%" + memoAndText + "%");
          if (!memoNot.isEmpty()) {
            clauses << "tag not like :c";
            args << ("%" + memoNotText + "%");
          }
        } else if (!memoNot.isEmpty()) {
          clauses << "tag not like :b";
          args << ("%" + memoNotText + "%");
        }      
      }
    }

    QString qq = "select nid from nodes ";
    qq += joins.join(" ");
    if (!clauses.isEmpty()) {
      qq += " where " + clauses.join(" and ");
    }
    qDebug() << "query" << qq;
    for (auto q: cleanup)
      db->query(q);
  }      
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
}

QVector<SBEMDB::Node> NodeSearchDialog::nodes() const {
  return QVector<SBEMDB::Node>();
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

