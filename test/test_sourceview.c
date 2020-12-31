#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtksourceview/gtksource.h>
#include <gtk/gtk.h>

#include <sys/stat.h>

#include "../src/list.h"
#include "../src/config.h"
#include "../src/sourceview.h"

#define FILE_IN     "/home/rafal/IdeaProjects/gtksourceview-my-ide/application/tmp"
#define FILE_OUT    "/home/rafal/IdeaProjects/gtksourceview-my-ide/application/output"

UserData* fill_user_data(GtkWidget* window, GtkWidget* buffer) {
    UserData* user_data;
    user_data = g_new0 (UserData, 1);
    user_data->window = G_OBJECT (window);
    user_data->buffer = G_OBJECT (buffer);
    return user_data;
}

int get_filesize (const char* filepath) {
    struct stat st;
    stat (filepath, &st);
    return st.st_size;
}

int main( int argc, char *argv[] ) {

    GtkWidget *window;
    GtkWidget *scroll;
    GtkWidget *srcview;
    GtkWidget *buffer;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_show(window);
    
    scroll = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    buffer = (GtkWidget*) gtk_source_buffer_new (NULL);
    srcview = sourceview_new (GTK_SOURCE_BUFFER (buffer));

    gtk_container_add (GTK_CONTAINER (scroll), GTK_WIDGET (srcview));
    gtk_container_add (GTK_CONTAINER (window), scroll);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    ///////////////////////////////////////////////////////

    gchar* text = NULL;
    gsize length;
    UserData* user_data;

    user_data = fill_user_data (window, buffer);
    length = get_filesize (FILE_IN);

    open_file_cb (FILE_IN, &text, length, user_data);

    gtk_text_buffer_set_text (GTK_TEXT_BUFFER (buffer), text, length);

    save_file_cb (FILE_OUT, text, user_data);

    //create_tab (user_data, filepath, text, length);
    //g_print("text: %s \n", text);
    
    ///////////////////////////////////////////////////////

    gtk_widget_show_all (window);
    gtk_main();

    return 0;
}
