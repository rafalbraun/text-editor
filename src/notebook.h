#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include <gtksourceview/gtksource.h>
#include <gtk/gtk.h>

//t_node *head;
//t_pair *map_ptr;

void list_tabs();
/*
int get_max();

int get_index(gchar* basename);

gchar* open_files();
*/
gboolean
notebook_tab_clicked(GtkWidget *widget, GdkEventButton *event, gpointer userdata);

gchar* 
get_filename_from_page_number (gpointer userdata, int pagenum);

void 
create_empty_tab (GtkMenuItem *menuitem, gpointer user_data);

//void
//save_file_default ();

GtkSourceBuffer*
create_tab (gpointer user_data, gchar* filepath);

void
close_tab (gpointer userdata, gchar* title);

//void
//load_file (gpointer userdata, guint pagenum);

//void
//save_file (const gchar* filename, const gchar* contents);

void
switch_page (GtkNotebook *notebook, GtkWidget *page, guint pagedst, gpointer userdata);


gchar*
get_text_from_eventbox(GtkWidget* widget);

#endif
