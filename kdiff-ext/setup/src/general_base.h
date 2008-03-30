/****************************************************************************
** Form interface generated from reading ui file '/home/serg/proj/tmp2/kdiffext/setup/src/general_base.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef GENERAL_BASE_H
#define GENERAL_BASE_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class KIntNumInput;

class GENERAL_BASE : public QWidget
{
    Q_OBJECT

public:
    GENERAL_BASE( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~GENERAL_BASE();

    KIntNumInput* kcfg_max_filename_length;

protected:
    QGridLayout* GENERAL_BASELayout;
    QSpacerItem* spacer17;
    QSpacerItem* spacer3_2_2;

protected slots:
    virtual void languageChange();

};

#endif // GENERAL_BASE_H
