/*
 * Copyright (c) 2008-2009, Sergey Zorin. All rights reserved.
 *
 * This software is distributable under the BSD license. See the terms
 * of the BSD license in the COPYING file provided with this software.
 *
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#include <glib/gqueue.h>
#include <exo/exo.h>

#include <thunarx/thunarx.h>
#include <thunar-vfs/thunar-vfs.h>

#include <preferences.h>

#include "submenu-action.h"

static GType type_list[1];

static GQueue* _saved = 0;

typedef struct _DIFF_EXT {
  GObject object;
} DIFF_EXT;

typedef struct _DIFF_EXT_CLASS {
  GObjectClass parent_class;
} DIFF_EXT_CLASS, DIFF_EXTClass;
  

static void menu_provider_init(ThunarxMenuProviderIface *iface);
static GList* get_file_actions(ThunarxMenuProvider* provider, GtkWidget* window, GList* files);
static GList *get_folder_actions(ThunarxMenuProvider* provider, GtkWidget* window, ThunarxFileInfo* folder);

THUNARX_DEFINE_TYPE_WITH_CODE(DIFF_EXT,
                               diff_ext,
                               G_TYPE_OBJECT,
                               THUNARX_IMPLEMENT_INTERFACE(THUNARX_TYPE_MENU_PROVIDER,
                                                            menu_provider_init));



void
diff(gchar* f1, gchar* f2) {
  gchar* argv[5];
  gchar* diff_tool = "";
  xdiff_ext_preferences* p = xdiff_ext_preferences_instance();
  
  g_object_get(G_OBJECT(p), "compare-command", &diff_tool, NULL);
  
  g_object_unref(p);
  
  argv[0] = diff_tool;
  argv[1] = diff_tool;
  argv[2] = f1;
  argv[3] = f2;
  argv[4] = 0;
  
  g_spawn_async(0, argv, 0, G_SPAWN_FILE_AND_ARGV_ZERO | G_SPAWN_SEARCH_PATH, 0, 0, 0, 0);
  // handle spawn errors
  g_free(diff_tool);
}

void
diff3(gchar* f1, gchar* f2, gchar* f3) {
  gchar* argv[6];
  gchar* diff_tool;
  xdiff_ext_preferences* p = xdiff_ext_preferences_instance();
  
  g_object_get(G_OBJECT(p), "three-way-compare-command", &diff_tool, NULL);
  
  g_object_unref(p);

  argv[0] = diff_tool;
  argv[1] = diff_tool;
  argv[2] = f1;
  argv[3] = f2;
  argv[4] = f3;
  argv[5] = 0;
  
  g_spawn_async(0, argv, 0, G_SPAWN_FILE_AND_ARGV_ZERO | G_SPAWN_SEARCH_PATH, 0, 0, 0, 0);
// handle spawn errors
  g_free(diff_tool);
}

void
clear_queue(GQueue* queue) {
  while(!g_queue_is_empty(queue)) {
    g_queue_pop_head(queue);
  }
}

void
compare_later(GtkAction* action, GtkWidget* window) {
  GList* files = g_list_first((GList*)g_object_get_data(G_OBJECT(action), "xdiff-ext::save"));
    
  while(files) {
    gchar* uri;
    gchar* path;
    GList* link;

    uri = thunarx_file_info_get_uri((ThunarxFileInfo*)files->data);
    path = g_filename_from_uri(uri, NULL, NULL);
    g_free(uri);
    
    link = g_queue_find_custom(_saved, path, (GCompareFunc)strcmp);
    
    if(link == NULL) {
      g_queue_push_head(_saved, path);
    } else {
      g_queue_unlink(_saved, link);
      g_queue_push_head_link(_saved, link);
    }
    // g_free(path) ???
    files = g_list_next(files);
  }
}

void
compare_to(GtkAction* action, GtkWidget* window) {
  GList* files = g_list_first((GList*)g_object_get_data(G_OBJECT(action), "xdiff-ext::compare_to"));
  GList* saved = (GList*)g_object_get_data(G_OBJECT(action), "xdiff-ext::saved");
  gchar* f1 = 0;
  gchar* f2 = (gchar*)saved->data;
  gboolean keep_file = FALSE;
  gchar* uri;
  xdiff_ext_preferences* p = xdiff_ext_preferences_instance();
  
  g_object_get(G_OBJECT(p), "keep-files-in-list", &keep_file, NULL);
  
  g_object_unref(p);

  uri = thunarx_file_info_get_uri((ThunarxFileInfo*)files->data);
  f1 = g_filename_from_uri(uri, NULL, NULL);
  g_free(uri);

  diff(f1, f2);

  g_free(f1);
  
  if(!keep_file) {
    clear_queue(_saved);
  }
}

void
compare3_to(GtkAction* action, GtkWidget* window) {
  GList* files = g_list_first((GList*)g_object_get_data(G_OBJECT(action), "xdiff-ext::compare_to"));
  GList* saved = (GList*)g_object_get_data(G_OBJECT(action), "xdiff-ext::saved");
  gchar* f1 = 0;
  gchar* f2 = 0;
  gchar* f3 = (gchar*)saved->data;
  gboolean keep_file = FALSE;
  gchar* uri;
  xdiff_ext_preferences* p = xdiff_ext_preferences_instance();
  
  g_object_get(G_OBJECT(p), "keep-files-in-list", &keep_file, NULL);
  
  g_object_unref(p);

  uri = thunarx_file_info_get_uri((ThunarxFileInfo*)files->data);
  f1 = g_filename_from_uri(uri, NULL, NULL);
  g_free(uri);
  files = g_list_next(files);
  uri = thunarx_file_info_get_uri((ThunarxFileInfo*)files->data);
  f2 = g_filename_from_uri(uri, NULL, NULL);
  g_free(uri);

  diff3(f1, f2, f3);

  g_free(f1);
  g_free(f2);
  
  if(!keep_file) {
    clear_queue(_saved);
  }
}


void
compare(GtkAction* action, GtkWidget* window) {
  GList* files = g_list_first((GList*)g_object_get_data(G_OBJECT(action), "xdiff-ext::compare"));
  gchar* f1 = 0;
  gchar* f2 = 0;
  gchar* uri;

  uri = thunarx_file_info_get_uri((ThunarxFileInfo*)files->data);
  f1 = g_filename_from_uri(uri, NULL, NULL);
  g_free(uri);
  files = g_list_next(files);
  uri = thunarx_file_info_get_uri((ThunarxFileInfo*)files->data);
  f2 = g_filename_from_uri(uri, NULL, NULL);
  g_free(uri);

  diff(f1, f2);
  
  g_free(f1);
  g_free(f2);
}

void
compare3(GtkAction* action, GtkWidget* window) {
  GList* files = g_list_first((GList*)g_object_get_data(G_OBJECT(action), "xdiff-ext::compare3"));
  gchar* f1 = 0;
  gchar* f2 = 0;
  gchar* f3 = 0;
  gchar* uri;

  uri = thunarx_file_info_get_uri((ThunarxFileInfo*)files->data);
  f1 = g_filename_from_uri(uri, NULL, NULL);
  g_free(uri);
  files = g_list_next(files);
  uri = thunarx_file_info_get_uri((ThunarxFileInfo*)files->data);
  f2 = g_filename_from_uri(uri, NULL, NULL);
  g_free(uri);
  files = g_list_next(files);
  uri = thunarx_file_info_get_uri((ThunarxFileInfo*)files->data);
  f3 = g_filename_from_uri(uri, NULL, NULL);
  g_free(uri);

  diff3(f1, f2, f3);
  g_free(f1);
  g_free(f2);
  g_free(f3);
}


void
clear(GtkAction* action, GtkWidget* window) {
  clear_queue(_saved);
}

static void
diff_ext_class_init(DIFF_EXT_CLASS* klass) {
  /* nothing to do here */
}



