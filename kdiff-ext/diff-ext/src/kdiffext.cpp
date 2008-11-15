/*
 * Copyright (c) 2007, 2008 Sergey Zorin. All rights reserved.
 *
 * This software is distributable under the BSD license. See the terms
 * of the BSD license in the COPYING file provided with this software.
 *
 */
#include <Qt/qlabel.h>
#include <Qt/qstring.h>
#include <Qt/qwidget.h>
//#include <kinstance.h>
#include <kactionmenu.h>
#include <kgenericfactory.h>
#include <kmessagebox.h>
#include <ktoolinvocation.h>
#include <kiconloader.h>

#include <settings.h>

#include "kdiffext.h"
#include "kdiffext.moc"

typedef KGenericFactory<kdiffext> factory;
K_EXPORT_COMPONENT_FACTORY(kdiff_ext, factory("kdiff_ext"))

KFileItemList kdiffext::_files;

KActionMenu* 
kdiffext::populate_compare_to_menu(KActionCollection* actionCollection) {
  KActionMenu* compare_to_menu = new KActionMenu(actionCollection);

  unsigned int n = 0;
  for(KFileItemList::iterator i = _files.begin(); i != _files.end(); i++, n++) {
    if(Settings::diff_kio() || _files.first().isLocalFile()) {
      KAction* action = actionCollection->addAction(QString("kdiffext::%1").arg(n));
      action->setText(display(*i));
//TODO          action->setIcon(KIcon("diff_later"));
  //      action->setShortcut(Qt::Key_F6);
      connect(action, SIGNAL(triggered()), _mapper, SLOT(map()));
      
      _mapper->setMapping(action, n);
      compare_to_menu->addAction(action);
      action->setEnabled(Settings::diff_kio() || (*i).isLocalFile());
      
      compare_to_menu->addAction(action);
    }
  }
  
  compare_to_menu->addSeparator();
  KAction* action = actionCollection->addAction("kdiffext::clear");
  action->setText(i18n("Clear"));
  action->setIcon(KIcon("clear"));
//      action->setShortcut(Qt::Key_F6);
  connect(action, SIGNAL(triggered()), this, SLOT(clear()));
  compare_to_menu->addAction(action);
  
  return compare_to_menu;
}

kdiffext::kdiffext(QObject* parent, const QStringList&) : KonqPopupMenuPlugin(parent) {
  _mapper = 0;

//  KMessageBox::information(0, "kdiffext::()");
//  _files.setAutoDelete(true);

  Settings::self()->readConfig();
}

kdiffext::~kdiffext() {
  if(_mapper != 0) {
    delete _mapper;
  }
}

