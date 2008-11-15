#include <Qt/qcheckbox.h>
#include <kurlrequester.h>

#include "diff.h"

DIFF::DIFF(QWidget* parent)
: QWidget(parent)
{
  setupUi(this);
  _diff->setObjectName("kcfg_diff_command");
  _diff_compare_folders->setObjectName("kcfg_diff_folders");
  _diff_use_kio->setObjectName("kcfg_diff_kio");
}

DIFF::~DIFF()
{
}

/*$SPECIALIZATION$*/


#include "diff.moc"

