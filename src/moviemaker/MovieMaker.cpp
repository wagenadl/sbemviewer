// MovieMaker.cpp

#include "MovieMaker.h"
#include "MovieRender.h"
#include "ui_MovieMaker.h"
#include <QDir>
#include <QDebug>

class MM_Data {
public:
  MM_Data(MovieMaker *w, ServerInfo const *info, SBEMDB const *db):
    w(w), info(info), db(db) {
    ui = new Ui_MovieMaker;
    render = new MovieRender(info, db, w);
  }
  void doRender();
public:
  MovieMaker *w;
  ServerInfo const *info;
  SBEMDB const *db;
  MovieRender *render;
  Ui_MovieMaker *ui;
};

MovieMaker::MovieMaker(ServerInfo const *info, SBEMDB const *db,
		       QWidget *parent):
  QMainWindow(parent), d(new MM_Data(this, info, db)) {
  d->ui->setupUi(this);
  d->ui->controls->setDatabase(db);
  d->render->setSettings(d->ui->controls->settings());
  QImage img(d->render->render(d->ui->controls->previewFrame()));
  d->ui->preview->setPixmap(QPixmap::fromImage(img));
  connect(d->ui->controls, &MMControls::settingsChanged,
	  [this]() {
	    qDebug() << "settingschanged";
	    d->render->setSettings(d->ui->controls->settings());
	    QImage img(d->render->render(d->ui->controls->previewFrame()));
	    d->ui->preview->setPixmap(QPixmap::fromImage(img));
	  });	    
  connect(d->ui->controls, &MMControls::previewChanged,
	  [this]() {
	    QImage img(d->render->render(d->ui->controls->previewFrame()));
	    d->ui->preview->setPixmap(QPixmap::fromImage(img));
	  });
  connect(d->ui->actionRender, &QAction::triggered,
	  [this]() { d->doRender(); });  
}

MovieMaker::~MovieMaker() {
  delete d;
}

void MM_Data::doRender() {
  QString odir = "/tmp/moviemaker";
  QDir::root().mkpath(odir);
  int N = ui->controls->settings().frameCount;
  for (int n=0; n<N; n++) {
    QImage img = render->render(n);
    img.save(QString("%1/%2.png").arg(odir).arg(n));
    qDebug() << "rendered" << n << N;
  }
}
