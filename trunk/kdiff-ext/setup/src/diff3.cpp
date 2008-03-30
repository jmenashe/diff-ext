#include <qcheckbox.h>
#include <kurlrequester.h>

#include "diff3.h"

DIFF3::DIFF3(QWidget* parent, const char* name, WFlags fl)
: UI_BASE(parent,name,fl)
{
  _diff->setName("kcfg_diff3_command");
  _diff_compare_folders->setName("kcfg_diff3_folders");
  _diff_use_kio->setName("kcfg_diff3_kio");
}

DIFF3::~DIFF3()
{
}

/*$SPECIALIZATION$*/


#include "diff3.moc"

