#include <kapplication.h>
#include <kurlrequester.h>
#include <kaboutapplication.h>
#include <kconfig.h>
#include <qlabel.h>
#include <qcheckbox.h>

#include "mainwidget.h"

MainWidget::MainWidget(QWidget* parent, const char* name, WFlags fl) : UI_Base(parent,name,fl) {
  _diff->setURL(kapp->config()->readEntry("diff-command"));
  _diff3->setURL(kapp->config()->readEntry("diff3-command"));
  _3_way_compare->setChecked(kapp->config()->readBoolEntry("3-way-diff-enabled", false));
  _compare_folders->setChecked(kapp->config()->readBoolEntry("compare-folders", false));
}

MainWidget::~MainWidget() {
  kapp->config()->sync();
}

void 
MainWidget::cancel_clicked() {
  KApplication::kApplication()->quit();
}

void 
MainWidget::apply_clicked() {
  apply();
}

void 
MainWidget::ok_clicked() {
  apply();
  KApplication::kApplication()->quit();
}

void 
MainWidget::about_clicked() {
  KAboutApplication* about = new KAboutApplication(this);
  about->show();
}

void
MainWidget::apply() {
  kapp->config()->writeEntry("diff-command", _diff->url());
  kapp->config()->writeEntry("diff3-command", _diff3->url());
  kapp->config()->writeEntry("3-way-diff-enabled", _3_way_compare->isChecked());
  kapp->config()->writeEntry("compare-folders", _compare_folders->isChecked());

  kapp->config()->sync();
}

#include "mainwidget.moc"
