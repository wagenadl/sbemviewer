// MMSettings.cpp

#include "MMSettings.h"

MMSettings::MMSettings() {
  theta = 288;
  phi = 90;
  alsoPresynaptic = true;
  alsoPostsynaptic = false;
  resolution = QSize(720, 720);
  frameCount = 360;
  lineWidth = 1;
  keyWidth = 2;
  synapseDiameter = 4;
  somaDiameter = 10;
  shadow = 0;
}
