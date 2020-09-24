#include <gtk/gtk.h>

void
preedit_changed (GtkWidget *widget, GdkEventKey *event, gpointer userdata)  {
  g_print("data %s \n", gtk_entry_get_text(GTK_ENTRY(widget)));
}

void add_to_list(GtkListStore* store, gchar *str0, gchar *str1, gchar *str2) {
  GtkTreeIter iter;
  //GType types[10];

  //gtk_list_store_set_column_types(store, 2, types);

  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, 0, str0, 1, str1, 2, str2, -1);
}

void on_changed(GtkWidget *widget, gpointer userdata) {
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *value;

  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(widget), &model, &iter)) {

    gtk_tree_model_get(model, &iter, 0, &value,  -1);
    //gtk_label_set_text(GTK_LABEL(label), value);
    g_print("value: %s \n", value);
    g_free(value);
  }
}

int
main (int   argc,
      char *argv[])
{
  GtkBuilder *builder;
  GObject *window;
  GObject *button;
  GError *error = NULL;
  GtkTreeSelection *selection; 

  gtk_init (&argc, &argv);

  /* Construct a GtkBuilder instance and load our UI description */
  builder = gtk_builder_new ();
  if (gtk_builder_add_from_file (builder, "search_path.ui", &error) == 0)
    {
      g_printerr ("Error loading file: %s\n", error->message);
      g_clear_error (&error);
      return 1;
    }

  // Connect signal handlers to the constructed widgets. 
  window = gtk_builder_get_object (builder, "window");
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
  
  button = gtk_builder_get_object (builder, "entry");
  g_signal_connect (button, "changed", G_CALLBACK (preedit_changed), NULL);

  GtkTreeView *treeview = NULL;
  GtkTreeModel *model = NULL;
  GtkListStore *liststore = NULL;
  GtkTreeIter iter;
  gchar* data1 = "Hello1aaaaa";
  gchar* data2 = "Hello2aaaa";

  treeview = GTK_TREE_VIEW(gtk_builder_get_object(builder, "treeview1"));
  model = gtk_tree_view_get_model(treeview);
  liststore = GTK_LIST_STORE(model);

  gtk_list_store_append(liststore, &iter);
  gtk_list_store_set(liststore, &iter, 0, data1, 1, data2, -1);

  //button = gtk_builder_get_object (builder, "listview");
  //selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(button));
  //g_signal_connect(selection, "changed", G_CALLBACK(on_changed), NULL);

  //button = gtk_builder_get_object (builder, "liststore");
  /*
  add_to_list(GTK_LIST_STORE(button), "Aaa", "aaa", "Aadwd");
  add_to_list(GTK_LIST_STORE(button), "Aaa", "aaa", "Aadwd");
  add_to_list(GTK_LIST_STORE(button), "Aaa", "aaa", "Aadwd");
  add_to_list(GTK_LIST_STORE(button), "Aaa", "aaa", "Aadwd");
  add_to_list(GTK_LIST_STORE(button), "Aaa", "aaa", "Aadwd");
  */
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
