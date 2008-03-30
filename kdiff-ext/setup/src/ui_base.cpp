#include <kdialog.h>
#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './ui_base.ui'
**
** Created by User Interface Compiler
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "ui_base.h"

#include <qvariant.h>
#include <qcheckbox.h>
#include <kurlrequester.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "kurlrequester.h"
#include "klineedit.h"
#include "kpushbutton.h"

/*
 *  Constructs a UI_BASE as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
UI_BASE::UI_BASE( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "ui_base" );
    ui_baseLayout = new QGridLayout( this, 1, 1, 11, 6, "ui_baseLayout"); 
    spacer1_2_2_2 = new QSpacerItem( 166, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    ui_baseLayout->addMultiCell( spacer1_2_2_2, 1, 1, 1, 2 );

    _diff_compare_folders = new QCheckBox( this, "_diff_compare_folders" );

    ui_baseLayout->addMultiCellWidget( _diff_compare_folders, 2, 2, 0, 1 );

    _diff = new KURLRequester( this, "_diff" );
    _diff->setEnabled( TRUE );

    ui_baseLayout->addMultiCellWidget( _diff, 0, 0, 0, 2 );
    spacer11 = new QSpacerItem( 121, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    ui_baseLayout->addItem( spacer11, 2, 2 );

    _diff_use_kio = new QCheckBox( this, "_diff_use_kio" );

    ui_baseLayout->addWidget( _diff_use_kio, 1, 0 );
    spacer12 = new QSpacerItem( 20, 80, QSizePolicy::Minimum, QSizePolicy::Expanding );
    ui_baseLayout->addItem( spacer12, 3, 2 );
    languageChange();
    resize( QSize(309, 197).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // tab order
    setTabOrder( _diff, _diff_use_kio );
    setTabOrder( _diff_use_kio, _diff_compare_folders );
}

/*
 *  Destroys the object and frees any allocated resources
 */
UI_BASE::~UI_BASE()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void UI_BASE::languageChange()
{
    setCaption( tr2i18n( "ui_base" ) );
    _diff_compare_folders->setText( tr2i18n( "C&ompare folders" ) );
    _diff_compare_folders->setAccel( QKeySequence( tr2i18n( "Alt+O" ) ) );
    _diff_use_kio->setText( tr2i18n( "&Use KIO" ) );
    _diff_use_kio->setAccel( QKeySequence( tr2i18n( "Alt+U" ) ) );
}

#include "ui_base.moc"
