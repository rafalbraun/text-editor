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

gchar *open_files ();
void open_file (gpointer userdata, gchar * filepath);
void full_search_cb (GtkWidget * widget, gpointer userdata);
void find_files_cb (GtkWidget * widget, gpointer userdata);

#include "list.c"
#include "config.h"
#include "sourceview.c"
#include "treeview.c"
#include "notebook.c"
#include "callback.c"

// on quit save clipboard to xclip: https://wiki.ubuntu.com/ClipboardPersistence
void
on_main_quit (GtkWidget * widget, gpointer userdata)
{
    GError *err = NULL;
    gchar *contents = open_files ();

    g_file_set_contents (cast_to_ud(userdata)->session_info, contents, strlen (contents), &err);
    g_free (contents);

    gtk_main_quit ();
}


void
new_file_cb (GtkButton * widget, gpointer userdata)
{
    gchar buffer[64];
    gchar *fname;
    g_print ("%d \n", cast_to_ud (userdata)->untitled_files_in_buffer_max);
    sprintf (buffer, "/tmp/Untitled %d",
       cast_to_ud (userdata)->untitled_files_in_buffer_max++);
    fname = g_strdup (buffer);
    g_creat (fname, S_IREAD | S_IWRITE);
    open_file (userdata, fname);
}

gboolean
_check_timeout_since_last_keypress (gpointer userdata)
{
  gchar *fname;
  GDateTime *time_val;
  gchar *iso8601_string;

  gdouble time_elapsed = g_timer_elapsed (timer, NULL);

  //g_print ("[INFO] Timer: %lf , is active %d \n", 
  //    time_elapsed, 
  //    g_timer_is_active (timer) );

  if (time_elapsed > 3.0 && is_saved == FALSE) {

    time_val = g_date_time_new_now_local ();
    iso8601_string = g_date_time_format_iso8601 (time_val);

    //g_print ("saving file ...\n");
    is_saved = TRUE;

    fname = g_strdup_printf ("./tmp/filename_%s", iso8601_string);
    g_creat (fname, S_IREAD | S_IWRITE);

    g_print ("\e[34m[INFO] File created.\e[0m \n");
  }

    /**
        co 400 ms sprawdzaj timer i jeśli ma nabite 5 sekund (przy każdym keypress/przełączeniu karty jest reset timera) to zapisujemy plik
        
        przy przełączeniu karty powinniśmy sprawdzać czy plik różni się od tego co mamy w ostatniej wersji i jeśli tak to pomijamy a jeśli zmieniło się to zapisujemy

    */
  return TRUE;
}


