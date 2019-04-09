// MMSettings.cpp

#include "MMSettings.h"

MMSettings::MMSettings() {
  theta = 270;
  phi = 90;
  alsoPresynaptic = true;
  alsoPostsynaptic = false;
  buildup = true;
  resolution = QSize(720, 480);
  frameCount = 360;
  lineWidth = 1;
  keyWidth = 2;
  synapseDiameter = 4;
  somaDiameter = 10;
  shadow = 0;
}
