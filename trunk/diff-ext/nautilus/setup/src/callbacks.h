#include <gtk/gtk.h>

extern gpointer model;
	
extern void apply();

void
on_ok_clicked                          (GtkButton       *button,
                                        gpointer         user_data);

void
on_apply_clicked                       (GtkButton       *button,
                                        gpointer         user_data);

void
on_about_clicked                       (GtkButton       *button,
                                        gpointer         user_data);

void
on_3_way_compare_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_diff_tool_selection_changed         (GtkFileChooser  *filechooser,
                                        gpointer         user_data);

void
on_diff_tool_realize                   (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_diff3_tool_realize                  (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_diff3_tool_selection_changed        (GtkFileChooser  *filechooser,
                                        gpointer         user_data);


void
on_3_way_compare_realize               (GtkWidget       *widget,
                                        gpointer         user_data);