void
open_files_from_last_session (gpointer userdata)
{
  gchar *text, *filepath = "~session-info";
  gchar **filepaths;
  gsize len;
  GError *err = NULL;
  guint i = 0;

  if (g_file_get_contents (filepath, &text, &len, &err) == FALSE) {
    g_error ("error reading %s: %s", filepath, err->message);
    return;
  }
  filepaths = g_strsplit (text, "\n", 0);

  while (filepaths[i]) {
    if (strcmp (filepaths[i], "") != 0) {
      open_file (userdata, filepaths[i]);
    }
    i++;
  }
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
    UserData* userdata;

    // Init GTK
    gtk_init (&argc, &argv);
    ud_init (&userdata);

    // Construct a GtkBuilder instance and load our UI description
    builder = gtk_builder_new ();
    if (gtk_builder_add_from_file (builder, "text_editor.ui", &error) == 0) {
        g_printerr ("Error loading file: %s\n", error->message);
        g_clear_error (&error);
        return 1;
    }

/*
    userdata = g_new0 (UserData, 1);
    cast_to_ud(userdata)->head = NULL;
    cast_to_ud(userdata)->session_info = "~session-info";
    cast_to_ud(userdata)->filepath = "/home/rafal/Desktop/gtksourceview-4.0.3";
*/
    // Connect signal handlers to the constructed widgets
    cast_to_ud(userdata)->window = window = gtk_builder_get_object (builder, "window");
    cast_to_ud(userdata)->buffer = buffer = gtk_builder_get_object (builder, "sourcebuffer");
    cast_to_ud(userdata)->treeview = treeview = gtk_builder_get_object (builder, "treeview");
    cast_to_ud(userdata)->treestore = treestore = gtk_builder_get_object (builder, "treestore");
    cast_to_ud(userdata)->notebook = notebook = gtk_builder_get_object (builder, "notebook");
    cast_to_ud(userdata)->treeview_menu = gtk_builder_get_object (builder, "treeview_context_menu");
    cast_to_ud(userdata)->treeview_menu_expand = gtk_builder_get_object (builder, "treeview_context_menu_expand");
    cast_to_ud(userdata)->treeview_menu_collapse = gtk_builder_get_object (builder, "treeview_context_menu_collapse");

    gtk_text_buffer_create_tag (GTK_TEXT_BUFFER (buffer), "blue", "background", "white", NULL);
    gtk_text_buffer_create_tag (GTK_TEXT_BUFFER (buffer), "black", "foreground", "black", NULL);
    gtk_text_buffer_create_tag (GTK_TEXT_BUFFER (buffer), "italic", "style", PANGO_STYLE_ITALIC, NULL);
    gtk_text_buffer_create_tag (GTK_TEXT_BUFFER (buffer), "underline", "underline", PANGO_UNDERLINE_SINGLE, NULL);

    set_language (buffer);
    set_buffer_scheme (buffer);


    /*
    // THE + BUTTON
    GtkWidget *action_widget = gtk_button_new_from_icon_name ("list-add-symbolic", GTK_ICON_SIZE_BUTTON);
    //g_signal_connect (action_widget, "clicked", G_CALLBACK (print_hello), notebook);
    gtk_notebook_set_action_widget (GTK_NOTEBOOK (notebook), action_widget, GTK_PACK_END);
    gtk_widget_show (action_widget);
    */

    fill_treestore_new (GTK_TREE_STORE (treestore), cast_to_ud(userdata)->filepath);
    expand_top_node (treeview);
    //gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(treeview), GTK_TREE_VIEW_GRID_LINES_BOTH);

    //g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), (gpointer)userdata);
    g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (on_main_quit), (gpointer) userdata);
    g_signal_connect (G_OBJECT (window), "key-press-event", G_CALLBACK (key_pressed_window), NULL);
    g_signal_connect (G_OBJECT (treeview), "key-press-event", G_CALLBACK (key_pressed_treeview), NULL);
    g_signal_connect (G_OBJECT (treeview), "button-press-event", G_CALLBACK (on_button_pressed), (gpointer) userdata);
    g_signal_connect (G_OBJECT (notebook), "switch-page", G_CALLBACK (switch_page), (gpointer) userdata);
    g_signal_connect (G_OBJECT (window), "key-press-event", G_CALLBACK (key_pressed_notebook), NULL);

    button = gtk_builder_get_object (builder, "file_new");
    g_signal_connect (button, "activate", G_CALLBACK (new_file_cb), userdata);

    button = gtk_builder_get_object (builder, "edit_cut");
    g_signal_connect (button, "activate", G_CALLBACK (show_langs), NULL);

    button = gtk_builder_get_object (builder, "edit_fullsearch");
    g_signal_connect (button, "activate", G_CALLBACK (full_search_cb), NULL);

    button = gtk_builder_get_object (builder, "edit_findfiles");
    g_signal_connect (button, "activate", G_CALLBACK (find_files_cb), NULL);

    //GObject* syntax_menuitem = gtk_builder_get_object (builder, "syntax");
    //set_syntax_submenu(GTK_MENU_ITEM(syntax_menuitem));

    timer = g_timer_new ();
    g_timer_start (timer);

    // DO ODKOMENTOWANIA
    //g_timeout_add (80, (GSourceFunc) _check_timeout_since_last_keypress, userdata);

    open_files_from_last_session (userdata);

    gtk_main ();

    return 0;
}
