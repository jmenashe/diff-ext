/*
 * Copyright (c) 2007 Sergey Zorin. All rights reserved.
 *
 * This software is distributable under the BSD license. See the terms
 * of the BSD license in the COPYING file provided with this software.
 *
 */
#include <qlabel.h>
#include <qstring.h>
#include <qwidget.h>
#include <kinstance.h>
#include <kgenericfactory.h>
#include <kmessagebox.h>
#include <kapplication.h>
#include <kiconloader.h>

#include <settings.h>

#include "kdiffext.h"
#include "kdiffext.moc"

typedef KGenericFactory<kdiffext, KonqPopupMenu> factory;
K_EXPORT_COMPONENT_FACTORY(konq_diffext, factory("kdiffext"))

KFileItemList kdiffext::_files;

kdiffext::kdiffext(KonqPopupMenu* popupmenu, const char* name, const QStringList&) : KonqPopupMenuPlugin(popupmenu, name) {
  _menu = popupmenu;

  _mapper = 0;

  _files.setAutoDelete(true);

  Settings::self()->readConfig();

  setup_actions();
}

kdiffext::~kdiffext() {
  if(_mapper != 0) {
    delete _mapper;
  }
}

void
kdiffext::setup_actions() {
  KFileItemList selected = _menu->fileItemList();
  int n = selected.count();
  bool diff3_enabled = Settings::diff3_command().length() > 0;

  addSeparator();
  if(n == 1) {
    if(_files.count() == 0) {
      addAction(new KAction(i18n("Compare later"), kapp->iconLoader()->loadIcon("diff_later.png", KIcon::Small), 0, this, SLOT(compare_later()), actionCollection(), "kdiffext::Compare later"));
    } else {
      if(Settings::diff_kio() || _files.first()->isLocalFile()) {
        addAction(new KAction(i18n("Compare to '%1'").arg(display(_files.first())), kapp->iconLoader()->loadIcon("diff.png", KIcon::Small), 0, this, SLOT(compare_to()), actionCollection(), "kdiffext::Compare to"));
      }
      addAction(new KAction(i18n("Compare later"), kapp->iconLoader()->loadIcon("diff_later.png", KIcon::Small), 0, this, SLOT(compare_later()), actionCollection(), "kdiffext::Compare later"));

      _mapper = new QSignalMapper(this);
      connect(_mapper, SIGNAL(mapped(int)), this, SLOT(compare_to(int)));

      KActionMenu* compare_to_menu = new KActionMenu(i18n("Compare to"), kapp->iconLoader()->loadIcon("diff.png", KIcon::Small), actionCollection(), "kdiffext::Compare to menu");
      unsigned int n = 0;
      for(KFileItemList::iterator i = _files.begin(); i != _files.end(); i++, n++) {
        if(Settings::diff_kio() || _files.first()->isLocalFile()) {
          KAction* action = new KAction(display(*i), kapp->iconLoader()->loadIcon((*i)->iconName(), KIcon::Small), 0, _mapper, SLOT(map()), compare_to_menu, QString("kdiffext::%1").arg(n));
          _mapper->setMapping(action, n);
          compare_to_menu->insert(action);
          action->setEnabled(Settings::diff_kio() || (*i)->isLocalFile());
        }
      }
      compare_to_menu->insert(new KActionSeparator());
      compare_to_menu->insert(new KAction(i18n("Clear"), kapp->iconLoader()->loadIcon("clear.png", KIcon::Small), 0, this, SLOT(clear()), compare_to_menu, QString("kdiffext::clear")));
      addAction(compare_to_menu);
    }
  } else if(n == 2) {
    if(diff3_enabled) {
      if(_files.count() == 0) {
        addAction(new KAction(i18n("Compare"), kapp->iconLoader()->loadIcon("diff.png", KIcon::Small), 0, this, SLOT(compare()), actionCollection(), "kdiffext::Compare"));
        addAction(new KAction(i18n("Compare later"), kapp->iconLoader()->loadIcon("diff_later.png", KIcon::Small), 0, this, SLOT(compare_later()), actionCollection(), "kdiffext::Compare later"));
      } else {
        if(Settings::diff3_kio() || _files.first()->isLocalFile()) {
          addAction(new KAction(i18n("3-way compare to '%1'").arg(display(_files.first())), kapp->iconLoader()->loadIcon("diff3.png", KIcon::Small), 0, this, SLOT(compare3_to()), actionCollection(), "kdiffext::3-way compare to"));
        }
        addAction(new KAction(i18n("Compare"), kapp->iconLoader()->loadIcon("diff.png", KIcon::Small), 0, this, SLOT(compare()), actionCollection(), "kdiffext::Compare"));
        addAction(new KAction(i18n("Compare later"), kapp->iconLoader()->loadIcon("diff_later.png", KIcon::Small), 0, this, SLOT(compare_later()), actionCollection(), "kdiffext::Compare later"));

        _mapper = new QSignalMapper(this);
        connect(_mapper, SIGNAL(mapped(int)), this, SLOT(compare3_to(int)));

        KActionMenu* compare_to_menu = new KActionMenu(i18n("3-way compare to"), kapp->iconLoader()->loadIcon("diff3.png", KIcon::Small), actionCollection(), "kdiffext::3-way compare to menu");
        unsigned int n = 0;
        for(KFileItemList::iterator i = _files.begin(); i != _files.end(); i++, n++) {
          KAction* action = new KAction(display(*i), kapp->iconLoader()->loadIcon((*i)->iconName(), KIcon::Small), 0, _mapper, SLOT(map()), compare_to_menu, QString("kdiffext::%1").arg(n));
          _mapper->setMapping(action, n);
          compare_to_menu->insert(action);
          action->setEnabled(Settings::diff3_kio() || (*i)->isLocalFile());
        }
        compare_to_menu->insert(new KActionSeparator());
        compare_to_menu->insert(new KAction(i18n("Clear"), kapp->iconLoader()->loadIcon("clear.png", KIcon::Small), 0, this, SLOT(clear()), compare_to_menu, QString("kdiffext::clear")));
        addAction(compare_to_menu);
      }
    } else {
      addAction(new KAction(i18n("Compare"), kapp->iconLoader()->loadIcon("diff.png", KIcon::Small), 0, this, SLOT(compare()), actionCollection(), "kdiffext::Compare"));
      addAction(new KAction(i18n("Compare later"), kapp->iconLoader()->loadIcon("diff_later.png", KIcon::Small), 0, this, SLOT(compare_later()), actionCollection(), "kdiffext::Compare later"));
    }
  } else if(n == 3) {
    if(diff3_enabled) {
      addAction(new KAction(i18n("3-way Compare"), kapp->iconLoader()->loadIcon("diff3.png", KIcon::Small), 0, this, SLOT(compare3()), actionCollection(), "kdiffext::3-way Compare"));
      addAction(new KAction(i18n("Compare later"), kapp->iconLoader()->loadIcon("diff_later.png", KIcon::Small), 0, this, SLOT(compare_later()), actionCollection(), "kdiffext::Compare later"));
    } else {
      addAction(new KAction(i18n("Compare later"), kapp->iconLoader()->loadIcon("diff_later.png", KIcon::Small), 0, this, SLOT(compare_later()), actionCollection(), "kdiffext::Compare later"));
    }
  } else {
    addAction(new KAction(i18n("Compare later"), kapp->iconLoader()->loadIcon("diff_later.png", KIcon::Small), 0, this, SLOT(compare_later()), actionCollection(), "kdiffext::Compare later"));
  }
  addSeparator();
}

