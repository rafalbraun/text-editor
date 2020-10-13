#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtk/gtk.h>

enum {

  LIST_ITEM = 0,
  N_COLUMNS
};

void init_list(GtkWidget *list) {

  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkListStore *store;

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes("List Items",
          renderer, "text", LIST_ITEM, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

  store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);

  gtk_tree_view_set_model(GTK_TREE_VIEW(list), 
      GTK_TREE_MODEL(store));

  g_object_unref(store);
}

void add_to_list(GtkWidget *list, const gchar *str) {
    
  GtkListStore *store;
  GtkTreeIter iter;

  store = GTK_LIST_STORE(gtk_tree_view_get_model
      (GTK_TREE_VIEW(list)));

  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, LIST_ITEM, str, -1);
}

gboolean
key_pressed(GtkWidget *entry, GdkEventKey *event, gpointer userdata) {
  g_print("key pressed : %s \n", gtk_entry_get_text(GTK_ENTRY(entry)));
  return FALSE;
}

int main(int argc, char *argv[]) {

  GtkWidget *window;
  GtkWidget *list;
  GtkWidget *entry;

  GtkWidget *vbox;
  GtkWidget *label;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  list = gtk_tree_view_new();

  gtk_window_set_title(GTK_WINDOW(window), "List view");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  gtk_window_set_default_size(GTK_WINDOW(window), 270, 250);

  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);

  //vbox = gtk_vbox_new(FALSE, 0);
  vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  entry = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 2);
  gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 2);

  gtk_container_add(GTK_CONTAINER(window), vbox);

  g_signal_connect (G_OBJECT (entry), 
      "changed", G_CALLBACK (key_pressed), NULL);
  g_signal_connect (G_OBJECT (window), 
      "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
