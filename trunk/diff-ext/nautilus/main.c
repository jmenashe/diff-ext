#include <string.h>
#include <gtk/gtk.h>
#include <glib-object.h>
#include <glib/gi18n-lib.h>
#include <gconf/gconf.h>
#include <gconf/gconf-client.h>
#include <libgnomevfs/gnome-vfs-utils.h>
#include <libnautilus-extension/nautilus-info-provider.h>
#include <libnautilus-extension/nautilus-menu-provider.h>

typedef struct _DIFF_EXT {
  GObject object;
} DIFF_EXT;

typedef struct _DIFF_EXT_CLASS {
  GObjectClass parent_class;
} DIFF_EXT_CLASS;

static GType provider_types[1];
static GType diff_ext_type;

static GString* saved = 0;

static void
compare_later(NautilusMenuItem *item, gpointer user_data) {
  GList* files = g_list_first((GList*)g_object_get_data(G_OBJECT(item), "diff-ext::save"));
  
  g_string_assign(saved, gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(files->data))));
}

static void
diff(gchar* f1, gchar* f2) {
  gchar* argv[5];
  gchar* diff_tool = 0;
  GConfClient* gconf_client;

  gconf_client = gconf_client_get_default ();
    diff_tool = gconf_client_get_string(gconf_client, "/apps/diff-ext/diff", NULL);
  g_object_unref(G_OBJECT(gconf_client));
  
  argv[0] = diff_tool;
  argv[1] = diff_tool;
  argv[2] = f1;
  argv[3] = f2;
  argv[4] = 0;
  
  g_spawn_async(0, argv, 0, G_SPAWN_FILE_AND_ARGV_ZERO | G_SPAWN_SEARCH_PATH, 0, 0, 0, 0);
}

static void
diff3(gchar* f1, gchar* f2, gchar* f3) {
  gchar* argv[6];
  gchar* diff3_tool = 0;
  GConfClient* gconf_client;

  gconf_client = gconf_client_get_default ();
    diff3_tool = gconf_client_get_string(gconf_client, "/apps/diff-ext/diff3", NULL);
  g_object_unref(G_OBJECT(gconf_client));
  
  argv[0] = diff3_tool;
  argv[1] = diff3_tool;
  argv[2] = f1;
  argv[3] = f2;
  argv[4] = f3;
  argv[5] = 0;
  
  g_spawn_async(0, argv, 0, G_SPAWN_FILE_AND_ARGV_ZERO | G_SPAWN_SEARCH_PATH, 0, 0, 0, 0);
}

static void
compare_to(NautilusMenuItem *item, gpointer user_data) {
  GList* files = g_list_first((GList*)g_object_get_data(G_OBJECT(item), "diff-ext::compare_to"));
  gchar* f1 = gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(files->data)));
  gchar* f2 = saved->str;

  diff(f1, f2);
}

static void
compare(NautilusMenuItem *item, gpointer user_data) {
  GList* files = g_list_first((GList*)g_object_get_data(G_OBJECT(item), "diff-ext::compare"));
  gchar* f1 = gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(files->data)));
  gchar* f2 = gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(g_list_next(files)->data)));
  
  diff(f1, f2);
}

static void
compare3_to(NautilusMenuItem *item, gpointer user_data) {
  GList* files = g_list_first((GList*)g_object_get_data(G_OBJECT(item), "diff-ext::compare3_to"));
  gchar* f1 = gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(files->data)));
  gchar* f2 = gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(g_list_next(files)->data)));
  gchar* f3 = saved->str;

  diff3(f1, f2, f3);
}

static void
compare3(NautilusMenuItem *item, gpointer user_data) {
  GList* files = g_list_first((GList*)g_object_get_data(G_OBJECT(item), "diff-ext::compare3"));
  gchar* f1 = gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(files->data)));
  gchar* f2 = gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(g_list_next(files)->data)));
  gchar* f3 = gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(g_list_next(g_list_next(files))->data)));
  
  diff3(f1, f2, f3);
}

