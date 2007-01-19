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

static GString* left = g_string_new("");
static GString* right = g_string_new("");

void
file_log(const gchar *log_domain, GLogLevelFlags log_level, const gchar *message, gpointer user_data) {
  FILE* f = fopen("/tmp/diff-ext.log", "a");
  fprintf(f, "Domain: %s; message: %s", log_domain, message);
  fclose(f);
}

static void
select_left(NautilusMenuItem *item, gpointer user_data) {
  GList* files = g_list_first((GList*)g_object_get_data(G_OBJECT (item), "diff-ext::left"));
  
  g_string_assign(left, gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(files->data))));
}

static void
select_right(NautilusMenuItem *item, gpointer user_data) {
  GList* files = g_list_first((GList*)g_object_get_data(G_OBJECT(item), "diff-ext::right"));
  
  g_string_assign(right, gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(files->data))));
}

static void
compare(NautilusMenuItem *item, gpointer user_data) {
  gchar* argv[4];
  char cwd[PATH_MAX+1];
  GList* files = g_list_first((GList*)g_object_get_data(G_OBJECT(item), "diff-ext::compare"));
  getcwd(cwd, PATH_MAX);
  argv[0] = "/home/serg/proj/kdiff3/src/kdiff3";
  argv[1] = "/home/serg/proj/kdiff3/src/kdiff3";
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
    
    g_log(G_LOG_DOMAIN, G_LOG_LEVEL_MESSAGE, "get_file_items: files selected: %d\n", n);
    for(scan = files; scan; scan = scan->next) {
      g_log(G_LOG_DOMAIN, G_LOG_LEVEL_MESSAGE, "get_file_items: file: %s\n", gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(scan->data))));
    }
    
    if(n == 1) {
      NautilusMenuItem* left = nautilus_menu_item_new("diff-ext::left", _("Select left side"), _("Select left side for comparison"), NULL /* icon name */);
      g_signal_connect(left, "activate", G_CALLBACK(select_left), provider);
      g_object_set_data_full(G_OBJECT(left), "diff-ext::left", nautilus_file_info_list_copy(files), (GDestroyNotify)nautilus_file_info_list_free);
      items = g_list_append(items, left);
      NautilusMenuItem* right = nautilus_menu_item_new("diff-ext::right", _("Select right side"), _("Select right side for comparison"), NULL /* icon name */);
      g_signal_connect(right, "activate", G_CALLBACK(select_right), provider);
      g_object_set_data_full(G_OBJECT(right), "diff-ext::right", nautilus_file_info_list_copy(files), (GDestroyNotify)nautilus_file_info_list_free);
      items = g_list_append(items, right);

      if() {
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
  
  g_log_set_handler ("GLib-GObject", G_LOG_LEVEL_MASK | G_LOG_FLAG_FATAL | G_LOG_FLAG_RECURSION, file_log, NULL);
  g_log_set_handler ("GLib", G_LOG_LEVEL_MASK | G_LOG_FLAG_FATAL | G_LOG_FLAG_RECURSION, file_log, NULL);
  g_log_set_handler ("", G_LOG_LEVEL_MASK | G_LOG_FLAG_FATAL | G_LOG_FLAG_RECURSION, file_log, NULL);
  g_log_set_handler (G_LOG_DOMAIN, G_LOG_LEVEL_MASK | G_LOG_FLAG_FATAL | G_LOG_FLAG_RECURSION, file_log, NULL);
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

