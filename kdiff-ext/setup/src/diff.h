#ifndef DIFF_H
#define DIFF_H

#include <kconfigdialog.h>

#include "ui_base.h"

class DIFF : public QWidget, Ui_BASE {
  Q_OBJECT

  public:
    DIFF(QWidget* parent, QString prefix);
    ~DIFF();
    /*$PUBLIC_FUNCTIONS$*/

  public slots:
    void browse();

  protected:
    /*$PROTECTED_FUNCTIONS$*/

  protected slots:
    /*$PROTECTED_SLOTS$*/
};

#endif

