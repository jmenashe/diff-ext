#ifndef __gdiff_ext_h__
#define __gdiff_ext_h__

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include <glib/gi18n-lib.h>
#include <gtk/gtk.h>
#include <gconf/gconf.h>
#include <gconf/gconf-client.h>
#include <libgnomevfs/gnome-vfs-utils.h>
#include <libnautilus-extension/nautilus-menu-provider.h>

extern GType provider_types[1];

extern GList* get_file_items(NautilusMenuProvider *provider, GtkWidget *window, GList *files);

extern void nautilus_module_initialize(GTypeModule *module);
extern void nautilus_module_shutdown();
extern void diff_ext_register_type(GTypeModule *module);
extern GType diff_ext_get_type();

extern void compare_later(NautilusMenuItem *item, gpointer user_data);
extern void diff(gchar* f1, gchar* f2);
extern void diff3(gchar* f1, gchar* f2, gchar* f3);
extern void compare_to(NautilusMenuItem *item, gpointer user_data);
extern void compare(NautilusMenuItem *item, gpointer user_data);
extern void compare3_to(NautilusMenuItem *item, gpointer user_data);
extern void compare3(NautilusMenuItem *item, gpointer user_data);

#endif /*__gdiff_ext_h__*/
