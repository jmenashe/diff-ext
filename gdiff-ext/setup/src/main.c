/*
 * Copyright (c) 2007, Sergey Zorin. All rights reserved.
 *
 * This software is distributable under the BSD license. See the terms
 * of the BSD license in the COPYING file provided with this software.
 *
 */
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <gconf/gconf.h>
#include <gconf/gconf-client.h>

#include "interface.h"
#include "support.h"
#include "model.h"

gpointer model = 0;

void
apply() {
  GConfClient* gconf_client;
  MODEL* m = (MODEL*)model;
	
  gconf_client = gconf_client_get_default ();
	
    gconf_client_set_string(gconf_client, "/apps/gdiff-ext/diff", m->diff_tool->str, NULL);
    gconf_client_set_string(gconf_client, "/apps/gdiff-ext/diff3", m->diff3_tool->str, NULL);
    gconf_client_set_bool(gconf_client, "/apps/gdiff-ext/enable-diff3",m->enable_diff3, NULL);
    gconf_client_set_bool(gconf_client, "/apps/gdiff-ext/keep-files",m->keep_files, NULL);
	
  g_object_unref(G_OBJECT(gconf_client));
}

int
main (int argc, char *argv[])
{
  GtkWidget *main;
  GConfClient* gconf_client;
  MODEL m;

#ifdef ENABLE_NLS
  bindtextdomain (GETTEXT_PACKAGE, SETUP_LOCALE_DIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);
#endif

  gtk_set_locale ();
  gtk_init (&argc, &argv);
  gconf_init(argc, argv, 0);

  add_pixmap_directory (SETUP_DATA_DIR "/" PACKAGE_NAME "/pixmaps");
  add_pixmap_directory (SETUP_DATA_DIR "/" PACKAGE_NAME);

  m.diff_tool = g_string_new("");
  m.diff3_tool = g_string_new("");
  gconf_client = gconf_client_get_default ();
    g_string_assign(m.diff_tool, gconf_client_get_string(gconf_client, "/apps/gdiff-ext/diff", NULL));
    g_string_assign(m.diff3_tool, gconf_client_get_string(gconf_client, "/apps/gdiff-ext/diff3", NULL));
    m.enable_diff3 = gconf_client_get_bool(gconf_client, "/apps/gdiff-ext/enable-diff3", NULL);
    m.keep_files = gconf_client_get_bool(gconf_client, "/apps/gdiff-ext/keep-files", NULL);
  g_object_unref(G_OBJECT(gconf_client));
  
  if(!g_path_is_absolute(m.diff_tool->str)) {
    g_string_assign(m.diff_tool, g_find_program_in_path(m.diff_tool->str));
  }
  
  if(!g_path_is_absolute(m.diff3_tool->str)) {
    g_string_assign(m.diff3_tool, g_find_program_in_path(m.diff3_tool->str));
  }
  
  model = &m;
  /*
   * The following code was added by Glade to create one of each component
   * (except popup menus), just so that you see something after building
   * the project. Delete any components that you don't want shown initially.
   */
  main = create_main ();
  gtk_widget_show (main);

  gtk_main ();
  return 0;
}

