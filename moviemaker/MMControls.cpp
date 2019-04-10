// MMControls.cpp

#include "MMControls.h"
#include "ui_MMControls.h"
#include "MovieRender.h"
#include <QDebug>

class MMC_Data {
public:
  MMC_Data(MMControls *w): w(w) {
    db = 0;
    render = 0;
    ui = new Ui_MMControls;
  }
  void clearTrees();
  void removeTree();
  void addTree();
  void recalcFrameCount();
public:
  MMControls *w;
  SBEMDB const *db;
  MovieRender *render;
  Ui_MMControls *ui;
};

void MMC_Data::clearTrees() {
  bool any = ui->trees->count() > 0;
  ui->trees->clear();
  if (any) {
    recalcFrameCount();    
    w->settingsChanged();
  }
}

void MMC_Data::removeTree() {
  bool any = false;
  for (QListWidgetItem *i: ui->trees->selectedItems()) {
    //    ui->trees->removeItemWidget(i);
    delete i;
    any = true;
  }
  if (any) {
    recalcFrameCount();
    w->settingsChanged();
  }
}

void MMC_Data::addTree() {
  if (!db)
    return;
  quint64 tid = db->selectedTree();
  if (!tid)
    return;
  SBEMDB::Tree tree(db->tree(tid));
  QString lbl(QString("%1 | %2").arg(tree.tname).arg(tree.tid));
  QList<QListWidgetItem *> existing
    = ui->trees->findItems(lbl, Qt::MatchExactly);
  if (existing.isEmpty()) {
    ui->trees->addItem(lbl);
    recalcFrameCount();
    w->settingsChanged();
  }
}
    
MMControls::MMControls(QWidget *parent):
  QWidget(parent),
  d(new MMC_Data(this)) {
  d->ui->setupUi(this);

  connect(d->ui->theta, QOverload<int>::of(&QSpinBox::valueChanged),
	  [this]() { settingsChanged(); });
  connect(d->ui->phi, QOverload<int>::of(&QSpinBox::valueChanged),
	  [this]() { settingsChanged(); });
  connect(d->ui->trees, &QListWidget::itemChanged,
	  [this]() { settingsChanged(); });
  connect(d->ui->presyn, &QCheckBox::toggled,
	  [this]() { d->recalcFrameCount(); settingsChanged(); });
  connect(d->ui->postsyn, &QCheckBox::toggled,
	  [this]() { d->recalcFrameCount(); settingsChanged(); });
  connect(d->ui->buildup, &QCheckBox::toggled,
	  [this]() { d->recalcFrameCount(); settingsChanged(); });
  connect(d->ui->width, QOverload<int>::of(&QSpinBox::valueChanged),
	  [this]() { settingsChanged(); });
  connect(d->ui->height, QOverload<int>::of(&QSpinBox::valueChanged),
	  [this]() { settingsChanged(); });
  connect(d->ui->steps, QOverload<int>::of(&QSpinBox::valueChanged),
	  [this]() { d->recalcFrameCount(); settingsChanged(); });
  connect(d->ui->linewidth, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
	  [this]() { settingsChanged(); });
  connect(d->ui->keywidth, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
	  [this]() { settingsChanged(); });
  connect(d->ui->syndiam, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
	  [this]() { settingsChanged(); });
  connect(d->ui->somadiam, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
	  [this]() { settingsChanged(); });
  connect(d->ui->shadow, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
	  [this]() { settingsChanged(); });
  connect(d->ui->preview, &QSlider::sliderMoved,
    [this](int n) { previewChanged(n); });

  connect(d->ui->del, &QAbstractButton::clicked,
	  [this]() { d->removeTree(); });
  connect(d->ui->clear, &QAbstractButton::clicked,
	  [this]() { d->clearTrees(); });
  connect(d->ui->add, &QAbstractButton::clicked,
	  [this]() { d->addTree(); });

  reset();
}

MMControls::~MMControls() {
  delete d;
}

void MMControls::setDatabase(SBEMDB const *db) {
  d->db = db;
}

void MMControls::reset() {
  MMSettings s;
  d->ui->presyn->setChecked(s.alsoPresynaptic);
  d->ui->postsyn->setChecked(s.alsoPostsynaptic);
  d->ui->buildup->setChecked(s.buildup);
  d->ui->trees->clear();
  d->ui->theta->setValue(s.theta);
  d->ui->phi->setValue(s.phi);
  d->ui->width->setValue(s.resolution.width());
  d->ui->height->setValue(s.resolution.height());
  d->ui->steps->setValue(s.frameCount);
  d->ui->linewidth->setValue(s.lineWidth);
  d->ui->keywidth->setValue(s.keyWidth);
  d->ui->syndiam->setValue(s.synapseDiameter);
  d->ui->somadiam->setValue(s.somaDiameter);
  d->ui->shadow->setValue(s.shadow);
  d->ui->preview->setValue(0);
}

MMSettings MMControls::settings() const {
  MMSettings s;
  s.theta = d->ui->theta->value();
  s.phi = d->ui->phi->value();
  s.keyTrees = QList<quint64>();
  for (int r=0; r<d->ui->trees->count(); r++) {
    QString lbl = d->ui->trees->item(r)->text();
    int idx = lbl.lastIndexOf(" | ");
    if (idx>=0)
      s.keyTrees << lbl.mid(idx+3).toULongLong();
  }
  s.alsoPresynaptic = d->ui->presyn->isChecked();
  s.alsoPostsynaptic = d->ui->postsyn->isChecked();
  s.buildup = d->ui->buildup->isChecked();
  s.resolution = QSize(d->ui->width->value(), d->ui->height->value());
  s.frameCount = d->ui->steps->value();
  s.lineWidth = d->ui->linewidth->value();
  s.keyWidth = d->ui->keywidth->value();
  s.synapseDiameter = d->ui->syndiam->value();
  s.somaDiameter = d->ui->somadiam->value();
  s.shadow = d->ui->shadow->value();
  return s;
}

int MMControls::previewFrame() const {
  return d->ui->preview->sliderPosition();
}

void MMC_Data::recalcFrameCount() {
  int N = ui->steps->value();
  if (render) {
    render->setSettings(w->settings());
    N += render->buildupFrameCount();
  }
  qDebug() << "recalcFramecount" << N;
 ui->preview->setMaximum(N-1);
}

void MMControls::setRender(MovieRender *mr) {
  d->render = mr;
}
