#include <iostream>
#include <Qt/qcheckbox.h>
#include <kurlrequester.h>
#include <kurlcompletion.h>
#include <kfiledialog.h>
#include <kprotocolmanager.h>

#include <settings.h>
#include "diff.h"

DIFF::DIFF(QWidget* parent, QString prefix) : QWidget(parent) {
  setupUi(this);
  _command->setObjectName(prefix+"_command");
  _command->setCompletionObject(new KUrlCompletion(KUrlCompletion::ExeCompletion));
  _folders->setObjectName(prefix+"_folders");
  _kio->setObjectName(prefix+"_kio");
}

DIFF::~DIFF() {
}

void
DIFF::browse() {
  QStringList filter;
  KFileDialog dlg(KUrl(""), QString(""), parentWidget());

  filter << "application/x-executable";  
  dlg.setMimeFilter(filter);
  
  if (!_command->text().isEmpty()) {
    KUrl u(_command->text());

    if (KProtocolManager::supportsListing(u))
      dlg.setSelection(u.url());
  }

  if (dlg.exec() == QDialog::Accepted) {
    _command->setText(dlg.selectedFile());
  }
}

/*$SPECIALIZATION$*/

#include "diff.moc"