static void
diff_ext_init(DIFF_EXT* open_terminal) {
  /* nothing to do here */
}



static void
menu_provider_init(ThunarxMenuProviderIface* iface) {
  iface->get_file_actions = get_file_actions;
  iface->get_folder_actions = get_folder_actions;
  /* get a reference of iface here? */
  g_object_ref(iface);
}

static void
add_compare_to_menu(GList* actions, GtkWidget* window, GList* files, char* menu_name, GString* (*make_hint)(va_list, char*), GCallback callback, ...) {
  GtkAction* action;    
  GtkIconTheme* theme = gtk_icon_theme_get_default();
  GList* head = g_queue_peek_head_link(_saved);
  xdiff_ext_submenu_action* submenu;
  GString* name = g_string_new("");
  int n = 1;
  va_list args;
  
  va_start(args, callback);
  
  submenu = xdiff_ext_submenu_action_new("xdiff-ext::compare_to_menu", menu_name, "", NULL);
  actions = g_list_append(actions, submenu);

  while(head) {
    gchar* head_file = (gchar*)head->data;    
    ThunarVfsPath* vfs_path = thunar_vfs_path_new(head_file, NULL);
    ThunarVfsInfo* vfs_info = thunar_vfs_info_new_for_path(vfs_path, NULL);
    const gchar* icon_name = thunar_vfs_info_get_custom_icon(vfs_info);
    GString* hint = make_hint(args, head_file);
    
    if(icon_name == NULL) {
      icon_name = thunar_vfs_mime_info_lookup_icon_name(vfs_info->mime_info, theme);
    }
    
    thunar_vfs_path_unref(vfs_path);
    thunar_vfs_info_unref(vfs_info);
    
    g_string_printf(name, "xdiff-ext::compare_to_%d", n);
    
    action = gtk_action_new(name->str, head_file, hint->str, icon_name);
    g_signal_connect(G_OBJECT(action), "activate", callback, window);
    g_object_set_data_full(G_OBJECT(action), "xdiff-ext::compare_to", thunarx_file_info_list_copy(files),(GDestroyNotify)thunarx_file_info_list_free);
    g_object_set_data(G_OBJECT(action), "xdiff-ext::saved", head);
    xdiff_ext_submenu_action_add(submenu, action);
    
    g_string_free(hint, TRUE);

    head = g_list_next(head);
    n++;
  }
  
  xdiff_ext_submenu_action_add(submenu, NULL);
  
  action = gtk_action_new("xdiff-ext::clear", _("Clear"), _("Clear selected files list"), GTK_STOCK_CLEAR);
  g_signal_connect(G_OBJECT(action), "activate", G_CALLBACK(clear), window);
  xdiff_ext_submenu_action_add(submenu, action);
  
  g_string_free(name, TRUE);
  va_end(args);
}

