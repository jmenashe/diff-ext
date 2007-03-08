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

#include <glib-object.h>
#include <libnautilus-extension/nautilus-info-provider.h>

#include "gdiff-ext.h"

typedef struct _DIFF_EXT {
  GObject object;
} DIFF_EXT;

typedef struct _DIFF_EXT_CLASS {
  GObjectClass parent_class;
} DIFF_EXT_CLASS;

GType provider_types[1];
static GType diff_ext_type = 0;

void
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
  
  g_free(diff_tool);
}

void
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

  g_free(diff3_tool);
}

void
compare(NautilusMenuItem *item, gpointer user_data) {
  GList* files = g_list_first((GList*)g_object_get_data(G_OBJECT(item), "diff-ext::compare"));
  gchar* f1 = 0;
  gchar* f2 = 0;
  
  f1 = gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(files->data)));
  files = g_list_next(files);
  f2 = gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(files->data)));

  diff(f1, f2);
}

void
compare3(NautilusMenuItem *item, gpointer user_data) {
  GList* files = g_list_first((GList*)g_object_get_data(G_OBJECT(item), "diff-ext::compare3"));
  gchar* f1 = 0;
  gchar* f2 = 0;
  gchar* f3 = 0;
  
  f1 = gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(files->data)));
  files = g_list_next(files);
  f2 = gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(files->data)));
  files = g_list_next(files);
  f3 = gnome_vfs_get_local_path_from_uri(nautilus_file_info_get_uri((NautilusFileInfo*)(files->data)));
  
  diff3(f1, f2, f3);
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

void 
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
nautilus_module_list_types(const GType **types, int *count) {
  g_return_if_fail(types != NULL);
  g_return_if_fail(count != NULL);

  *types = provider_types;
  *count = G_N_ELEMENTS(provider_types);
}
