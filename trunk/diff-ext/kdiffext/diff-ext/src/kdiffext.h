/*
 * Copyright (c) 2007 Sergey Zorin. All rights reserved.
 *
 * This software is distributable under the BSD license. See the terms
 * of the BSD license in the COPYING file provided with this software.
 *
 */
#ifndef KDIFFEXT_H
#define KDIFFEXT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <konq_popupmenu.h>
#include <qstring.h>
#include <kconfig.h>

class kdiffext : public KonqPopupMenuPlugin
{
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
    void about_to_show();
    
  private:
    KonqPopupMenu* _menu;
    QPopupMenu* _compare_to;
    KConfig* _config;
    static QStringList _files;
};

#endif
