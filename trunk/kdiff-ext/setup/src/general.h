#ifndef __general_h__
#define __general_h__

#include "ui_general.h"

class GENERAL : public QWidget, private Ui::GENERAL {
  public:
    GENERAL(QWidget* parent = 0) : QWidget(parent) {
      setupUi(this);
    }
};

#endif // __general_h__
