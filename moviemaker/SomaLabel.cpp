// SomaLabel.cpp

#include "SomaLabel.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

SomaLabel::SomaLabel() {
}

bool SomaLabel::load(QString fn) {
  re.clear();
  label.clear();
  QFile fd(fn);
  if (!fd.open(QFile::ReadOnly))
    return false;
  QTextStream ts(&fd);
  while (!ts.atEnd()) {
    QString line = ts.readLine().simplified();
    if (line.isEmpty())
      continue;
    if (line.startsWith("#"))
      continue;
    int spidx=line.indexOf(' ');
    if (spidx<0) {
      qDebug() << "SomaLabel cannot parse line: " << line;
      continue;
    }
    QString key = line.left(spidx);
    QString value = line.mid(spidx+1);
    if (key.isEmpty() || value.isEmpty()) {
      qDebug() << "SomaLabel cannot parse line: " << line;
      continue;
    }
    if (key[0].isDigit()) {
      re << QRegularExpression("^" + key + "\\b(?!\\.)");
      label << value;
    } else {
      re << QRegularExpression("\\(" + key + "\\b");
      label << value;
    }
  }
  return !isEmpty();
}

int SomaLabel::size() const {
  return re.size();
}

QString SomaLabel::lookup(QString tname) const {
  if (tname.isEmpty())
    return QString();
  int N = re.size();
  tname = tname.simplified();
  for (int n=0; n<N; n++) 
    if (re[n].match(tname).hasMatch())
      return label[n];
  return QString();
}
