// MMSettings.h

#ifndef MMSETTINGS_H

#define MMSETTINGS_H

#include <QList>
#include <QSize>

struct MMSettings {
public:
  MMSettings();
public:
  double theta;
  double phi;
  QList<quint64> keyTrees;
  bool alsoPresynaptic;
  bool alsoPostsynaptic;
  bool buildup;
  QSize resolution;
  int frameCount;
  double lineWidth;
  double keyWidth;
  double synapseDiameter;
  double somaDiameter;
  double shadow;
  double fontsize;
};

#endif