static GString* 
make_hint(va_list args, char* path2) {
  char* path = va_arg(args, char*);
  GString* hint = g_string_new("");
  
  g_string_printf(hint, _("Compare '%s' and '%s'"), path, path2);
  
  return hint;
}

static GString* 
make_hint3(va_list args, char* path3) {
  char* path1 = va_arg(args, char*);
  char* path2 = va_arg(args, char*);
  GString* hint = g_string_new("");
  
  g_string_printf(hint, _("3-way compare '%s', '%s' and '%s'"), path1, path2, path3);
  
  return hint;
}

static GList*
get_file_actions(ThunarxMenuProvider* provider, GtkWidget* window, GList* files) {
  GList* actions = 0;
  
  if(files != 0) {
    GtkIconTheme* theme = gtk_icon_theme_get_default();
    guint n = g_list_length(files);
    gchar* three_way_compare_command;
    xdiff_ext_preferences* p = xdiff_ext_preferences_instance();

    g_object_get(G_OBJECT(p), "three-way-compare-command", &three_way_compare_command, NULL);

    g_object_unref(p);
     
    GList* scan = files;
    gboolean go = TRUE;
    
    while(scan && go) {
      gchar* scheme;

      scheme = thunarx_file_info_get_uri_scheme((ThunarxFileInfo*)(scan->data));
      go = strncmp(scheme, "file", 4) == 0;
      g_free(scheme);
      
      scan = g_list_next(scan);
    }
    
    if(go) {
      GtkAction* action = gtk_action_new("xdiff-ext::save", _("Compare later"), _("Select file for comparison"), DIFF_EXT_DATA_DIR"/icons/hicolor/16x16/actions/diff_later.png");
      g_signal_connect(G_OBJECT(action), "activate", G_CALLBACK(compare_later), window);
      g_object_set_data_full(G_OBJECT(action), "xdiff-ext::save", thunarx_file_info_list_copy(files),(GDestroyNotify)thunarx_file_info_list_free);
      actions = g_list_append(actions, action);
      
      if(n == 1) {
        if(!g_queue_is_empty(_saved)) {
          GString* caption = g_string_new("");
          GString* hint = g_string_new("");
          GList* head = g_queue_peek_head_link(_saved);
          gchar* head_file = (gchar*)head->data;
          ThunarVfsInfo* vfs_info = NULL;
          ThunarVfsPath* vfs_path = NULL;
          const gchar* icon_name;
          
          gchar* uri;
          gchar* path;

          uri = thunarx_file_info_get_uri((ThunarxFileInfo*)files->data);
          path = g_filename_from_uri(uri, NULL, NULL);
          g_free(uri);
          
          g_string_printf(caption,_("Compare to '%s'"), head_file);
          g_string_printf(hint, _("Compare '%s' and '%s'"), path, head_file);
          
          vfs_path = thunar_vfs_path_new(head_file, NULL);
          vfs_info = thunar_vfs_info_new_for_path(vfs_path, NULL);
          icon_name = thunar_vfs_info_get_custom_icon(vfs_info);
          if(icon_name == NULL) {
            icon_name = thunar_vfs_mime_info_lookup_icon_name(vfs_info->mime_info, theme);
          }
          
          g_message("icon name: '%s'", icon_name);
          thunar_vfs_path_unref(vfs_path);
          thunar_vfs_info_unref(vfs_info);
          
          action = gtk_action_new("xdiff-ext::compare_to", caption->str, hint->str, "gaim.png");
          g_signal_connect(G_OBJECT(action), "activate", G_CALLBACK(compare_to), window);
          g_object_set_data_full(G_OBJECT(action), "xdiff-ext::compare_to", thunarx_file_info_list_copy(files),(GDestroyNotify)thunarx_file_info_list_free);
          g_object_set_data(G_OBJECT(action), "xdiff-ext::saved", head);
          actions = g_list_append(actions, action);
          
          if(_saved->length > 1) {
            add_compare_to_menu(actions, window, files, _("Compare to"), make_hint, G_CALLBACK(compare_to), path);
          }
          
          g_string_free(caption, TRUE);
          g_string_free(hint, TRUE);
        }
      } else if(n == 2) {
        GtkAction* action = gtk_action_new("xdiff-ext::compare", _("Compare"), _("Compare selected files"), "diff");
        g_signal_connect(G_OBJECT(action), "activate", G_CALLBACK(compare), window);
        g_object_set_data_full(G_OBJECT(action), "xdiff-ext::compare", thunarx_file_info_list_copy(files),(GDestroyNotify)thunarx_file_info_list_free);
        actions = g_list_append(actions, action);

        if(strcmp("", three_way_compare_command) != 0) {
          if(!g_queue_is_empty(_saved)) {
            GString* caption = g_string_new("");
            GString* hint = g_string_new("");
            GList* head = g_queue_peek_head_link(_saved);
            gchar* head_file = (gchar*)head->data;
            gchar* uri;
            gchar* path1;
            gchar* path2;

            uri = thunarx_file_info_get_uri((ThunarxFileInfo*)files->data);
            path1 = g_filename_from_uri(uri, NULL, NULL);
            g_free(uri);
            files = g_list_next(files);
            uri = thunarx_file_info_get_uri((ThunarxFileInfo*)files->data);
            path2 = g_filename_from_uri(uri, NULL, NULL);
            g_free(uri);
            
            g_string_printf(caption, _("3-way compare to '%s'"), head_file);
            g_string_printf(hint, _("3-way compare '%s', '%s' and '%s'"), path1, path2, head_file);
            
            action = gtk_action_new("xdiff-ext::compare_to", caption->str, hint->str, "diff3_with");
            g_signal_connect(G_OBJECT(action), "activate", G_CALLBACK(compare3_to), window);
            g_object_set_data_full(G_OBJECT(action), "xdiff-ext::compare_to", thunarx_file_info_list_copy(files),(GDestroyNotify)thunarx_file_info_list_free);
            g_object_set_data(G_OBJECT(action), "xdiff-ext::saved", head);
            actions = g_list_append(actions, action);

            if(_saved->length > 1) {
              add_compare_to_menu(actions, window, files, _("3-way compare to"), make_hint3, G_CALLBACK(compare3_to), path1, path2);
            }
            
            g_string_free(caption, TRUE);
            g_string_free(hint, TRUE);
          }
        }
      } else if(n == 3) {
        if(strcmp("", three_way_compare_command) != 0) {
          GtkAction* action = gtk_action_new("xdiff-ext::compare3", _("3-way Compare"), _("Compare selected files"), "diff3");
          g_signal_connect(G_OBJECT(action), "activate", G_CALLBACK(compare3), window);
          g_object_set_data_full(G_OBJECT(action), "xdiff-ext::compare3", thunarx_file_info_list_copy(files),(GDestroyNotify)thunarx_file_info_list_free);
          actions = g_list_append(actions, action);
        }
      }
    }
    
    g_free(three_way_compare_command);
  }
          
  return actions;
}



