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

#include <QDoubleValidator>
#include <QDebug>
#include <QFileDialog>
#include <cmath>

class MWData {
public:
  MWData() {
    db = new SBEMDB;
    tm = new TreeModel(db);
  }
  ~MWData() {
    db->close();
    delete db;
  }
public:
  ServerInfo *info; // provided
  Ui_MainWindow *ui; // created by MainWindow
  SBEMDB *db; // we create
  TreeModel *tm; // we create
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
  void createDB() {
    QString fn = QFileDialog::getSaveFileName(0, "Create Database...", "",
                                              "*.sbemdb");
    if (fn.isEmpty())
      return;
    if (!fn.endsWith(".sbemdb"))
      fn += ".sbemdb";
    SBEMDB::create(fn);
    tm->beginReset();
    db->open(fn);
    tm->concludeReset();
    ui->mode->ui->editTrees->setEnabled(true);
  }
  void openDB() {
    QString fn = QFileDialog::getOpenFileName(0, "Open Database...", "",
                                              "*.sbemdb");
    if (fn.isEmpty())
      return;
    tm->beginReset();
    db->open(fn);
    tm->concludeReset();
    ui->mode->ui->editTrees->setEnabled(true);
  }
};

MainWindow::MainWindow(TileCache *cache, ServerInfo *info) {
  ui = new Ui_MainWindow;
  d = new MWData;
  d->info = info;
  d->ui = ui;
  ui->setupUi(this);
  ui->tileviewer->setCache(cache);
  ui->tileviewer->setInfo(info);
  
  connect(ui->actionCreate_DB, &QAction::triggered,
          [this]() { d->createDB(); });
  connect(ui->actionOpen_DB, &QAction::triggered,
          [this]() { d->openDB(); });
  connect(ui->actionQuit, &QAction::triggered,
          []() { QApplication::quit(); });
  connect(ui->actionAbout, &QAction::triggered,
          this, &MainWindow::aboutAct);

  ui->nav->ui->zoom->setText(QString("2<sup>–%1</sup>")
                             .arg(ui->tileviewer->scale()));
  //ui->nav->ui->zoom->setText(QString("1/%1")
  //.arg(1<<ui->tileviewer->scale()));
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
          [this]() { ui->tileviewer->setZ(100000); });
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
            ui->curves->ui->xgamma->setText(QString::number(g, 'f', 2)); //("%1").arg(g, 1, 'g', 2));   
            ui->tileviewer->setGamma(g);
          });

  connect(ui->curves->ui->sharp, &QSlider::sliderMoved,
          [this](int v) {
            double g = v/25.;
            ui->curves->ui->xsharp->setText(QString::number(g, 'f', 2)); //("%1").arg(g, 1, 'g', 2));   
            ui->tileviewer->setSharpening(g);
          });

  ui->tileviewer->setFocusPolicy(Qt::StrongFocus);
  resizeDocks({ui->navdock}, {10}, Qt::Vertical);
  resizeDocks({ui->modeDock}, {10}, Qt::Vertical);

  ui->treeView->setModel(d->tm);
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
  QMainWindow::resizeEvent(e);
}
