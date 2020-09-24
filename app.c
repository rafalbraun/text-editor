#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#define TREEVIEW 1
#define NOTEBOOK 1

#include <gtk/gtk.h>
#include "treeview.c"
#include "notebook2.c"
/*
static void
print_hello (GtkWidget *widget,
             gpointer   data)
{
  GList* list = gtk_container_get_children(GTK_CONTAINER(widget));
  GtkLabel* label = ((GtkLabel*) list->data);
  const gchar* text = gtk_label_get_text(GTK_LABEL(label));
  g_print("label: %s \n", text);
}
*/
gboolean
key_pressed_treeview(GtkWidget *notebook, GdkEventKey *event, gpointer userdata) 
{
  g_print("key_pressed: GtkTreeView \n");
  return FALSE;
}
gboolean
key_pressed_window(GtkWidget *notebook, GdkEventKey *event, gpointer userdata) 
{
  g_print("key_pressed: GtkWindow \n");
  return FALSE;
}

int
main (int   argc,
      char *argv[])
{
  GtkBuilder *builder;
  GObject *window;
  GObject *button;
  GObject *widget;
  GError *error = NULL;
  GtkWidget *event_box;
  GtkWidget *view;
  gchar* filepath = "/home/rafal/IdeaProjects/gtksourceview-my-ide/application";

  gtk_init (&argc, &argv);

  /* Construct a GtkBuilder instance and load our UI description */
  builder = gtk_builder_new ();
  if (gtk_builder_add_from_file (builder, "app.ui", &error) == 0)
  {
    g_printerr ("Error loading file: %s\n", error->message);
    g_clear_error (&error);
    return 1;
  }

  /* Connect signal handlers to the constructed widgets. */
  window = gtk_builder_get_object (builder, "window");
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
  g_signal_connect (window, "key-press-event", G_CALLBACK (key_pressed_window), NULL);

  //event_box = gtk_event_box_new ();
  widget = gtk_builder_get_object (builder, "tab0");
  g_signal_connect (widget, "button-press-event", G_CALLBACK (print_hello), NULL);

  widget = gtk_builder_get_object (builder, "tab1");
  g_signal_connect (widget, "button-press-event", G_CALLBACK (print_hello), NULL);


  widget = gtk_builder_get_object (builder, "treeview");
  create_view_and_model(filepath, GTK_WIDGET(widget));
  gtk_tree_view_set_enable_tree_lines(GTK_TREE_VIEW(widget), TRUE);
  gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(widget), GTK_TREE_VIEW_GRID_LINES_BOTH);
  g_signal_connect (widget, "key-press-event", G_CALLBACK (key_pressed_treeview), NULL);

  widget = gtk_builder_get_object (builder, "notebook");
  open_file (GTK_NOTEBOOK(widget), "/tmp/Makefile");


  /*
  button = gtk_builder_get_object (builder, "button1");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

  button = gtk_builder_get_object (builder, "button2");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

  button = gtk_builder_get_object (builder, "quit");
  g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);
  */
  gtk_main ();

  return 0;
}
