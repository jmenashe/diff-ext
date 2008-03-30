/*
 * Copyright (c) 2007 Sergey Zorin. All rights reserved.
 *
 * This software is distributable under the BSD license. See the terms
 * of the BSD license in the COPYING file provided with this software.
 *
 */
#ifndef __KDIFFEXT_H__
#define __KDIFFEXT_H__

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qstring.h>
#include <qsignalmapper.h>
#include <konq_popupmenu.h>
#include <kconfig.h>
#include <kfileitem.h>

class kdiffext : public KonqPopupMenuPlugin {
  Q_OBJECT
  public:
    kdiffext(KonqPopupMenu* popupmenu, const char* name, const QStringList &list);
    ~kdiffext();

  public slots:
    void compare_later();
    void compare();
    void compare3();
    void compare_to(int n = 0);
    void compare3_to(int n = 0);
    void clear();

  protected:
    virtual void setup_actions();
    QString arg(KFileItem*, bool);
    QString display(KFileItem*);

  protected:
    KonqPopupMenu* _menu;
    QSignalMapper* _mapper;
    static KFileItemList _files;
};

#endif //__KDIFFEXT_H__
