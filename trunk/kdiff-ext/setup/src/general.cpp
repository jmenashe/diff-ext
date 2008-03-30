#include <kdialog.h>
#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './general.ui'
**
** Created by User Interface Compiler
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "general.h"

#include <qvariant.h>
#include <knuminput.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "knuminput.h"

/*
 *  Constructs a GENERAL as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
GENERAL::GENERAL( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "GENERAL" );
    GENERALLayout = new QGridLayout( this, 1, 1, 11, 6, "GENERALLayout"); 
    spacer17 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    GENERALLayout->addItem( spacer17, 0, 1 );
    spacer3_2_2 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    GENERALLayout->addItem( spacer3_2_2, 1, 1 );

    kcfg_max_filename_length = new KIntNumInput( this, "kcfg_max_filename_length" );
    kcfg_max_filename_length->setValue( 63 );
    kcfg_max_filename_length->setMinValue( 20 );
    kcfg_max_filename_length->setMaxValue( 1024 );

    GENERALLayout->addWidget( kcfg_max_filename_length, 0, 0 );
    languageChange();
    resize( QSize(236, 97).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
GENERAL::~GENERAL()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void GENERAL::languageChange()
{
    setCaption( tr2i18n( "Form1" ) );
    kcfg_max_filename_length->setLabel( tr2i18n( "S&queeze filenames longer then:" ) );
    kcfg_max_filename_length->setSpecialValueText( tr2i18n( "diasbled" ) );
}

#include "general.moc"