void
kdiffext::setup(KActionCollection* actionCollection,
                       const KonqPopupMenuInformation& popupMenuInfo,
                       QMenu *menu) {
  _selected = popupMenuInfo.items(); 
  int n = _selected.count();
  bool diff3_enabled = Settings::diff3_command().length() > 0;
  QList<QAction*> actions = actionCollection->actions();

  menu->addSeparator();
  if(n == 1) {
    if(_files.count() > 0) {
      if(Settings::diff_kio() || _files.first().isLocalFile()) {
        KAction* action = actionCollection->addAction("kdiffext::Compare to");
        action->setText(i18n("Compare to '%1'", display(_files.first())));
        action->setIcon(KIcon("diff"));
  //      action->setShortcut(Qt::Key_F6);
        connect(action, SIGNAL(triggered()), this, SLOT(compare_to()));
        menu->addAction(action);
      }
      
      _mapper = new QSignalMapper(this);
      connect(_mapper, SIGNAL(mapped(int)), this, SLOT(compare_to(int)));

      KActionMenu* compare_to_menu = populate_compare_to_menu(actionCollection);
      
      compare_to_menu->setText(i18n("Compare to"));
      compare_to_menu->setIcon(KIcon("diff"));
      
      menu->addAction(compare_to_menu);
    }
  } else if(n == 2) {
    KAction* action = actionCollection->addAction("kdiffext::Compare");
    action->setText(i18n("Compare"));
    action->setIcon(KIcon("diff"));
//      action->setShortcut(Qt::Key_F6);
    connect(action, SIGNAL(triggered()), this, SLOT(compare()));
    menu->addAction(action);
    if(diff3_enabled) {
      if(_files.count() > 0) {
        if(Settings::diff3_kio() || _files.first().isLocalFile()) {
          KAction* action = actionCollection->addAction("kdiffext::3-way compare to");
          action->setText(i18n("3-way compare to '%1'", display(_files.first())));
          action->setIcon(KIcon("diff3"));
    //      action->setShortcut(Qt::Key_F6);
          connect(action, SIGNAL(triggered()), this, SLOT(compare_to()));
          menu->addAction(action);
        }
        KActionMenu* compare_to_menu = populate_compare_to_menu(actionCollection);
        
        compare_to_menu->setText(i18n("3-way compare to"));
        compare_to_menu->setIcon(KIcon("diff3"));
        
        menu->addAction(compare_to_menu);
      }
    }
  } else if(n == 3) {
    if(diff3_enabled) {
      KAction* action = actionCollection->addAction("kdiffext::3-way Compare");
      action->setText(i18n("3-way Compare"));
      action->setIcon(KIcon("diff3"));
  //      action->setShortcut(Qt::Key_F6);
      connect(action, SIGNAL(triggered()), this, SLOT(compare3()));
      menu->addAction(action);
    }
  }
  
  KAction* action = actionCollection->addAction("kdiffext::Compare later");
  action->setText(i18n("Compare later"));
  action->setIcon(KIcon("diff_later"));
//      action->setShortcut(Qt::Key_F6);
  connect(action, SIGNAL(triggered()), this, SLOT(compare_later()));
  menu->addAction(action);
  
  menu->addSeparator();
}

void 
kdiffext::compare_later() {
  for(KFileItemList::iterator i = _selected.begin(); i != _selected.end(); i++) {
    KFileItemList::iterator found = _files.begin();
    while(found != _files.end()) {
      if((*found).url() == (*i).url()) {
        _files.erase(found);
        found = _files.end();
      } else {
        found++;
      }
    }
    _files.prepend(KFileItem(*i));
  }
}

void 
kdiffext::compare() {
  QStringList args;

  args << arg(_selected.at(0), Settings::diff_kio()) << arg(_selected.at(1), Settings::diff_kio());
  KToolInvocation::kdeinitExec(Settings::diff_command(), args);
}

void 
kdiffext::compare3() {
  QStringList args;

  args << arg(_selected.at(0), Settings::diff3_kio()) << arg(_selected.at(1), Settings::diff3_kio()) << arg(_selected.at(2), Settings::diff3_kio());
  KToolInvocation::kdeinitExec(Settings::diff3_command(), args);
}

void 
kdiffext::compare_to(int n) {
  QStringList args;

  args << arg(_selected.at(0), Settings::diff_kio()) << arg(_files.at(n), Settings::diff_kio());
  KToolInvocation::kdeinitExec(Settings::diff_command(), args);
}

void 
kdiffext::compare3_to(int n) {
  QStringList args;

  args << arg(_selected.at(0), Settings::diff3_kio()) << arg(_selected.at(1), Settings::diff3_kio()) << arg(_files.at(n), Settings::diff3_kio());
  KToolInvocation::kdeinitExec(Settings::diff3_command(), args);
}

void 
kdiffext::clear() {
  _files.clear();
}

QString
kdiffext::arg(const KFileItem& item, bool _3_way) {
  return _3_way ? item.url().url() : item.url().path();
}

QString
kdiffext::display(const KFileItem& item) {
  QString tmp = item.isLocalFile() ? item.url().path() : item.url().prettyUrl();
  QString result;
  int length = Settings::max_filename_length();

  if(length > 20) {
    if(tmp.length() > length) {
      int part_length = (length-3)/2;

      result = tmp.left(part_length) + "..." + tmp.right(part_length);
    } else {
      result = tmp;
    }
  } else {
    result = tmp;
  }

  return result;
}
