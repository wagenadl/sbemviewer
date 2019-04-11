// SomaLabel.h

#ifndef SOMALABEL_H

#define SOMALABEL_H

#include <QRegularExpression>
#include <QVector>

class SomaLabel {
public:
  SomaLabel();
  bool load(QString fn);
  /* The file must consist of lines with the format:
       KEY LABEL
     where KEY is either a number, or one or two lower case letters.

     If a number, it should be a uCT id, which is matched at the
     beginning of tnames and only if not followed by a period. Thus,
     "1" matches "1 (fr)" but not "1.pres.p.150".

     If letters, it should be a VSD id, which is matched between
     parentheses in tnames. Thus, "es" matches "115 (es)" but not
     "1.pres.p.106-posterior ggl". Also "ex" does not match "390 (exit
     point?)", but "eb" does match "211 (eb-24L)+1.pres.p.5 +
     17+23+111+112+113+114+116".

     LABEL should be a canonical cell name, e.g., "DE3_L" or "208".

     Lines starting with # are ignored.

     Returns TRUE if OK.
  */
  int size() const;
  bool isEmpty() const { return size()==0; }
  QString lookup(QString tname) const; // null string if no match
private:
  QVector<QRegularExpression> re;
  QVector<QString> label;
};

#endif
