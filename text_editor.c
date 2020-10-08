#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#define TREEVIEW 1
#define NOTEBOOK 1
#define FULL_SEARCH 1
#define SOURCEVIEW 1
#define LIST 1
#define MAP 1

#include <gtksourceview/gtksource.h>
#include <gtk/gtk.h>

static GtkClipboard *clipboard;

void show_error(GtkWindow* window, gchar* message) {
  
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(window),
              GTK_DIALOG_DESTROY_WITH_PARENT,
              GTK_MESSAGE_ERROR,
              GTK_BUTTONS_OK,
              message);
    gtk_window_set_title(GTK_WINDOW(dialog), "Error");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// TODO -- check if user installed xclip
// TODO -- check if I can cut some code from xclip to get rid of dependency

// on quit save clipboard to xclip
// https://wiki.ubuntu.com/ClipboardPersistence
void
on_main_quit (void) {

    GdkScreen *screen = gdk_screen_get_default();
    GdkDisplay *display = gdk_display_get_default();
    GtkClipboard *clipboard = gtk_clipboard_get_for_display(display, GDK_SELECTION_CLIPBOARD);

    gchar* filename = "/tmp/clipboard";
    gchar* command  = g_strconcat("xclip -sel clip < ", filename, NULL);
    gchar* contents = gtk_clipboard_wait_for_text(clipboard);

    int fd = g_mkstemp(filename);
    if (fd != -1) {
        g_warning ("g_mkstemp works even if template doesn't contain XXXXXX");
    }

    GError *err = NULL;
    g_file_set_contents(filename, contents, strlen(contents), &err);

    // to print clipboard contents:
    // $ xclip -selection clipboard -o
    system(command);
    close (fd);

    gtk_main_quit();
}

/*
gchar *
g_basename(char *filepath) {
    char *base = strrchr(filepath, '/');
    return base ? base+1 : filepath;
}
*/
#include "map.c"
#include "list.c"
#include "config.h"
#include "treeview.c"
#include "notebook.c"
#include "sourceview.c"
#include "callback.c"

int
main (int argc, char *argv[])
{
    GtkBuilder *builder;
    GObject *buffer;
    GObject *window;
    GObject *button;
    GObject *treeview;
    GObject *treestore;
    GObject *notebook;
    GError *error = NULL;
    //gchar* filepath = ".";
    gchar* filepath = "/home/rafal/Desktop/gtksourceview-4.0.3";

    // init code
    gtk_init (&argc, &argv);
    //map_ptr = (t_pair*)malloc(MAPSIZE * sizeof(t_pair));

    //clipboard = gtk_clipboard_get(GDK_SELECTION_PRIMARY);
    //clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    //gtk_clipboard_set_can_store(clipboard, NULL, 0);


    /*
    GdkScreen *screen = gdk_screen_get_default();
    GdkDisplay *display = gdk_display_get_default();
    GtkClipboard *clipboard =
        gtk_clipboard_get_for_display(display, GDK_SELECTION_CLIPBOARD);
    gtk_clipboard_set_text(clipboard, "Hello world", -1);
    gtk_clipboard_request_text(clipboard, callback, NULL);
    if (gdk_display_supports_clipboard_persistence(display)) {
        printf("Supports clipboard persistence.\n");
        gtk_clipboard_store(clipboard);
    } else {
        printf("Noooooooooooooooo.\n");
    }*/


    /* Construct a GtkBuilder instance and load our UI description */
    builder = gtk_builder_new ();
    if (gtk_builder_add_from_file (builder, "text_editor.ui", &error) == 0)
    {
      g_printerr ("Error loading file: %s\n", error->message);
      g_clear_error (&error);
      return 1;
    }

    UserData *userdata = g_new0(UserData, 1);
    userdata->head = NULL;

    /* Connect signal handlers to the constructed widgets. */
    userdata->window    = window    = gtk_builder_get_object (builder, "window");
    userdata->buffer    = buffer    = gtk_builder_get_object (builder, "sourcebuffer");
    userdata->treeview  = treeview  = gtk_builder_get_object (builder, "treeview");
    userdata->treestore = treestore = gtk_builder_get_object (builder, "treestore");
    userdata->notebook  = notebook  = gtk_builder_get_object (builder, "notebook");

    fill_treestore_new(GTK_TREE_STORE(treestore), filepath);
    expand_top_node (treeview);
    //gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(treeview), GTK_TREE_VIEW_GRID_LINES_BOTH);

    g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (on_main_quit), NULL);
    g_signal_connect (G_OBJECT (window), "key-press-event", G_CALLBACK (key_pressed_window), NULL);
    g_signal_connect (G_OBJECT (treeview), "key-press-event", G_CALLBACK (key_pressed_treeview), NULL);
    g_signal_connect (G_OBJECT (treeview), "button-press-event", G_CALLBACK (on_button_pressed), (gpointer)userdata);
    g_signal_connect (G_OBJECT (notebook), "switch-page", G_CALLBACK (switch_page), (gpointer)userdata);

    gtk_main ();

    return 0;
}
