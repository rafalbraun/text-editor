#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtk/gtk.h>

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
    //GtkWidget *menu, *menuitem;
    GtkMenu* menu;

    menu = get_treeview_menu(userdata);

    //menu = gtk_menu_new();

    /*
    menuitem = gtk_menu_item_new_with_label("Rename");
    g_signal_connect(menuitem, "activate", (GCallback) popup_menu_copy_file, treeview);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

    menuitem = gtk_menu_item_new_with_label("Copy");
    g_signal_connect(menuitem, "activate", (GCallback) popup_menu_copy_file, treeview);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

    menuitem = gtk_menu_item_new_with_label("Paste");
    g_signal_connect(menuitem, "activate", (GCallback) popup_menu_copy_file, treeview);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

    menuitem = gtk_menu_item_new_with_label("Local History");
    g_signal_connect(menuitem, "activate", (GCallback) popup_menu_copy_file, treeview);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

    menuitem = gtk_menu_item_new_with_label("Format File");
    g_signal_connect(menuitem, "activate", (GCallback) popup_menu_copy_file, treeview);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

    menuitem = gtk_menu_item_new_with_label("Filter");
    g_signal_connect(menuitem, "activate", (GCallback) popup_menu_copy_file, treeview);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

    menuitem = gtk_menu_item_new_with_label("Git");
    g_signal_connect(menuitem, "activate", (GCallback) popup_menu_copy_file, treeview);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
    */

    gtk_widget_show_all(GTK_WIDGET(menu));
    gtk_menu_popup_at_pointer(GTK_MENU(menu), (GdkEvent * ) event);
}

void
on_changed(GtkWidget * widget, gpointer statusbar) {

    GtkTreeIter iter;
    GtkTreeModel * model;
    gchar * value;

    if (gtk_tree_selection_get_selected(
            GTK_TREE_SELECTION(widget), & model, & iter)) {

        gtk_tree_model_get(model, & iter, COLUMN, & value, -1);
        gtk_statusbar_push(GTK_STATUSBAR(statusbar),
            gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar),
                value), value);
        g_free(value);
    }
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
            gtk_tree_store_append(treestore, &child, &toplevel);
            gtk_tree_store_set(treestore, &child, COLUMN, entry -> d_name, -1);
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

#if!TREEVIEW

int main(int argc, char * argv[]) {

    GtkWidget * window;
    GtkWidget * swindow;
    GtkWidget * view;
    GtkTreeSelection * selection;
    GtkWidget * vbox;
    GtkWidget * statusbar;
    gchar * filepath = ".";

    gtk_init( & argc, & argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window), "Tree view");
    gtk_widget_set_size_request(window, 400, 900);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    view = gtk_tree_view_new();
    create_view_and_model(filepath, view);
    swindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(swindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(swindow), view);

    gtk_box_pack_start(GTK_BOX(vbox), swindow, TRUE, TRUE, 1);

    statusbar = gtk_statusbar_new();
    gtk_box_pack_start(GTK_BOX(vbox), statusbar, FALSE, TRUE, 1);

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
    g_signal_connect(selection, "changed", G_CALLBACK(on_changed), statusbar);

    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

#endif