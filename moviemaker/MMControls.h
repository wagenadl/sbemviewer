// MMControls.h

#ifndef MMCONTROLS_H

#define MMCONTROLS_H

#include "SBEMDB.h"
#include "MMSettings.h"
#include <QWidget>

class MMControls: public QWidget {
  Q_OBJECT;
public:
  MMControls(QWidget *parent=0);
  virtual ~MMControls();
  void setDatabase(SBEMDB const *db);
  void setRender(class MovieRender *);
  void reset();
  MMSettings settings() const;
  int previewFrame() const;
signals:
  void settingsChanged();
  void previewChanged(int);
private:
  class MMC_Data *d;
};

#endif
