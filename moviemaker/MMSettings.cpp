// MMSettings.cpp

#include "MMSettings.h"

MMSettings::MMSettings() {
  theta = 270;
  phi = 90;
  alsoPresynaptic = false;//true;
  alsoPostsynaptic = false;
  buildup = true;
  resolution = QSize(1152, 720);
  frameCount = 180;
  lineWidth = 1.5;
  keyWidth = 2.5;
  synapseDiameter = 5;
  somaDiameter = 20;
  shadow = 10;
  fontsize = 20;
}
