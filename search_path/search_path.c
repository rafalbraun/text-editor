#include <gtk/gtk.h>

void
preedit_changed (GtkEntry *widget, GdkEventKey *event, gpointer userdata)  {
  g_print("data %s \n", gtk_entry_get_text(GTK_ENTRY(widget)));
}

// widget is of type GtkTreeView
void add_to_list(GObject* widget, gchar *str0, gchar *str1) {
  GtkTreeView *treeview = NULL;
  GtkTreeModel *model = NULL;
  GtkListStore *liststore = NULL;
  GtkTreeIter iter;

  treeview = GTK_TREE_VIEW(widget);
  model = gtk_tree_view_get_model(treeview);
  liststore = GTK_LIST_STORE(model);

  gtk_list_store_append(liststore, &iter);
  gtk_list_store_set(liststore, &iter, 0, str0, 1, str1, -1);
}

void on_changed(GtkTreeSelection *widget, gpointer userdata) {
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

void
row_activated (GtkTreeView       *tree_view,
               GtkTreePath       *path,
               GtkTreeViewColumn *column,
               gpointer           user_data) {
  g_print("row activated \n");
}

int
main (int   argc,
      char *argv[])
{
  GtkBuilder *builder;
  GObject *window, *entry, *treeview;
  GtkTreeSelection *selection;
  GError *error = NULL;

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
  
  entry = gtk_builder_get_object (builder, "entry");
  g_signal_connect (entry, "changed", G_CALLBACK (preedit_changed), NULL);

  treeview = gtk_builder_get_object(builder, "treeview1");
  g_signal_connect(treeview, "row-activated", G_CALLBACK(row_activated), NULL);

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
  g_signal_connect(selection, "changed", G_CALLBACK(on_changed), NULL);

  add_to_list(treeview, "string000000000", "string11111111111");
  add_to_list(treeview, "string000000000", "string11111111111");
  add_to_list(treeview, "string000000000", "string11111111111");
  add_to_list(treeview, "string000000000", "string11111111111");
  add_to_list(treeview, "string000000000", "string11111111111");
  add_to_list(treeview, "string000000000", "string11111111111");
  add_to_list(treeview, "string000000000", "string11111111111");
  add_to_list(treeview, "string000000000", "string11111111111");
  add_to_list(treeview, "string000000000", "string11111111111");
  add_to_list(treeview, "string000000000", "string11111111111");
  add_to_list(treeview, "string000000000", "string11111111111");
  add_to_list(treeview, "string000000000", "string11111111111");









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
