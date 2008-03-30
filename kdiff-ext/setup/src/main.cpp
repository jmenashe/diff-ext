/*
 * Copyright (c) 2007, Sergey Zorin. All rights reserved.
 *
 * This software is distributable under the BSD license. See the terms
 * of the BSD license in the COPYING file provided with this software.
 *
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kconfigdialog.h>
#include <kaboutapplication.h>

#include <settings.h>
#include "diff.h"
#include "diff3.h"
#include "general.h"
#include "main.h"
    
static const char description[] = I18N_NOOP("diff-ext context menu extension for Konqueror");

static const char version[] = "0.3.1";

static KCmdLineOptions options[] = {
//    { "+[URL]", I18N_NOOP( "Document to open" ), 0 },
  KCmdLineLastOption
};

int 
main(int argc, char **argv) {
  KLocale::setMainCatalogue("kdiffext");

  KAboutData about("kdiffextsetup", 
                   I18N_NOOP("kdiffextsetup"), 
                   version, description,
                   KAboutData::License_Custom, 
                   I18N_NOOP("(c) 2007 Sergey Zorin\n"
                   "All rights reserved."),
                   0, "http://diff-ext.sourceforge.net", "szorin@comcast.net");
  about.addAuthor( "Sergey Zorin", 0, "szorin@comcast.net" );
  about.setLicenseText(I18N_NOOP("Copyright (c) 2007 Sergey Zorin\n"
      "All rights reserved.\n\n"
      "Redistribution and use in source and binary forms, with or without\n"
      "modification, are permitted provided that the following conditions\n"
      "are met:\n"
      "   1. Redistributions  of  source  code   must  retain  the  above\n"
      "      copyright notice,  this list of conditions and the following\n"
      "      disclaimer.\n"
      "   2. Redistributions in binary  form  must  reproduce  the  above\n"
      "      copyright notice,  this list of conditions and the following\n"
      "      disclaimer  in  the  documentation  and/or  other  materials\n"
      "      provided with the distribution.\n\n"
      "THIS SOFTWARE  IS  PROVIDED  BY  THE  COPYRIGHT  HOLDERS  AND  CONTRIBUTORS\n"
      "\"AS IS\" AND ANY EXPRESS OR IMPLIED  WARRANTIES, INCLUDING,  BUT NOT LIMITED\n"
      "TO, THE IMPLIED WARRANTIES  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR\n"
      "PURPOSE  ARE  DISCLAIMED.  IN  NO  EVENT  SHALL  THE  COPYRIGHT   OWNER  OR\n"
      "CONTRIBUTORS  BE  LIABLE  FOR  ANY DIRECT,  INDIRECT, INCIDENTAL,  SPECIAL,\n"
      "EXEMPLARY,  OR  CONSEQUENTIAL  DAMAGES  (INCLUDING,  BUT  NOT  LIMITED  TO,\n"
      "PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;\n"
      "OR BUSINESS INTERRUPTION)  HOWEVER CAUSED AND ON ANY THEORY  OF  LIABILITY,\n"
      "WHETHER IN CONTRACT,  STRICT LIABILITY,  OR  TORT  (INCLUDING NEGLIGENCE OR\n"
      "OTHERWISE) ARISING  IN  ANY WAY OUT OF THE USE  OF THIS  SOFTWARE,  EVEN IF\n"
      "ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n"));
  KCmdLineArgs::init(argc, argv, &about);
  KCmdLineArgs::addCmdLineOptions(options);
  KDIFF_EXT_SETUP app;

  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
  KConfigDialog* mainWin = new KConfigDialog(0, "settings", Settings::self());

  mainWin->addPage(new GENERAL(), i18n("General"), "kdiffextsetup.png");
  mainWin->addPage(new DIFF(), i18n("Compare tool"), "package_settings.png");
  mainWin->addPage(new DIFF3(), i18n("3-way compare tool"), "package_settings.png");

//  mainWin->connect(mainWin, SIGNAL(helpClicked()), &app, SLOT(about_clicked()));

  app.setMainWidget(mainWin);

  mainWin->show();

  args->clear();

  // mainWin has WDestructiveClose flag by default, so it will delete itself.
  return app.exec();
}
