#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtk/gtk.h>

#include <sys/stat.h>
#include <fcntl.h>

//#define __TEST__

#include "../src/list.h"
#include "../src/config.h"
#include "../src/treeview.h"

#define UI_DIR "/home/rafal/IdeaProjects/gtksourceview-my-ide/application/ui/treeview.ui"
#define FILEPATH "/home/rafal/IdeaProjects/gtksourceview-my-ide/application/"

int main(int argc, char * argv[]) {

    GtkBuilder *builder;
    GObject *window;
    GObject *treeview;
    GError *error = NULL;

/*
    GtkWidget * window;
    GtkWidget * swindow;
    GtkWidget * view;
    GtkTreeSelection * selection;
    GtkWidget * vbox;
    GtkWidget * statusbar;
    gchar * filepath = ".";
*/
    gtk_init(&argc, &argv);

    builder = gtk_builder_new ();
    if (gtk_builder_add_from_file (builder, UI_DIR, &error) == 0) {
        g_printerr ("Error loading file: %s\n", error->message);
        g_clear_error (&error);
        return 1;
    }
    
    window = gtk_builder_get_object (builder, "window");
    g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);

    treeview = gtk_builder_get_object (builder, "treeview");
    fill_treeview (GTK_TREE_VIEW (treeview), FILEPATH);


/*
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
    //g_signal_connect(selection, "changed", G_CALLBACK(on_changed), statusbar);

    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
*/
    gtk_main();

    return 0;
}
