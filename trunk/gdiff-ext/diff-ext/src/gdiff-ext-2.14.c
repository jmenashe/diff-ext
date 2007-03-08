/*
 * Copyright (c) 2007, Sergey Zorin. All rights reserved.
 *
 * This software is distributable under the BSD license. See the terms
 * of the BSD license in the COPYING file provided with this software.
 *
 */
#include "gdiff-ext.h"

static GString* saved = 0;

void
compare_later(NautilusMenuItem *item, gpointer user_data) {
  GList* files = g_list_first((GList*)g_object_get_data(G_OBJECT(item), "diff-ext::save"));
  
  g_string_assign(saved, gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(files->data))));
}

void
compare_to(NautilusMenuItem *item, gpointer user_data) {
  GList* files = g_list_first((GList*)g_object_get_data(G_OBJECT(item), "diff-ext::compare_to"));
  gchar* f1 = gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(files->data)));
  gchar* f2 = saved->str;

  diff(f1, f2);
}

void
compare3_to(NautilusMenuItem *item, gpointer user_data) {
  GList* files = g_list_first((GList*)g_object_get_data(G_OBJECT(item), "diff-ext::compare3_to"));
  gchar* f1 = 0;
  gchar* f2 = 0;
  gchar* f3 = saved->str;

  f1 = gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(files->data)));
  files = g_list_next(files);
  f2 = gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(files->data)));

  diff3(f1, f2, f3);
}

void
nautilus_module_initialize(GTypeModule *module) {
  g_print("Initializing diff-ext\n");
  diff_ext_register_type(module);
  provider_types[0] = diff_ext_get_type();

  bindtextdomain(GETTEXT_PACKAGE, DIFF_EXT_LOCALE_DIR);
  bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
  
  saved = g_string_new("");
}

void 
nautilus_module_shutdown() {
  g_print("Shutting down diff-ext\n");
  g_string_free(saved, TRUE);
}

