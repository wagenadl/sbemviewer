// MovieRender.h

#ifndef MOVIERENDER_H

#define MOVIERENDER_H

#include "ServerInfo.h"
#include "SBEMDB.h"
#include "MMSettings.h"
#include "LineF.h"
#include "Transform3.h"

class MovieRender: public QObject {
public:
  MovieRender(SBEMDB const *, QObject *parent);
  virtual ~MovieRender();
  void rereadDatabase();
  void setSettings(MMSettings const &);
  QImage render(int frame);
private:
  class MR_Data *d;
};

#endif
