#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <gtk/gtk.h>

#include <sys/stat.h>
#include <fcntl.h>


#define SIZE 1000

enum {
    COLUMN = 0,
    NUM_COLS
};

gchar* 
get_selection(GtkWidget * treeview);

void
popup_menu(GtkWidget *treeview, GdkEventButton *event, gpointer userdata);

void
on_changed(GtkWidget * widget, gpointer statusbar);


void
fill_treestore(const char * pathname, GtkTreeStore * treestore, GtkTreeIter toplevel);

void
fill_treeview(GtkTreeView * treeview, const char * pathname);

gboolean
on_button_pressed(GtkWidget *treeview, GdkEventButton *event, gpointer userdata);

gboolean
key_pressed_treeview(GtkWidget *treeview, GdkEventKey *event, gpointer userdata);

// void
// expand_top_node(GObject * treeview);
/*
void
create_view_and_model(gchar * filepath, GtkWidget * treeview);

GtkTreeModel *
create_and_fill_model(const char * pathname);
*/
#endif
