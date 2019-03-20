// TreeSearchDialog.h

#ifndef TREESEARCHDIALOG_H

#define TREESEARCHDIALOG_H

#include <QDialog>
#include "SBEMDB.h"
#include "TreeModel.h"

class TreeSearchDialog: public QDialog {
public:
  TreeSearchDialog(SBEMDB *db, TreeModel *treemodel, QWidget *parent=0);
  /* The treemodel is notified if we make changes to visibility.
     That then updates the rest of the UI.
  */
  virtual ~TreeSearchDialog();
  void open(); // populate existingWhere from current selection
private:
  class TSD_Data *d;
  
};

#endif
