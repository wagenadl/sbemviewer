// TiffLoader.h

#ifndef TIFFLOADER_H

#define TIFFLOADER_H

#include <QString>
#include <QByteArray>
#include <QImage>

class TiffLoader {
public:
  static QImage load(QString fn);
  static QImage read(QByteArray data);
};

#endif