static GList*
get_folder_actions(ThunarxMenuProvider* provider, GtkWidget* window, ThunarxFileInfo* folder) {
  GList* actions = 0;
  gchar* scheme;

  scheme = thunarx_file_info_get_uri_scheme(folder);
  if(strncmp(scheme, "file", 4) == 0) {
    GList* files = g_list_append(0, folder);
    GtkAction* action = gtk_action_new("xdiff-ext::save", _("Compare later"), _("Select file for comparison"), "diff_later");
    g_signal_connect(G_OBJECT(action), "activate", G_CALLBACK(compare_later), window);
    g_object_set_data_full(G_OBJECT(action), "xdiff-ext::save", thunarx_file_info_list_copy(files), (GDestroyNotify)thunarx_file_info_list_free);
    actions = g_list_append(actions, action);
    
    if(!g_queue_is_empty(_saved)) {
      GString* caption = g_string_new("");
      GString* hint = g_string_new("");
      GList* head = g_queue_peek_head_link(_saved);
      gchar* head_file = (gchar*)head->data;
      gchar* uri;
      gchar* path;

      uri = thunarx_file_info_get_uri((ThunarxFileInfo*)files->data);
      path = g_filename_from_uri(uri, NULL, NULL);
      g_free(uri);
      
      g_string_printf(caption, _("Compare to '%s'"), head_file);
      g_string_printf(hint, _("Compare '%s' and '%s'"), path, head_file);
      
      action = gtk_action_new("xdiff-ext::compare_to", caption->str, hint->str, "diff_with");
      g_signal_connect(G_OBJECT(action), "activate", G_CALLBACK(compare_to), window);
      g_object_set_data_full(G_OBJECT(action), "xdiff-ext::compare_to", thunarx_file_info_list_copy(files), (GDestroyNotify)thunarx_file_info_list_free);
      g_object_set_data(G_OBJECT(action), "xdiff-ext::saved", head);
      actions = g_list_append(actions, action);
      
      g_string_free(caption, TRUE);
      g_string_free(hint, TRUE);
    }
    
    g_list_free(files);
  }

  g_free(scheme);

  return actions;
}




