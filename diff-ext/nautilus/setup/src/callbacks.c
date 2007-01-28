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

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "model.h"

void
on_ok_clicked                          (GtkButton       *button,
                                        gpointer         user_data)
{
  apply();
  gtk_main_quit();
}


void
on_apply_clicked                       (GtkButton       *button,
                                        gpointer         user_data)
{
  apply();
}


void
on_about_clicked                       (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_show(create_about()); 
}


void
on_3_way_compare_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  MODEL* m = (MODEL*)model;
  m->enable_diff3 = gtk_toggle_button_get_active(togglebutton);
}


void
on_diff_tool_selection_changed         (GtkFileChooser  *filechooser,
                                        gpointer         user_data)
{
  MODEL* m = (MODEL*)model;
  gchar* name = gtk_file_chooser_get_filename(filechooser);
  if(name != NULL) {
    g_string_assign(m->diff_tool, name);
  }
}


void
on_diff3_tool_selection_changed        (GtkFileChooser  *filechooser,
                                        gpointer         user_data)
{
  MODEL* m = (MODEL*)model;
  gchar* name = gtk_file_chooser_get_filename(filechooser);
  if(name != NULL) {
    g_string_assign(m->diff3_tool, name);
  }
}

static void
set_filter(GtkFileChooser* filechooser) {
  GtkFileFilter* filter = gtk_file_filter_new();
  gtk_file_filter_add_mime_type(filter, "application/x-executable");
  gtk_file_chooser_set_filter(filechooser, filter);
}

void
on_diff_tool_realize                   (GtkWidget       *widget,
                                        gpointer         user_data)
{
  MODEL* m = (MODEL*)model;
  set_filter((GtkFileChooser*)widget);
  gtk_file_chooser_set_filename((GtkFileChooser*)widget, m->diff_tool->str);
}

void
on_diff3_tool_realize                   (GtkWidget       *widget,
                                        gpointer         user_data)
{
  MODEL* m = (MODEL*)model;
  set_filter((GtkFileChooser*)widget);
  gtk_file_chooser_set_filename((GtkFileChooser*)widget, m->diff3_tool->str);
}

void
on_3_way_compare_realize               (GtkWidget       *widget,
                                        gpointer         user_data)
{
  MODEL* m = (MODEL*)model;
  gtk_toggle_button_set_active((GtkToggleButton*)widget, m->enable_diff3);
}
