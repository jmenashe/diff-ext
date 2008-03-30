#include <qcheckbox.h>
#include <kurlrequester.h>

#include "diff.h"

DIFF::DIFF(QWidget* parent, const char* name, WFlags fl)
: UI_BASE(parent,name,fl)
{
  _diff->setName("kcfg_diff_command");
  _diff_compare_folders->setName("kcfg_diff_folders");
  _diff_use_kio->setName("kcfg_diff_kio");
}

DIFF::~DIFF()
{
}

/*$SPECIALIZATION$*/


#include "diff.moc"