G_MODULE_EXPORT void
thunar_extension_initialize(ThunarxProviderPlugin* plugin) {
  const gchar* mismatch;

  /* verify that the thunarx versions are compatible */
  mismatch = thunarx_check_version(THUNARX_MAJOR_VERSION, THUNARX_MINOR_VERSION, THUNARX_MICRO_VERSION);
  
  if(G_LIKELY(mismatch == NULL)) {
    g_message("Initializing xdiff-ext extension; DIFF_EXT_DATA_DIR='%s'", DIFF_EXT_DATA_DIR);
    
    {
      int n;
      char** path;
      int i;
      
      gtk_icon_theme_get_search_path(gtk_icon_theme_get_default(), &path, &n);
      
      for(i = 0; i < n; i++) {
        g_message("icon search path: '%s'", path[i]);
      }
    }
    
#ifdef ENABLE_NLS
    bindtextdomain(GETTEXT_PACKAGE, DIFF_EXT_LOCALE_DIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
#endif /* ENABLE_NLS */

    /* register the types provided by this plugin */
    diff_ext_register_type(plugin);

    /* setup the plugin type list */
    type_list[0] = diff_ext_get_type();
    _saved = g_queue_new();

    thunarx_provider_plugin_set_resident(plugin, TRUE);
  } else {
    g_warning("Version mismatch: %s", mismatch);
  }
}



G_MODULE_EXPORT void
thunar_extension_shutdown() {
  g_message("Shutting down xdiff-ext extension");
}



G_MODULE_EXPORT void
thunar_extension_list_types(const GType** types, gint* n_types) {
  *types = type_list;
  *n_types = G_N_ELEMENTS(type_list);
}
