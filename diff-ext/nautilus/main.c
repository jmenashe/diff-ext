#include <string.h>
#include <gtk/gtk.h>
#include <glib-object.h>
#include <glib/gi18n-lib.h>
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
compare_to(NautilusMenuItem *item, gpointer user_data) {
  gchar* argv[4];
  GList* files = g_list_first((GList*)g_object_get_data(G_OBJECT(item), "diff-ext::compare_to"));

  argv[0] = "kdiff3";
  argv[1] = "kdiff3";
  argv[2] = gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(files->data)));
  argv[3] = saved->str;
  argv[4] = 0;
  
  g_spawn_async(0, argv, 0, G_SPAWN_FILE_AND_ARGV_ZERO | G_SPAWN_SEARCH_PATH, 0, 0, 0, 0);
}

static void
compare(NautilusMenuItem *item, gpointer user_data) {
  gchar* argv[4];
  GList* files = g_list_first((GList*)g_object_get_data(G_OBJECT(item), "diff-ext::compare"));

  argv[0] = "kdiff3";
  argv[1] = "kdiff3";
  argv[2] = gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(files->data)));
  argv[3] = gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(g_list_next(files)->data)));
  argv[4] = 0;
  
  g_spawn_async(0, argv, 0, G_SPAWN_FILE_AND_ARGV_ZERO | G_SPAWN_SEARCH_PATH, 0, 0, 0, 0);
}

static GList* 
get_file_items(NautilusMenuProvider *provider, GtkWidget *window, GList *files) {
  g_return_val_if_fail(NAUTILUS_IS_MENU_PROVIDER(provider), NULL);
  g_return_val_if_fail(GTK_IS_WIDGET(window), NULL);

  GList* items = NULL;
  
  if(files != NULL) {
    GList* scan;
    guint n = g_list_length(files);
    
    if(n == 1) {
      NautilusMenuItem* save = nautilus_menu_item_new("diff-ext::save", _("Compare later"), _("Select file for comparison"), NULL /* icon name */);
      g_signal_connect(save, "activate", G_CALLBACK(compare_later), provider);
      g_object_set_data_full(G_OBJECT(save), "diff-ext::save", nautilus_file_info_list_copy(files), (GDestroyNotify)nautilus_file_info_list_free);
      items = g_list_append(items, save);

      if(strcmp("", saved->str) != 0) {
        GString* caption = g_string_new(_("Compare to '"));
        GString* hint = g_string_new(_("Compare '"));
        
        caption = g_string_append(caption, saved->str);
        caption = g_string_append_c(caption, '\'');
        
        hint = g_string_append(hint, gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(files->data))));
        hint = g_string_append(hint, _("' and '"));
        hint = g_string_append(hint, saved->str);
        hint = g_string_append_c(hint, '\'');
        
        NautilusMenuItem* compare_to_item = nautilus_menu_item_new("diff-ext::compare_to", caption->str, hint->str, NULL /* icon name */);
        g_signal_connect(compare_to_item, "activate", G_CALLBACK(compare_to), provider);
        g_object_set_data_full(G_OBJECT(compare_to_item), "diff-ext::compare_to", nautilus_file_info_list_copy(files), (GDestroyNotify)nautilus_file_info_list_free);
        items = g_list_append(items, compare_to_item);
      }
    } 

    if(n == 2) {    
      NautilusMenuItem* diff = nautilus_menu_item_new("diff-ext::compare", _("Compare"), _("Compare selected files"), NULL /* icon name */);
      g_signal_connect(diff, "activate", G_CALLBACK(compare), provider);
      g_object_set_data_full(G_OBJECT(diff), "diff-ext::compare", nautilus_file_info_list_copy(files), (GDestroyNotify)nautilus_file_info_list_free);
      items = g_list_append(items, diff);
    }
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
}

void 
nautilus_module_list_types(const GType **types, int *count) {
  g_return_if_fail(types != NULL);
  g_return_if_fail(count != NULL);

  *types = provider_types;
  *count = G_N_ELEMENTS(provider_types);
}
