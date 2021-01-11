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

typedef enum pm {
	COLLAPSE,
	EXPAND
} PopupMenu;

void 
load_file (GtkSourceBuffer* buffer, gchar* path, gpointer user_data);

gchar* 
get_selection(GtkWidget * treeview);

void
popup_menu(GtkWidget *treeview, GdkEventButton *event, gpointer userdata);

void
on_changed(GtkWidget * widget, gpointer statusbar);

void
fill_treestore(const gchar * filepath, GtkTreeView * tree_view, GtkTreeIter toplevel, gpointer user_data) ;

void
fill_treeview(gpointer user_data);

gboolean
on_button_pressed(GtkWidget *treeview, GdkEventButton *event, gpointer userdata);

gboolean
key_pressed_treeview(GtkWidget *treeview, GdkEventKey *event, gpointer userdata);

void 
validate_file(GtkTreeModel *model, GtkTreeSelection *selection, gpointer user_data);

void 
save_expanded_tree_nodes (gpointer user_data);

gchar* tree_path_to_string (GtkTreeModel* tree_model, GtkTreePath* path);

static gboolean
for_each_func_expand (GtkTreeModel *tree_model, GtkTreePath *path, GtkTreeIter *iter, gpointer user_data);

void
map_expanded_rows (GtkTreeView *tree_view, GtkTreePath *path, gpointer user_data);

void 
save_expanded_nodes_to_file (gpointer user_data);

#endif
