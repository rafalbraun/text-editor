#include <gtk/gtk.h>

static void
print_hello (GtkWidget *widget,
             gpointer   data)
{
  GList* list = gtk_container_get_children(GTK_CONTAINER(widget));
  GtkLabel* label = ((GtkLabel*) list->data);
  const gchar* text = gtk_label_get_text(GTK_LABEL(label));
  g_print("label: %s \n", text);
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

  //event_box = gtk_event_box_new ();
  widget = gtk_builder_get_object (builder, "tab0");
  g_signal_connect (widget, "button-press-event",
                    G_CALLBACK (print_hello), NULL);

  widget = gtk_builder_get_object (builder, "tab1");
  g_signal_connect (widget, "button-press-event",
                    G_CALLBACK (print_hello), NULL);

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