GList* 
get_file_items(NautilusMenuProvider *provider, GtkWidget *window, GList *files) {
  GList* items = NULL;
  
  g_return_val_if_fail(NAUTILUS_IS_MENU_PROVIDER(provider), NULL);
  g_return_val_if_fail(GTK_IS_WIDGET(window), NULL);

  if(files != NULL) {
    GList* scan = files;
    gboolean go = TRUE;
    guint i = 0;
    
    while(scan && (i < 3) && go) {
      gchar* scheme;

      scheme = nautilus_file_info_get_uri_scheme((NautilusFileInfo*)(scan->data));
      go = strncmp(scheme, "file", 4) == 0;
      g_free(scheme);
      
      scan = g_list_next(scan);
      i++;
    }
    
    if(g_list_next(scan) != NULL) {
      go = FALSE;
    }
        
    if(go) {
      guint n = g_list_length(files);
      GConfClient* gconf_client;
      gboolean enable_diff3;
      GString* icon = g_string_new("");
      gchar* tmp;
      gboolean enable_icons = FALSE;
      
      gconf_client = gconf_client_get_default ();
        enable_diff3 = gconf_client_get_bool(gconf_client, "/apps/diff-ext/enable-diff3", NULL);
        tmp = gconf_client_get_string(gconf_client, "/apps/diff-ext/icons", NULL); 
      g_object_unref(G_OBJECT(gconf_client));
      
      if(strlen(tmp) != 0) {
        enable_icons = TRUE;
      }

      if(n == 1) {
        NautilusMenuItem* save;
        
        if(enable_icons) {
          g_string_printf(icon, DIFF_EXT_DATA_DIR"/icons/%s/diff_later.png", tmp);
          save = nautilus_menu_item_new("diff-ext::save", _("Compare later"), _("Select file for comparison"), icon->str);
        } else {
          save = nautilus_menu_item_new("diff-ext::save", _("Compare later"), _("Select file for comparison"), NULL);
        }
        g_signal_connect(save, "activate", G_CALLBACK(compare_later), provider);
        g_object_set_data_full(G_OBJECT(save), "diff-ext::save", nautilus_file_info_list_copy(files), (GDestroyNotify)nautilus_file_info_list_free);
        items = g_list_append(items, save);

        if(strcmp("", saved->str) != 0) {
          GString* caption = g_string_new("");
          GString* hint = g_string_new("");
          NautilusMenuItem* compare_to_item;
          
          g_string_printf(caption, _("Compare to '%s'"), saved->str);
          g_string_printf(hint, _("Compare '%s' and '%s'"), gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(files->data))), saved->str);
          
          if(enable_icons) {
            g_string_printf(icon, DIFF_EXT_DATA_DIR"/icons/%s/diff_with.png", tmp);
            compare_to_item = nautilus_menu_item_new("diff-ext::compare_to", caption->str, hint->str, icon->str);
          } else {
            compare_to_item = nautilus_menu_item_new("diff-ext::compare_to", caption->str, hint->str, NULL);
          }
          g_signal_connect(compare_to_item, "activate", G_CALLBACK(compare_to), provider);
          g_object_set_data_full(G_OBJECT(compare_to_item), "diff-ext::compare_to", nautilus_file_info_list_copy(files), (GDestroyNotify)nautilus_file_info_list_free);
          items = g_list_append(items, compare_to_item);
          
          g_string_free(caption, TRUE);
          g_string_free(hint, TRUE);
        }
      } else if(n == 2) {    
        NautilusMenuItem* diff;
        
        if(enable_icons) {
          g_string_printf(icon, DIFF_EXT_DATA_DIR"/icons/%s/diff.png", tmp);
          diff = nautilus_menu_item_new("diff-ext::compare", _("Compare"), _("Compare selected files"), icon->str);
        } else {
          diff = nautilus_menu_item_new("diff-ext::compare", _("Compare"), _("Compare selected files"), NULL);
        }
        g_signal_connect(diff, "activate", G_CALLBACK(compare), provider);
        g_object_set_data_full(G_OBJECT(diff), "diff-ext::compare", nautilus_file_info_list_copy(files), (GDestroyNotify)nautilus_file_info_list_free);
        items = g_list_append(items, diff);
        
        if(enable_diff3) {
          if(strcmp("", saved->str) != 0) {
            GString* caption = g_string_new("");
            GString* hint = g_string_new("");
            NautilusMenuItem* compare3_to_item;
          
            g_string_printf(caption, _("3-way compare to '%s'"), saved->str);
            g_string_printf(hint, _("3-way compare '%s', '%s' and '%s'"), 
              gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(files->data))), 
              gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(g_list_next(files)->data))), 
              saved->str);
          
            if(enable_icons) {
              g_string_printf(icon, DIFF_EXT_DATA_DIR"/icons/%s/diff3_with.png", tmp);
              compare3_to_item = nautilus_menu_item_new("diff-ext::compare3_to", caption->str, hint->str, icon->str);
            } else {
              compare3_to_item = nautilus_menu_item_new("diff-ext::compare3_to", caption->str, hint->str, NULL);
            }
            g_signal_connect(compare3_to_item, "activate", G_CALLBACK(compare3_to), provider);
            g_object_set_data_full(G_OBJECT(compare3_to_item), "diff-ext::compare3_to", nautilus_file_info_list_copy(files), (GDestroyNotify)nautilus_file_info_list_free);
            items = g_list_append(items, compare3_to_item);
            
            g_string_free(caption, TRUE);
            g_string_free(hint, TRUE);
          }
        }
      } else if(n == 3) {
        if(enable_diff3) {
          NautilusMenuItem* diff3;
          
          if(enable_icons) {
            g_string_printf(icon, DIFF_EXT_DATA_DIR"/icons/%s/diff3.png", tmp);
            diff3 = nautilus_menu_item_new("diff-ext::compare3", _("3-way compare"), _("3-way compare selected files"), icon->str);
          } else {
            diff3 = nautilus_menu_item_new("diff-ext::compare3", _("3-way compare"), _("3-way compare selected files"), NULL);
          }
          g_signal_connect(diff3, "activate", G_CALLBACK(compare3), provider);
          g_object_set_data_full(G_OBJECT(diff3), "diff-ext::compare3", nautilus_file_info_list_copy(files), (GDestroyNotify)nautilus_file_info_list_free);
          items = g_list_append(items, diff3);
        }
      }
      
      g_free(tmp);
      g_string_free(icon, TRUE);
      g_object_unref(G_OBJECT(gconf_client));
    }
  }

  return items;
}
