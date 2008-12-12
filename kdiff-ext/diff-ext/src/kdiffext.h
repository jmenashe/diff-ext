/*
 * Copyright (c) 2007-2008 Sergey Zorin. All rights reserved.
 *
 * This software is distributable under the BSD license. See the terms
 * of the BSD license in the COPYING file provided with this software.
 *
 */
#ifndef __KDIFFEXT_H__
#define __KDIFFEXT_H__

#include <Qt/qstring.h>
#include <Qt/qsignalmapper.h>
#include <konq_popupmenu.h>
#include <konq_popupmenuinformation.h>
#include <konq_popupmenuplugin.h>
#include <kconfig.h>
#include <kfileitem.h>

class kdiffext : public KonqPopupMenuPlugin {
  Q_OBJECT
  public:
    kdiffext(QObject*, const QStringList&);
    virtual ~kdiffext();

    virtual void setup(KActionCollection* actionCollection,
                       const KonqPopupMenuInformation& popupMenuInfo,
                       QMenu *menu);

  public slots:
    void compare_later();
    void compare();
    void compare3();
    void compare_to(int n = 0);
    void compare3_to(int n = 0);
    void clear();

  protected:
    QString arg(const KFileItem&, bool);
    QString display(const KFileItem&);
    void populate_compare_to_menu(KActionMenu*, KActionCollection*);

  protected:
    QSignalMapper* _mapper;
    KFileItemList _selected;
    static KFileItemList _files;
};

#endif //__KDIFFEXT_H__
