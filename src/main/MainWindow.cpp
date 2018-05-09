// MainWindow.cpp

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QApplication>
#include "Navigation.h"
#include <QDockWidget>
#include <QMessageBox>
#include "ServerInfo.h"
#include "SBEMDB.h"
#include "TreeModel.h"
#include "TreeView.h"
#include "EditOverlay.h"
#include "Mode.h"
#include "Settings.h"
#include "Overview.h"
#include "NodeSearchDialog.h"
#include "NodeListWidget.h"
#include "ProjectionWidget.h"

#include <QTime>
#include <QDoubleValidator>
#include <QDebug>
#include <QFileDialog>
#include <cmath>
#include <QTimer>

class MWData {
public:
  void timeout();
public:
  MWData(MainWindow *mw, ServerInfo *info, Ui_MainWindow *ui):
    mw(mw), info(info), ui(ui) {
    timer = new QTimer(mw);
    db = new SBEMDB;
    tm = new TreeModel(db);
    eo = new EditOverlay(db, ui->tileviewer);
    ui->tileviewer->addOverlay(eo);
    QObject::connect(timer, &QTimer::timeout, [this]() { timeout(); });
  }
  ~MWData() {
    db->close();
    delete eo;
    delete tm;
    delete db;
  }
public:
  MainWindow *mw;
  ServerInfo *info; // provided
  Ui_MainWindow *ui; // created by MainWindow
  SBEMDB *db; // we create
  TreeModel *tm; // we create
  EditOverlay *eo; // we create
  QTimer *timer;
public:
  double xscale() {
    return info->contains("dx") ? info->real("dx") : 0.0055;
  }
  double yscale() {
    return info->contains("dy") ? info->real("dy") : 0.0055;
  }
  double zscale() {
    return info->contains("dz") ? info->real("dz") : 0.050;
  }
  void autoBlack() {
    int v = ui->tileviewer->level(0.00001);
    ui->curves->ui->black->setValue(v);
    ui->curves->ui->xblack->setText(QString::number(v));
    ui->tileviewer->setBlackLevel(v);
  }
  void autoWhite() {
    int v = ui->tileviewer->level(0.999);
    ui->curves->ui->white->setValue(v);
    ui->curves->ui->xwhite->setText(QString::number(v));
    ui->tileviewer->setWhiteLevel(v);
  }
  void autoGamma() {
    int p50 = ui->tileviewer->level(.5);
    int b = ui->curves->ui->black->value();
    int w = ui->curves->ui->white->value();
    /* I want p50 to map to g. Typically, g = 0.5.
       We know that b maps to 0 and w maps to 1.
       Now, y = ((x-b)/(w-b))^gamma. So I want
       0.5 = ((p50 - b)/(w-b))^gamma. In other words,
       gamma = log[0.5]/log((p50 - b)/(w-b)).
     */
    if (w<=b || p50<=b)
      return; // cannot do it.
    double gamma = log(.5) / log((p50-b)*1.0/(w-b));
    gamma = sqrt(gamma); // I don't want to overdo it.
    /* The slider value v is interpreted as
       gamma = 2^(v/50),
       so v=50*(log(gamma)/log(2)).
    */
    int v = 50*log(gamma)/log(2);
    ui->curves->ui->gamma->setValue(v);
    v = ui->curves->ui->gamma->value(); // let's clip
    double g = pow(2, v/50.);
    ui->curves->ui->xgamma->setText(QString::number(g, 'f', 2));
    ui->tileviewer->setGamma(g);
  }
  void openDB(QString fn) {
    tm->beginReset();
    db->open(fn);
    tm->concludeReset();
    ui->mode->ui->editTrees->setEnabled(true);
    ui->treeView->updateAfterChangingDB();
    ui->modeDock->show();
    ui->treeDock->show();
    mw->resizeDocks({ui->modeDock}, {10}, Qt::Vertical);
    quint64 nid = db->simpleQuery("select nid from selectednode").toULongLong();
    if (nid>0) {
      auto n = db->node(nid);
      qDebug() << n.x << n.y << n.z;
      if (n.nid)
        ui->tileviewer->setPosition(n.x, n.y, n.z);
    }
    Settings settings;
    settings.set("database", fn);
    enableEditActions(true);
  }
  void createDB() {
    QString fn = QFileDialog::getSaveFileName(0, "Create Database...", "",
                                              "*.sbemdb");
    if (fn.isEmpty())
      return;
    if (!fn.endsWith(".sbemdb"))
      fn += ".sbemdb";
    SBEMDB::create(fn);
    openDB(fn);
  }
  void openDB() {
    QString fn = QFileDialog::getOpenFileName(0, "Open Database...", "",
                                              "*.sbemdb");
    if (fn.isEmpty())
      return;
    openDB(fn);
  }
  void toggleTimer() {
    if (ui->nav->ui->autoupdate->isChecked()) {
      timer->setInterval(5000);
      timer->start();
      timeout();
    } else {
      timer->stop();
    }
  }
  void enableEditActions(bool en) {
    ui->actionTypeTreeNode->setEnabled(en);
    ui->actionTypePresynapticTerminal->setEnabled(en);
    ui->actionTypePostsynapticDensity->setEnabled(en);
    ui->actionTypeSoma->setEnabled(en);
    ui->actionTypeNeuropilExitPoint->setEnabled(en);
    ui->actionEditMemo->setEnabled(en);
    ui->actionConnectNodes->setEnabled(en);
    ui->actionDisconnectNodes->setEnabled(en);
    ui->actionDeleteNode->setEnabled(en);
    ui->actionConnectTerminals->setEnabled(en);
    ui->actionDissolveSynapse->setEnabled(en);
    ui->actionNewTree->setEnabled(en);
    ui->actionDeleteTree->setEnabled(en);
    ui->actionShowTrees->setEnabled(en);
    ui->actionHideTrees->setEnabled(en);
    ui->actionCenterSelectedNode->setEnabled(en);
    ui->treeView->ui->add->setText("+");
    ui->treeView->ui->del->setText(QWidget::tr("–"));
    ui->treeView->ui->show->setText(QWidget::tr("⭕")); // 🌕
    ui->treeView->ui->hide->setText(QWidget::tr("◍")); // 🌘
    ui->actionFindNode->setEnabled(en);
  }
  void restoreDialogs() {
    mw->addDockWidget(Qt::LeftDockWidgetArea, ui->navdock);
    ui->navdock->show();
    mw->addDockWidget(Qt::LeftDockWidgetArea, ui->curvedock);
    ui->curvedock->show();
    mw->addDockWidget(Qt::LeftDockWidgetArea, ui->overviewDock);
    ui->overviewDock->show();
    mw->resizeDocks({ui->navdock, ui->curvedock, ui->overviewDock},
                {10, 10, 10000}, Qt::Vertical);
    if (db && db->isOpen()) {
      mw->addDockWidget(Qt::RightDockWidgetArea, ui->modeDock);
      ui->modeDock->show();
      mw->addDockWidget(Qt::RightDockWidgetArea, ui->treeDock);
      ui->treeDock->show();
      mw->resizeDocks({ui->modeDock}, {10}, Qt::Vertical);
    }
  }
  void doFindNodeDialog() {
    QVector<SBEMDB::Node> nodes = NodeSearchDialog::exec1(db);
    qDebug() << "NSD: " << nodes.size();
    if (nodes.isEmpty())
      return;
    NodeListWidget *w = new NodeListWidget(info, db, nodes);
    QObject::connect(w, &NodeListWidget::selected,
            [this, nodes](int k) {
              SBEMDB::Node n(nodes[k]);
              ui->treeView->setActiveTree(n.tid);
              eo->setActiveNode(n.nid);
              ui->tileviewer->setPosition(n.x, n.y, n.z);
            });
    w->show();
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->resize(w->sizeHint());
  }
  void do3DProjection() {
    ProjectionWidget *w = new ProjectionWidget(info, db);
    w->addSelectedTree();
    w->addVisibleTrees();
    w->show();
    w->setAttribute(Qt::WA_DeleteOnClose);
  }
  void centerSelectedNode() {
    int nid = db->selectedNode();
    if (nid>0) {
      SBEMDB::Node n = db->node(nid);
      ui->tileviewer->setPosition(n.x, n.y, n.z);
    }
  }
};

