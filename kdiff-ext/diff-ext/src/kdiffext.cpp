/*
 * Copyright (c) 2007 Sergey Zorin. All rights reserved.
 *
 * This software is distributable under the BSD license. See the terms
 * of the BSD license in the COPYING file provided with this software.
 *
 */
#include <qlabel.h>
#include <kinstance.h>
#include <qstring.h>
#include <qwidget.h>
#include <kgenericfactory.h>
#include <kmessagebox.h>
#include <kapplication.h>

#include "kdiffext.h"
#include "kdiffext.moc"

typedef KGenericFactory<kdiffext, KonqPopupMenu> factory;
K_EXPORT_COMPONENT_FACTORY(konq_diffext, factory("kdiffext"))

QStringList kdiffext::_files;
    
kdiffext::kdiffext(KonqPopupMenu* popupmenu, const char* name, const QStringList &list) : KonqPopupMenuPlugin(popupmenu, name) {
  _config = new KConfig("kdiffextsetuprc"); //use setConfigName() in setup
  _menu = popupmenu;
  
  KFileItemList selected = _menu->fileItemList();
  int n = selected.count();
  
  addSeparator();
  
  if(n == 1) {
    if(_files.size() == 0) {
      addAction(new KAction(i18n("Compare later"), 0, this, SLOT(compare_later()), actionCollection(), "kdiffext::Compare later"));
    } else {
      addAction(new KAction(i18n("Compare to '%1'").arg(_files.first()), 0, this, SLOT(compare_to()), actionCollection(), "kdiffext::Compare to"));
      addAction(new KAction(i18n("Compare later"), 0, this, SLOT(compare_later()), actionCollection(), "kdiffext::Compare later"));

      addAction(new KAction("diff-ext::", 0, this, SLOT(compare_later()), actionCollection(), "kdiffext::"));
      _compare_to = new QPopupMenu(_menu, i18n("Compare to"));
      unsigned int n = 0;
      for(QStringList::iterator i = _files.begin(); i != _files.end(); i++, n++) {
        int id;
        id = _compare_to->insertItem(*i, this, SLOT(compare_to(int)));
        _compare_to->setItemParameter(id, n);
      }
    }
  } else if(n == 2) {
    if(_config->readBoolEntry("3-way-diff-enabled", false)) {
      if(_files.size() == 0) {
        addAction(new KAction(i18n("Compare"), 0, this, SLOT(compare()), actionCollection(), "kdiffext::Compare"));
        addAction(new KAction(i18n("Compare later"), 0, this, SLOT(compare_later()), actionCollection(), "kdiffext::Compare later"));
      } else {
        addAction(new KAction(i18n("3-way Compare to '%1'").arg(_files.first()), 0, this, SLOT(compare3_to()), actionCollection(), "kdiffext::3-way Compare to"));
        addAction(new KAction(i18n("Compare"), 0, this, SLOT(compare()), actionCollection(), "kdiffext::Compare"));
        addAction(new KAction(i18n("Compare later"), 0, this, SLOT(compare_later()), actionCollection(), "kdiffext::Compare later"));

        addAction(new KAction("diff-ext::3", 0, this, SLOT(compare_later()), actionCollection(), "kdiffext::"));
        _compare_to = new QPopupMenu(_menu, i18n("3-way compare to"));
        unsigned int n = 0;
        for(QStringList::iterator i = _files.begin(); i != _files.end(); i++, n++) {
          int id;
          id = _compare_to->insertItem(*i, this, SLOT(compare3_to(int)));
          _compare_to->setItemParameter(id, n);
        }
      }
    } else {
      addAction(new KAction(i18n("Compare"), 0, this, SLOT(compare()), actionCollection(), "kdiffext::Compare"));
      addAction(new KAction(i18n("Compare later"), 0, this, SLOT(compare_later()), actionCollection(), "kdiffext::Compare later"));
    }
  } else if(n == 3) {
    if(_config->readBoolEntry("3-way-diff-enabled", false)) {
      addAction(new KAction(i18n("3-way Compare"), 0, this, SLOT(compare3()), actionCollection(), "kdiffext::3-way Compare"));
      addAction(new KAction(i18n("Compare later"), 0, this, SLOT(compare_later()), actionCollection(), "kdiffext::Compare later"));
    } else {
      addAction(new KAction(i18n("Compare later"), 0, this, SLOT(compare_later()), actionCollection(), "kdiffext::Compare later"));
    }
  } else {
    addAction(new KAction(i18n("Compare later"), 0, this, SLOT(compare_later()), actionCollection(), "kdiffext::Compare later"));
  }
  
  addSeparator();
  
  connect(_menu, SIGNAL(aboutToShow()), this, SLOT(about_to_show()));
}

kdiffext::~kdiffext() {
  delete _config;
}

void 
kdiffext::compare_later() {
  KFileItem* file;
  KFileItemList selected = _menu->fileItemList();
  
  for(file=selected.first(); file; file=selected.next()) {
    QStringList::iterator i = _files.find(file->url().path());
    if(i != _files.end()) {
      _files.remove(i);
    }
    _files.push_front(file->url().path());
  }
}

void 
kdiffext::compare() {
  QStringList args;
  KFileItemList selected = _menu->fileItemList();
  args << selected.at(0)->url().path() << selected.at(1)->url().path();
  KApplication::kdeinitExec(_config->readEntry("diff-command"), args);
}

void 
kdiffext::compare3() {
  QStringList args;
  KFileItemList selected = _menu->fileItemList();
  args << selected.at(0)->url().path() << selected.at(1)->url().path() << selected.at(2)->url().path();
  KApplication::kdeinitExec(_config->readEntry("diff3-command"), args);
}

void 
kdiffext::compare_to(int n) {
  QStringList args;
  KFileItemList selected = _menu->fileItemList();
  args << selected.at(0)->url().path() << _files[n];
  KApplication::kdeinitExec(_config->readEntry("diff-command"), args);
}

void 
kdiffext::compare3_to(int n) {
  QStringList args;
  KFileItemList selected = _menu->fileItemList();
  args << selected.at(0)->url().path() << selected.at(1)->url().path() << _files[n];
  KApplication::kdeinitExec(_config->readEntry("diff3-command"), args);
}

void
kdiffext::about_to_show() {
  for(int i = _menu->count(); i >= 1; i--) {
    int id = _menu->idAt(i);
    QString text = _menu->text(id);
    if(text.contains("diff-ext::")) {
      _menu->removeItem(id);
      _menu->insertItem(i18n("Compare to"), _compare_to, -1, i);
      break;
    } else if(text.contains("diff-ext::3")) {
      _menu->removeItem(id);
      _menu->insertItem(i18n("3-way compare to"), _compare_to, -1, i);
      break;
    }
  }
}