void 
kdiffext::compare_later() {
  KFileItemList selected = _menu->fileItemList();

  for(KFileItemList::iterator i = selected.begin(); i != selected.end(); i++) {
    KFileItemList::iterator found = _files.begin();
    while(found != _files.end()) {
      if((*found)->url() == (*i)->url()) {
        _files.erase(found);
        found = _files.end();
      } else {
        found++;
      }
    }
    _files.prepend(new KFileItem(*(*i)));
  }
}

void 
kdiffext::compare() {
  QStringList args;

  KFileItemList selected = _menu->fileItemList();
  args << arg(selected.at(0), Settings::diff_kio()) << arg(selected.at(1), Settings::diff_kio());
  KApplication::kdeinitExec(Settings::diff_command(), args);
}

void 
kdiffext::compare3() {
  QStringList args;

  KFileItemList selected = _menu->fileItemList();
  args << arg(selected.at(0), Settings::diff3_kio()) << arg(selected.at(1), Settings::diff3_kio()) << arg(selected.at(2), Settings::diff3_kio());
  KApplication::kdeinitExec(Settings::diff3_command(), args);
}

void 
kdiffext::compare_to(int n) {
  QStringList args;

  KFileItemList selected = _menu->fileItemList();
  args << arg(selected.at(0), Settings::diff_kio()) << arg(_files.at(n), Settings::diff_kio());
  KApplication::kdeinitExec(Settings::diff_command(), args);
}

void 
kdiffext::compare3_to(int n) {
  QStringList args;

  KFileItemList selected = _menu->fileItemList();
  args << arg(selected.at(0), Settings::diff3_kio()) << arg(selected.at(1), Settings::diff3_kio()) << arg(_files.at(n), Settings::diff3_kio());
  KApplication::kdeinitExec(Settings::diff3_command(), args);
}

void 
kdiffext::clear() {
  _files.clear();
}

QString
kdiffext::arg(KFileItem* item, bool _3_way) {
  return _3_way ? item->url().url() : item->url().path();
}

QString
kdiffext::display(KFileItem* item) {
  QString tmp = item->isLocalFile() ? item->url().path() : item->url().prettyURL();
  QString result;
  unsigned int length = Settings::max_filename_length();

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
