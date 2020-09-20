#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtk/gtk.h>

static void
print_hello (GtkWidget *widget, GtkWidget* parent_window) {
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;

  dialog = gtk_file_chooser_dialog_new ("Open File",
                                        GTK_WINDOW(parent_window),
                                        action,
                                        "_Cancel",
                                        GTK_RESPONSE_CANCEL,
                                        "_Open",
                                        GTK_RESPONSE_ACCEPT,
                                        NULL);

  res = gtk_dialog_run (GTK_DIALOG (dialog));
  if (res == GTK_RESPONSE_ACCEPT) {
      gchar *filename;
      GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
      filename = gtk_file_chooser_get_filename (chooser);
      //open_file (filename);
      g_print("GTK_RESPONSE_ACCEPT: %s \n", filename);
      g_free (filename);
  } else if (res == GTK_RESPONSE_CANCEL) {
      g_print("GTK_RESPONSE_CANCEL \n");
  }

  gtk_widget_destroy (dialog);

}

int main(int argc, char *argv[]) {

  GtkWidget *window;
  GtkWidget *button;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Tooltip");
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
  
  button = gtk_button_new_with_label("Button");
  gtk_widget_set_tooltip_text(button, "Button widget");
  
  gtk_container_add(GTK_CONTAINER(window), button);  

  gtk_widget_show_all(window);
  
  g_signal_connect(G_OBJECT(button), "clicked",
      G_CALLBACK(print_hello), window); 
  g_signal_connect(G_OBJECT(window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL); 

  gtk_main();

  return 0;
}