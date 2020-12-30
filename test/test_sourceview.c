#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtksourceview/gtksource.h>
#include <gtk/gtk.h>

#include "../src/list.h"
#include "../src/config.h"
#include "../src/sourceview.h"

int main( int argc, char *argv[] ) {

    GtkWidget *window;
    GtkWidget *scroll;
    GtkWidget *srcview;
    GtkSourceBuffer *buffer;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_show(window);

    scroll = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    buffer = GTK_SOURCE_BUFFER (gtk_source_buffer_new (NULL));
    srcview = sourceview_new (buffer);

    gtk_container_add (GTK_CONTAINER (scroll), GTK_WIDGET (srcview));
    gtk_container_add (GTK_CONTAINER (window), scroll);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gchar* text;
    gsize length = 18856;
    gchar* filepath = "/home/rafal/IdeaProjects/gtksourceview-my-ide/application/tmp";
    UserData* user_data;
    user_data = g_new0 (UserData, 1);
    user_data->window = G_OBJECT (window);
    open_file_cb (filepath, text, length, user_data);
    //create_tab (user_data, filepath, text, length);

    //g_print("%s \n", text);
    //gtk_text_buffer_set_text (GTK_TEXT_BUFFER (buffer), text, length);

    gtk_widget_show_all (window);
    gtk_main();

    return 0;
}