void MWData::timeout() {
  qDebug() << "timeout";
  int z = ui->tileviewer->z();
  ui->tileviewer->setZ(1000000);
  if (ui->tileviewer->z() != z) {
    // updated
    QTime now = QTime::currentTime();
    ui->nav->ui->updateinfo->setText("Updated: " + now.toString());
  }
}

MainWindow::MainWindow(TileCache *cache, ServerInfo *info) {
  ui = new Ui_MainWindow;
  ui->setupUi(this);
  d = new MWData(this, info, ui);
  ui->tileviewer->setCache(cache);
  ui->tileviewer->setInfo(info);
  ui->overview->setInfo(info);
  ui->overview->setViewer(ui->tileviewer);
  auto *tc = new TileCache(cache->urlRoot());
  tc->setAutoNeighbors(true);
  tc->setMaxRetained(50);
  ui->overview->setCache(tc);

  connect(ui->tileviewer, SIGNAL(scaleChanged(int)),
          ui->overview, SLOT(update()));
  connect(ui->tileviewer, SIGNAL(viewChanged(int,int,int,bool)),
          ui->overview, SLOT(update()));
  connect(ui->overview, &Overview::clicked,
          [this](int x, int y) { ui->tileviewer->setPosition(x, y); });
  
  connect(ui->actionCreate_DB, &QAction::triggered,
          [this]() { d->createDB(); });
  connect(ui->actionOpen_DB, &QAction::triggered,
          [this]() { d->openDB(); });
  connect(ui->actionQuit, &QAction::triggered,
          []() { QApplication::quit(); });
  connect(ui->actionAbout, &QAction::triggered,
          this, &MainWindow::aboutAct);
  connect(ui->actionRestoreDialogs, &QAction::triggered,
          [this]() { d->restoreDialogs(); });

  connect(ui->actionTypeTreeNode, &QAction::triggered,
          [this]() { d->eo->actSetNodeType(SBEMDB::TreeNode); });
  ui->actionTypeTreeNode->setShortcut(tr("T, N"));
  connect(ui->actionTypePresynapticTerminal, &QAction::triggered,
          [this]() { d->eo->actSetNodeType(SBEMDB::PresynTerm); });
  ui->actionTypePresynapticTerminal->setShortcut(tr("T, T"));
  connect(ui->actionTypePostsynapticDensity, &QAction::triggered,
          [this]() { d->eo->actSetNodeType(SBEMDB::PostsynTerm); });
  ui->actionTypePostsynapticDensity->setShortcut(tr("T, D"));
  connect(ui->actionTypeSoma, &QAction::triggered,
          [this]() { d->eo->actSetNodeType(SBEMDB::Soma); });
  ui->actionTypeSoma->setShortcut(tr("T, S"));
  connect(ui->actionTypeNeuropilExitPoint, &QAction::triggered,
          [this]() { d->eo->actSetNodeType(SBEMDB::ExitPoint); });
  ui->actionTypeNeuropilExitPoint->setShortcut(tr("T, E"));
  connect(ui->actionEditMemo, &QAction::triggered,
          d->eo, &EditOverlay::actEditMemo);
  connect(ui->actionConnectNodes, &QAction::triggered,
          d->eo, &EditOverlay::actConnectNodes);
  connect(ui->actionDisconnectNodes, &QAction::triggered,
          d->eo, &EditOverlay::actDisconnectNodes);
  connect(ui->actionDeleteNode, &QAction::triggered,
          d->eo, &EditOverlay::actDeleteNode);
  connect(ui->actionConnectTerminals, &QAction::triggered,
          d->eo, &EditOverlay::actConnectTerminals);
  connect(ui->actionDissolveSynapse, &QAction::triggered,
          d->eo, &EditOverlay::actDissolveSynapse);

  connect(ui->actionNewTree, &QAction::triggered,
          ui->treeView, &TreeView::actNewTree);
  connect(ui->actionDeleteTree, &QAction::triggered,
          ui->treeView, &TreeView::actDeleteTree);
  connect(ui->actionShowTrees, &QAction::triggered,
          ui->treeView, &TreeView::actShowAll);
  connect(ui->actionHideTrees, &QAction::triggered,
          ui->treeView, &TreeView::actHideAll);
  connect(ui->actionFindNode, &QAction::triggered,
          [this]() { d->doFindNodeDialog(); });
  connect(ui->action3DProjection, &QAction::triggered,
          [this]() { d->do3DProjection(); });
  connect(ui->actionCenterSelectedNode, &QAction::triggered,
          [this]() { d->centerSelectedNode(); });
  
  
  ui->treeView->ui->add->setDefaultAction(ui->actionNewTree);
  ui->treeView->ui->del->setDefaultAction(ui->actionDeleteTree);
  ui->treeView->ui->show->setDefaultAction(ui->actionShowTrees);
  ui->treeView->ui->hide->setDefaultAction(ui->actionHideTrees);

  d->enableEditActions(false);
  
  ui->nav->ui->zoom->setText(QString("2<sup>–%1</sup>")
                             .arg(ui->tileviewer->scale()));
  connect(ui->tileviewer, &TileViewer::scaleChanged,
	  [this](int a) {
	    ui->nav->ui->zoom->setText(QString("2<sup>–%1</sup").arg(a));
	  });
  
  connect(ui->tileviewer, &TileViewer::viewChanged,
          [this](int x, int y, int z, bool c) {
            ui->nav->ui->x->setText(QString("%1").arg(x*d->xscale(), 0, 'f', 2));
            ui->nav->ui->y->setText(QString("%1").arg(y*d->yscale(), 0, 'f', 2));
            ui->nav->ui->z->setText(QString("%1").arg(z*d->zscale(), 0, 'f', 2));
            QPalette pal(ui->nav->ui->x->palette());
            QBrush b(c ? QColor(0, 64, 255) : QColor(0, 0, 0));
            pal.setBrush(QPalette::Text, b);
            ui->nav->ui->x->setPalette(pal);
            ui->nav->ui->y->setPalette(pal);
          });

  ui->nav->ui->nextslice->setDefaultAction(new QAction(">"));
  ui->nav->ui->nextslice2->setDefaultAction(new QAction(tr("≫")));
  ui->nav->ui->nextslice3->setDefaultAction(new QAction(tr("⋙")));
  ui->nav->ui->lastslice->setDefaultAction(new QAction(tr("⇥")));
  ui->nav->ui->prevslice->setDefaultAction(new QAction("<"));
  ui->nav->ui->prevslice2->setDefaultAction(new QAction(tr("≪")));
  ui->nav->ui->prevslice3->setDefaultAction(new QAction(tr("⋘")));
  ui->nav->ui->firstslice->setDefaultAction(new QAction(tr("⇤")));
  
  connect(ui->nav->ui->nextslice, &QToolButton::triggered,
          [this]() { ui->tileviewer->stepZ(1); });
  connect(ui->nav->ui->nextslice2, &QToolButton::triggered,
          [this]() { ui->tileviewer->stepZ(10); });
  connect(ui->nav->ui->nextslice3, &QToolButton::triggered,
          [this]() { ui->tileviewer->stepZ(100); });
  connect(ui->nav->ui->lastslice, &QToolButton::triggered,
          [this]() { ui->tileviewer->setZ(1000000); });
  connect(ui->nav->ui->prevslice, &QToolButton::triggered,
          [this]() { ui->tileviewer->stepZ(-1); });
  connect(ui->nav->ui->prevslice2, &QToolButton::triggered,
          [this]() { ui->tileviewer->stepZ(-10); });
  connect(ui->nav->ui->prevslice3, &QToolButton::triggered,
          [this]() { ui->tileviewer->stepZ(-100); });
  connect(ui->nav->ui->firstslice, &QToolButton::triggered,
          [this]() { ui->tileviewer->setZ(0); });

  ui->nav->ui->left->setDefaultAction(new QAction("<"));
  ui->nav->ui->right->setDefaultAction(new QAction(">"));
  ui->nav->ui->up->setDefaultAction(new QAction("<"));
  ui->nav->ui->down->setDefaultAction(new QAction(">"));

  connect(ui->nav->ui->left, &QToolButton::triggered,
          [this]() { ui->tileviewer
              ->stepX(-9*ui->tileviewer->visibleXRange()/10); });
  connect(ui->nav->ui->right, &QToolButton::triggered,
          [this]() { ui->tileviewer
              ->stepX(9*ui->tileviewer->visibleXRange()/10); });
  connect(ui->nav->ui->up, &QToolButton::triggered,
          [this]() { ui->tileviewer
              ->stepY(-9*ui->tileviewer->visibleYRange()/10); });
  connect(ui->nav->ui->down, &QToolButton::triggered,
          [this]() { ui->tileviewer
              ->stepY(9*ui->tileviewer->visibleYRange()/10); });

  ui->nav->ui->zoomin->setDefaultAction(new QAction("+"));
  connect(ui->nav->ui->zoomin, &QToolButton::triggered,
          [this]() { ui->tileviewer->setScale(ui->tileviewer->scale()-1); });

  ui->nav->ui->zoomout->setDefaultAction(new QAction("–"));
  connect(ui->nav->ui->zoomout, &QToolButton::triggered,
          [this]() { ui->tileviewer->setScale(ui->tileviewer->scale()+1); });

  ui->curves->ui->autoBlack->setDefaultAction(new QAction("Auto"));
  ui->curves->ui->autoWhite->setDefaultAction(new QAction("Auto"));
  ui->curves->ui->autoGamma->setDefaultAction(new QAction("Auto"));
  connect(ui->curves->ui->autoBlack, &QToolButton::triggered,
          [this]() { d->autoBlack(); });
  connect(ui->curves->ui->autoWhite, &QToolButton::triggered,
          [this]() { d->autoWhite(); });
  connect(ui->curves->ui->autoGamma, &QToolButton::triggered,
          [this]() { d->autoGamma(); });
  
  ui->nav->ui->x->setText(QString("%1")
                         .arg(ui->tileviewer->x()*.0055, 0, 'f', 2));
  ui->nav->ui->y->setText(QString("%1")
                         .arg(ui->tileviewer->y()*.0055, 0, 'f', 2));
  ui->nav->ui->z->setText(QString("%1")
                         .arg(ui->tileviewer->z()*.05, 0, 'f', 2));

  ui->nav->ui->x->setValidator(new QDoubleValidator);
  ui->nav->ui->y->setValidator(new QDoubleValidator);
  ui->nav->ui->z->setValidator(new QDoubleValidator);
  
  connect(ui->nav->ui->x, &QLineEdit::returnPressed,
          [this]() {
            bool ok;
            double x = ui->nav->ui->x->text().toDouble(&ok);
            qDebug() << "set x" << x;
            if (ok) ui->tileviewer->setX(x/d->xscale());
          });
  connect(ui->nav->ui->y, &QLineEdit::returnPressed,
          [this]() {
            bool ok;
            double y = ui->nav->ui->y->text().toDouble(&ok);
            qDebug() << "set y" << y;
            if (ok) ui->tileviewer->setY(y/d->yscale());
          });
  connect(ui->nav->ui->z, &QLineEdit::returnPressed,
          [this]() {
            bool ok;
            double z = ui->nav->ui->z->text().toDouble(&ok);
            qDebug() << "set z" << z;
            if (ok) ui->tileviewer->setZ(z/d->zscale());
          });
  connect(ui->nav->ui->autoupdate, &QPushButton::toggled,
          [this]() { d->toggleTimer(); });

  connect(ui->curves->ui->black, &QSlider::sliderMoved,
          [this](int v) {
            ui->curves->ui->xblack->setText(QString::number(v));
            ui->tileviewer->setBlackLevel(v);
          });
  connect(ui->curves->ui->white, &QSlider::sliderMoved,
          [this](int v) {
            ui->curves->ui->xwhite->setText(QString::number(v));
            ui->tileviewer->setWhiteLevel(v);
          });
  connect(ui->curves->ui->gamma, &QSlider::sliderMoved,
          [this](int v) {
            double g = pow(2, v/50.);
            ui->curves->ui->xgamma->setText(QString::number(g, 'f', 2));
            ui->tileviewer->setGamma(g);
          });

  connect(ui->curves->ui->sharp, &QSlider::sliderMoved,
          [this](int v) {
            double g = v/25.;
            ui->curves->ui->xsharp->setText(QString::number(g, 'f', 2));
            ui->tileviewer->setSharpening(g);
          });

  ui->tileviewer->setFocusPolicy(Qt::StrongFocus);
  resizeDocks({ui->navdock, ui->curvedock, ui->overviewDock},
              {10, 10, 10000}, Qt::Vertical);
  ui->modeDock->hide();
  ui->treeDock->hide();

  ui->treeView->setModel(d->tm);
  connect(ui->treeView, &TreeView::activeTreeChanged,
          d->eo, &EditOverlay::setActiveTree);
  connect(ui->treeView, &TreeView::activeNodeChanged,
          d->eo, &EditOverlay::setActiveNode);
  connect(d->tm, &TreeModel::visibilityChanged,
          [this]() { ui->tileviewer->update(); });
  connect(d->eo, &EditOverlay::otherTreePressed,
          [this](quint64 tid, quint64 nid) {
            ui->treeView->setActiveTree(tid);
            d->eo->setActiveNode(nid); });
  connect(d->eo, &EditOverlay::treeTableAltered,
          [this]() { d->tm->beginReset(); d->tm->concludeReset(); });
  connect(ui->mode->ui->editTrees, &QRadioButton::toggled,
          [this](bool b) {
            ui->tileviewer->setMode(b ? Mode_Edit : Mode_View);
          });
  connect(d->eo, &EditOverlay::gotoNodeRequest,
          [this](Point const &p) {
            ui->tileviewer->setPosition(p.x, p.y, p.z);
          });
}

