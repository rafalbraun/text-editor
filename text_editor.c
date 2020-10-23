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

#include <glib/gstdio.h>

#include <sys/stat.h>
#include <fcntl.h>

//static GtkClipboard *clipboard;
gchar* open_files();
static void open_file (gpointer userdata, gchar* filepath);

void 
show_error(GtkWindow* window, gchar* message) {
  
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

#include "list.c"
#include "config.h"
#include "sourceview.c"
#include "treeview.c"
#include "notebook.c"
#include "callback.c"


// TODO -- check if user installed xclip
// TODO -- check if I can cut some code from xclip to get rid of dependency

// on quit save clipboard to xclip
// https://wiki.ubuntu.com/ClipboardPersistence
void
on_main_quit (GtkWidget *widget, gpointer userdata) {

    GdkScreen *screen = gdk_screen_get_default();
    GdkDisplay *display = gdk_display_get_default();
    GtkClipboard *clipboard = gtk_clipboard_get_for_display(display, GDK_SELECTION_CLIPBOARD);

    gchar* filename = "/tmp/clipboard";
    gchar* command  = g_strconcat("xclip -sel clip < ", filename, NULL);
    gchar* contents = gtk_clipboard_wait_for_text(clipboard);
    GError *err = NULL;

    int fd = g_mkstemp(filename);
    if (fd != -1) {
        g_warning ("g_mkstemp works even if template doesn't contain XXXXXX");
    }
    g_file_set_contents(filename, contents, strlen(contents), &err);
    g_close (fd, &err);
    g_free (contents);

    // to print clipboard contents:
    // $ xclip -selection clipboard -o
    system (command);

    /*
    // save sessions
    // https://stackoverflow.com/questions/28533553/what-is-the-default-mode-for-open-calls-with-o-creat-and-how-to-properly-set-i
    //fd = g_open("~session-info", O_WRONLY | O_CREAT | O_TRUNC, 0640);
    fd = g_open("~session-info", O_RDONLY);
    g_close (fd, &err);
    */
    contents = open_files();
    //g_print("%s\n", contents);

    //GError *err = NULL;
    err = NULL;
    //g_file_set_contents ("~session-info", contents, strlen(contents), &err);
    g_file_set_contents (((UserData*)userdata)->session_info, contents, strlen(contents), &err);
    g_free(contents);

    gtk_main_quit();
}

void 
open_files_from_last_session(gpointer userdata) {
    gchar *text, *filepath = "~session-info";
    gchar **filepaths;
    gsize len;
    GError *err = NULL;
    guint i = 0;

    if (g_file_get_contents(filepath, &text, &len, &err) == FALSE) {
        g_error("error reading %s: %s", filepath, err->message);
        return;
    }
    filepaths = g_strsplit(text, "\n", 0);

    while(filepaths[i]) {
        if(strcmp(filepaths[i], "")!=0) {
            open_file(userdata, filepaths[i]);
        }
        i++;
    }
}

// static void
// print_hello (GtkMenuItem *menuitem,
//              gpointer   data)
// {
//   //g_print ("Hello World\n");
//   GObject *window;
//   window = full_search_window_new();
// }

/*
gchar *
g_basename(char *filepath) {
    char *base = strrchr(filepath, '/');
    return base ? base+1 : filepath;
}
*/
//#include "map.c"
static GtkWidget* subwindow;
static void
print_hello (GtkWidget *widget,
             gpointer   data)
{
  g_print ("Hello World\n");
}
static void show_subwindow(GtkButton *widget, gpointer   user_data) {
    subwindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    GtkWidget *button;
    button = gtk_button_new_with_label("Button");
    gtk_container_add(GTK_CONTAINER(subwindow), button);

    gtk_widget_show_all(subwindow);
    g_signal_connect(G_OBJECT(subwindow), "destroy", G_CALLBACK(gtk_widget_destroy), NULL);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(print_hello), NULL);
}

// https://en.wikibooks.org/wiki/GTK%2B_By_Example/Tree_View/Tree_Models
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
    //gchar* filepath = "/home/rafal/Desktop/gtksourceview-4.0.3";
    //gchar* filepath = "/home/rafal/IdeaProjects/vamos-0.8.2-x86_64";
    //gchar* filepath = "/home/rafal/IdeaProjects/vdrift";
    //gchar* filepath = "/home/rafal/IdeaProjects";
    //gchar* filepath = "/home/rafal/IdeaProjects/gtksourceview-my-ide/application";
    gchar* filepath = "/home/rafal/go/src/wykop.pl";

    // init code
    gtk_init (&argc, &argv);

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
    userdata->session_info = "~session-info";

    /* Connect signal handlers to the constructed widgets. */
    userdata->window    = window    = gtk_builder_get_object (builder, "window");
    userdata->buffer    = buffer    = gtk_builder_get_object (builder, "sourcebuffer");
    userdata->treeview  = treeview  = gtk_builder_get_object (builder, "treeview");
    userdata->treestore = treestore = gtk_builder_get_object (builder, "treestore");
    userdata->notebook  = notebook  = gtk_builder_get_object (builder, "notebook");

    fill_treestore_new(GTK_TREE_STORE(treestore), filepath);
    expand_top_node (treeview);
    //gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(treeview), GTK_TREE_VIEW_GRID_LINES_BOTH);

    g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (on_main_quit), (gpointer)userdata);
    g_signal_connect (G_OBJECT (window), "key-press-event", G_CALLBACK (key_pressed_window), NULL);
    g_signal_connect (G_OBJECT (treeview), "key-press-event", G_CALLBACK (key_pressed_treeview), NULL);
    g_signal_connect (G_OBJECT (treeview), "button-press-event", G_CALLBACK (on_button_pressed), (gpointer)userdata);
    g_signal_connect (G_OBJECT (notebook), "switch-page", G_CALLBACK (switch_page), (gpointer)userdata);

    button = gtk_builder_get_object (builder, "filenew");
    g_signal_connect (button, "activate", G_CALLBACK (show_subwindow), NULL);

    button = gtk_builder_get_object (builder, "editcut");
    g_signal_connect (button, "activate", G_CALLBACK (list_tabs), NULL);

    open_files_from_last_session (userdata);

    gtk_main ();

    return 0;
}
