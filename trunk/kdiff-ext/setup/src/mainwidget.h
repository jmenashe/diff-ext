#ifndef _KDIFFEXTSETUPWIDGET_H_
#define _KDIFFEXTSETUPWIDGET_H_
#include <kconfig.h>

#include "ui_base.h"

class MainWidget : public UI_Base {
  Q_OBJECT

  public:
    MainWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~MainWidget();

  public slots:
    virtual void about_clicked();
    virtual void ok_clicked();
    virtual void apply_clicked();
    virtual void cancel_clicked();

  protected:
    void apply();
};

#endif
