#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#define TREEVIEW 1
#define NOTEBOOK 1
#define FULL_SEARCH 1
#define SOURCEVIEW 1

#include <gtksourceview/gtksource.h>
#include <gtk/gtk.h>

GObject *buffer;
GObject *window;

void show_error(GObject* window, gchar* message) {
  
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

#include "config.h"
#include "treeview.c"
#include "notebook.c"
#include "sourceview.c"
#include "callback.c"

int
main (int   argc,
      char *argv[])
{
  GtkBuilder *builder;
  GObject *button;
  GObject *treeview;
  GObject *treestore;
  GObject *notebook;
  GError *error = NULL;
  gchar* filepath = ".";

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

  /* Connect signal handlers to the constructed widgets. */
  userdata->window    = window    = gtk_builder_get_object (builder, "window");
  userdata->buffer    = buffer    = gtk_builder_get_object (builder, "sourcebuffer");
  userdata->treeview  = treeview  = gtk_builder_get_object (builder, "treeview");
  userdata->treestore = treestore = gtk_builder_get_object (builder, "treestore");
  userdata->notebook  = notebook  = gtk_builder_get_object (builder, "notebook");

  fill_treestore_new(GTK_TREE_STORE(treestore), filepath);
  expand_top_node (treeview);
  //gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(treeview), GTK_TREE_VIEW_GRID_LINES_BOTH);

  g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
  g_signal_connect (G_OBJECT (window), "key-press-event", G_CALLBACK (key_pressed_window), NULL);
  g_signal_connect (G_OBJECT (treeview), "key-press-event", G_CALLBACK (key_pressed_treeview), NULL);
  g_signal_connect (G_OBJECT (treeview), "button-press-event", G_CALLBACK (on_button_pressed), (gpointer)userdata);
  g_signal_connect (G_OBJECT (notebook), "switch-page", G_CALLBACK (switch_page), (gpointer)userdata);

  gtk_main ();

  return 0;
}
