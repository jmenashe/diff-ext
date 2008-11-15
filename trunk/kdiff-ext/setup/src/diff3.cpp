#include <Qt/qcheckbox.h>
#include <kurlrequester.h>

#include "diff3.h"

DIFF3::DIFF3(QWidget* parent)
: QWidget(parent)
{
  setupUi(this);
  _diff->setObjectName("kcfg_diff3_command");
  _diff_compare_folders->setObjectName("kcfg_diff3_folders");
  _diff_use_kio->setObjectName("kcfg_diff3_kio");
}

DIFF3::~DIFF3()
{
}

/*$SPECIALIZATION$*/


#include "diff3.moc"

