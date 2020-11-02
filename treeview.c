#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtk/gtk.h>

#include <sys/stat.h>
#include <fcntl.h>

#define SIZE 1000

enum {
    COLUMN = 0,
    NUM_COLS
};

gchar* 
get_selection(GtkWidget * treeview) {
    GtkTreeIter iter;
    GtkTreeModel *model;
    GtkTreeSelection *selection;
    gchar *value;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));

    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)) {
        gtk_tree_model_get(model, &iter, COLUMN, &value, -1);
        return value;
    }
    return NULL;
}

void
popup_menu_copy_file(GtkWidget *menuitem, gpointer treeview) {
    gchar *filename = get_selection(treeview);
    g_free(filename);
}

void
popup_menu(GtkWidget *treeview, GdkEventButton *event, gpointer userdata) {

    GtkMenu* menu;

    menu = get_treeview_menu(userdata);

    gtk_widget_show_all(GTK_WIDGET(menu));
    gtk_menu_popup_at_pointer(GTK_MENU(menu), (GdkEvent * ) event);

}

void
on_changed(GtkWidget * widget, gpointer statusbar) {

    GtkTreeIter iter;
    GtkTreeModel * model;
    gchar * value;

    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), & model, & iter)) {

        gtk_tree_model_get(model, & iter, COLUMN, & value, -1);
        gtk_statusbar_push(GTK_STATUSBAR(statusbar),
            gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), value), value);
        g_free(value);
    }
}

int is_text_file(gchar* filepath) {
    gchar *text;
    gsize len;
    GError *err = NULL;

    if (g_file_get_contents(filepath, &text, &len, &err) == FALSE) {
        return 0;
    }
    if (!g_utf8_validate (text, len, NULL)) {
        return 0;
    }
    return 1;
}

void
fill_treestore(const char * pathname, GtkTreeStore * treestore, GtkTreeIter toplevel) {
    DIR * dir;
    struct dirent * entry;
    GtkTreeIter child;

    if (!(dir = opendir(pathname))) {
        return;
    }

    while ((entry = readdir(dir)) != NULL) {

        if (entry -> d_name[0] == '.') {
            continue;
        }

        if (entry -> d_type == DT_DIR) {

            gchar path[SIZE];
            if (g_strcmp0(entry -> d_name, ".") == 0 || g_strcmp0(entry -> d_name, "..") == 0) {
                continue;
            }

            snprintf(path, sizeof(path), "%s/%s", pathname, entry -> d_name); // create name of subdirectory

            gtk_tree_store_append(treestore, &child, &toplevel);
            gtk_tree_store_set(treestore, &child, COLUMN, entry -> d_name, -1);

            fill_treestore(path, treestore, child);
        } else {

            struct stat sb;
            gchar path[SIZE];
            snprintf(path, sizeof(path), "%s/%s", pathname, entry -> d_name); // create name of subdirectory
            //g_print("[INFO] adding file %s \n", path);

            //if (stat(path, &sb) == 0 && sb.st_mode & S_IXUSR) {
            //    continue;
            //} else {
                gtk_tree_store_append(treestore, &child, &toplevel);
                gtk_tree_store_set(treestore, &child, COLUMN, entry->d_name, -1);
            //}
        }
    }
    closedir(dir);
}

static void
fill_treestore_new(GtkTreeStore * treestore,
    const char * pathname) {
    GtkTreeIter toplevel;

    gtk_tree_store_append(treestore, & toplevel, NULL);
    gtk_tree_store_set(treestore, & toplevel, COLUMN, pathname, -1);

    fill_treestore(pathname, treestore, toplevel);
}

GtkTreeModel *
create_and_fill_model(const char * pathname) {
    GtkTreeIter toplevel;
    GtkTreeStore * treestore;

    treestore = gtk_tree_store_new(NUM_COLS,
        G_TYPE_STRING);

    gtk_tree_store_append(treestore, & toplevel, NULL);
    gtk_tree_store_set(treestore, & toplevel, COLUMN, pathname, -1);

    fill_treestore(pathname, treestore, toplevel);

    return GTK_TREE_MODEL(treestore);
}

void expand_top_node(GObject * treeview) {
    GtkTreePath * treepath;

    /* Expand top tree node */
    treepath = gtk_tree_path_new_from_string("0");
    gtk_tree_view_expand_row(GTK_TREE_VIEW(treeview), treepath, FALSE);
}

void
create_view_and_model(gchar * filepath, GtkWidget * treeview) {
    GtkCellRenderer * renderer;
    GtkTreeViewColumn * col;
    GtkTreePath * treepath;
    GtkTreeModel * model;

    gtk_tree_view_set_enable_search(GTK_TREE_VIEW(treeview), FALSE);

    col = gtk_tree_view_column_new();
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);

    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_add_attribute(col, renderer, "text", COLUMN);

    model = create_and_fill_model(filepath);
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), model);
    g_object_unref(model);

    /* Expand top tree node */
    treepath = gtk_tree_path_new_from_string("0");
    gtk_tree_view_expand_row(GTK_TREE_VIEW(treeview), treepath, FALSE);
}
