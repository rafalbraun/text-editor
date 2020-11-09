#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include <stdio.h>

#include <gtksourceview/gtksource.h>
#include <gtk/gtk.h>

#include <glib/gstdio.h>

//t_node *head;
//t_pair *map_ptr;

int get_max();

void list_tabs();

int get_index(gchar* basename);

gchar* open_files();

void
close_file(gpointer userdata, gchar* title);

 gboolean
notebook_tab_clicked(GtkWidget *widget, GdkEventButton *event, gpointer userdata);

 void
load_file(gpointer userdata, guint pagenum);

 void
save_file(gchar* path, gchar* contents);

void
open_file (gpointer userdata, gchar* filepath);

void
switch_page (GtkNotebook *notebook, GtkWidget *page, guint pagedst, gpointer userdata);

#endif