static GList* 
get_file_items(NautilusMenuProvider *provider, GtkWidget *window, GList *files) {
  g_return_val_if_fail(NAUTILUS_IS_MENU_PROVIDER(provider), NULL);
  g_return_val_if_fail(GTK_IS_WIDGET(window), NULL);

  GList* items = NULL;
  
  if(files != NULL) {
    guint n = g_list_length(files);
    GConfClient* gconf_client;
    gboolean enable_diff3;

    gconf_client = gconf_client_get_default ();
      enable_diff3 = gconf_client_get_bool(gconf_client, "/apps/diff-ext/enable-diff3", NULL);
    g_object_unref(G_OBJECT(gconf_client));
    
    if(n == 1) {
      NautilusMenuItem* save = nautilus_menu_item_new("diff-ext::save", _("Compare later"), _("Select file for comparison"), NULL /* icon name */);
      g_signal_connect(save, "activate", G_CALLBACK(compare_later), provider);
      g_object_set_data_full(G_OBJECT(save), "diff-ext::save", nautilus_file_info_list_copy(files), (GDestroyNotify)nautilus_file_info_list_free);
      items = g_list_append(items, save);

      if(strcmp("", saved->str) != 0) {
        GString* caption = g_string_new("");
        GString* hint = g_string_new("");
        
        g_string_printf(caption, _("Compare to '%s'"), saved->str);
        g_string_printf(hint, _("Compare '%s' and '%s'"), gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(files->data))), saved->str);
        
        NautilusMenuItem* compare_to_item = nautilus_menu_item_new("diff-ext::compare_to", caption->str, hint->str, NULL /* icon name */);
        g_signal_connect(compare_to_item, "activate", G_CALLBACK(compare_to), provider);
        g_object_set_data_full(G_OBJECT(compare_to_item), "diff-ext::compare_to", nautilus_file_info_list_copy(files), (GDestroyNotify)nautilus_file_info_list_free);
        items = g_list_append(items, compare_to_item);
        
        g_string_free(caption, TRUE);
        g_string_free(hint, TRUE);
      }
    } else if(n == 2) {    
      NautilusMenuItem* diff = nautilus_menu_item_new("diff-ext::compare", _("Compare"), _("Compare selected files"), NULL /* icon name */);
      g_signal_connect(diff, "activate", G_CALLBACK(compare), provider);
      g_object_set_data_full(G_OBJECT(diff), "diff-ext::compare", nautilus_file_info_list_copy(files), (GDestroyNotify)nautilus_file_info_list_free);
      items = g_list_append(items, diff);
      
      if(enable_diff3) {
        if(strcmp("", saved->str) != 0) {
          GString* caption = g_string_new("");
          GString* hint = g_string_new("");
        
          g_string_printf(caption, _("3-way compare to '%s'"), saved->str);
          g_string_printf(hint, _("3-way compare '%s', '%s' and '%s'"), 
          gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(files->data))), 
          gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(g_list_next(files)->data))), 
          saved->str);
        
          NautilusMenuItem* compare3_to_item = nautilus_menu_item_new("diff-ext::compare3_to", caption->str, hint->str, NULL /* icon name */);
          g_signal_connect(compare3_to_item, "activate", G_CALLBACK(compare3_to), provider);
          g_object_set_data_full(G_OBJECT(compare3_to_item), "diff-ext::compare3_to", nautilus_file_info_list_copy(files), (GDestroyNotify)nautilus_file_info_list_free);
          items = g_list_append(items, compare3_to_item);
          
          g_string_free(caption, TRUE);
          g_string_free(hint, TRUE);
        }
      }
    } else if(n == 3) {
      if(enable_diff3) {
        NautilusMenuItem* diff3 = nautilus_menu_item_new("diff-ext::compare3", _("3-way compare"), _("3-way compare selected files"), NULL /* icon name */);
        g_signal_connect(diff3, "activate", G_CALLBACK(compare3), provider);
        g_object_set_data_full(G_OBJECT(diff3), "diff-ext::compare3", nautilus_file_info_list_copy(files), (GDestroyNotify)nautilus_file_info_list_free);
        items = g_list_append(items, diff3);
      }
    }
    
    g_object_unref(G_OBJECT(gconf_client));
  }

  return items;
}

static GList* 
get_background_items(NautilusMenuProvider *provider, GtkWidget *window,NautilusFileInfo *folder) {
  return NULL;
}

static GList* 
get_toolbar_items(NautilusMenuProvider *provider, GtkWidget *window, NautilusFileInfo *folder) {
  return NULL;
}

/******************************************************************************
 * GObject Registration
 ******************************************************************************/
static void 
menu_provider_interface_init(NautilusMenuProviderIface *iface, gpointer data) {
  iface->get_file_items = get_file_items;
  iface->get_toolbar_items = get_toolbar_items;
  iface->get_background_items = get_background_items;
}

static void 
diff_ext_register_type(GTypeModule *module) {
  static const GTypeInfo type_info = {
    sizeof(DIFF_EXT_CLASS),
    (GBaseInitFunc)NULL,
    (GBaseFinalizeFunc)NULL,
  
    (GClassInitFunc)NULL,
    (GClassFinalizeFunc)NULL,
    NULL, 
  
    sizeof(DIFF_EXT), 0,
    (GInstanceInitFunc) NULL,
  };

  static const GInterfaceInfo menu_provider_interface_info = {
    (GInterfaceInitFunc)menu_provider_interface_init,
    (GInterfaceFinalizeFunc) NULL,
    NULL,
  };

  if(G_IS_TYPE_MODULE(module)) {
    diff_ext_type = g_type_module_register_type(module, G_TYPE_OBJECT, "DIFF_EXT", &type_info, 0);
    g_type_module_add_interface(module, diff_ext_type, nautilus_menu_provider_get_type(), &menu_provider_interface_info);
  } else {
    diff_ext_type = g_type_register_static(G_TYPE_OBJECT, "DIFF_EXT", &type_info, 0);
    g_type_add_interface_static(diff_ext_type, nautilus_menu_provider_get_type(), &menu_provider_interface_info);
  }
}

GType 
diff_ext_get_type() {
  if (diff_ext_type == 0) {
    diff_ext_register_type(NULL);
  }

  return diff_ext_type;
}

/******************************************************************************
 * Nautilus' Plug-In-Interface
 ******************************************************************************/
void
nautilus_module_initialize(GTypeModule *module) {
  g_print("Initializing diff-ext\n");
  diff_ext_register_type(module);
  provider_types[0] = diff_ext_get_type();

  bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
  bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
  
  saved = g_string_new(_(""));
}

void 
nautilus_module_shutdown() {
  g_print("Shutting down diff-ext\n");
  g_string_free(saved, TRUE);
}

void 
nautilus_module_list_types(const GType **types, int *count) {
  g_return_if_fail(types != NULL);
  g_return_if_fail(count != NULL);

  *types = provider_types;
  *count = G_N_ELEMENTS(provider_types);
}