MainWindow::~MainWindow() {
  delete ui;
  delete d;
}

void MainWindow::aboutAct() {
  QString me0 = "SBEM Viewer";
  QString me = "<b>" + me0 + "</b>";
  QString vsn = "0.1";
  QMessageBox::about(0, "About " + me0,
                     me + " " + vsn
                     + "<p>" + "(C) 2018 Daniel A. Wagenaar\n"
                     + "<p>" + me + " is a program for visualizing the SBEM data presently being collected by the Wagenaar Lab at Caltech and NCMIR at UCSD.\n"
                     + "<p>" + me + " is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.\n"
                     + "<p>" + me + " is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.\n"
                     + "<p>" + "You should have received a copy of the GNU General Public License along with this program. If not, see <a href=\"http://www.gnu.org/licenses/gpl-3.0.en.html\">www.gnu.org/licenses/gpl-3.0.en.html</a>.");
}

void MainWindow::resizeEvent(QResizeEvent *e) {
  resizeDocks({ui->navdock, ui->curvedock, ui->overviewDock},
              {10, 10, 10000}, Qt::Vertical);
  QMainWindow::resizeEvent(e);
}

void MainWindow::openDB(QString fn) {
  d->openDB(fn);
}

SBEMDB *MainWindow::database() const {
  return d->db;
}

TileViewer *MainWindow::tileViewer() const {
  return ui->tileviewer;
}

void MainWindow::closeEvent(QCloseEvent *) {
  QApplication::quit();
}
