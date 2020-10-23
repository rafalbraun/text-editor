#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtk/gtk.h>

#define BUFFER_SIZE 50000
#define SIZE 1024

enum {

  LIST_ITEM = 0,
  N_COLUMNS
};

int stdout_fd = -1;
char buffer [BUFFER_SIZE];
guint gLimit = 2;
gchar entry_buffer[SIZE];

void add_to_list(GtkWidget *listview, const gchar *str) {

  GtkListStore *store;
  GtkTreeIter iter;

  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(listview)));

  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, LIST_ITEM, str, -1);
}

gboolean 
_main_loop (gpointer treeview)
{
    int       bytes;
    gchar**   lines;
    gchar**   line;
    gchar*    filepath;
    gchar     *start, *end;

    bytes = read (stdout_fd, buffer, BUFFER_SIZE);
    buffer[bytes-1] = '\0';

    if (bytes > 0) {
        lines = g_strsplit(buffer, "\n", -1);

        for (int i=0 ; i<g_strv_length(lines); i++) {
          add_to_list(treeview, lines[i]);
        }

        g_strfreev(lines);
    }
    memset(buffer, 0, sizeof(buffer));

    return TRUE;
}

int search_process(gchar* needle) {
  char *argv [15];
  GPid child_pid;
  int status;

  memset (argv, 0, 15 * sizeof (char *));
  argv[0] = "./find_files";
  argv[1] = "/usr/include";
  argv[2] = needle;
  status = g_spawn_async_with_pipes (NULL, argv, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, &child_pid, NULL, &stdout_fd, NULL, NULL);

  if (!status) {
    g_print("[FAILED] 1 Failed to run %s: %d \n", argv [0], status);
    return 1;
  }
  if (child_pid == 0) {
    g_print("[FAILED] 2 child pid not returned \n");
    return 1;
  }
  if (stdout_fd == -1) {
    g_print("[FAILED] 3 out fd is -1 \n");
    return 1;
  }

  return 0;
}

void init_list(GtkWidget *listview) {

  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkListStore *store;

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes("List Items",
          renderer, "text", LIST_ITEM, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(listview), column);

  store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);

  gtk_tree_view_set_model(GTK_TREE_VIEW(listview), GTK_TREE_MODEL(store));

  gtk_tree_view_set_enable_search(GTK_TREE_VIEW(listview), FALSE);

  g_object_unref(store);
}

void clear_list(GtkWidget* listview) {

  GtkTreeView *treeview = NULL;
  GtkTreeModel *model = NULL;
  GtkListStore *liststore = NULL;

  treeview = GTK_TREE_VIEW(listview);
  model = gtk_tree_view_get_model(treeview);
  liststore = GTK_LIST_STORE(model);

  gtk_list_store_clear(liststore);
}

gboolean
key_pressed(GtkWidget *entry, GtkWidget* listview) {
    clear_list(listview);

    gchar* needle = (gchar*)gtk_entry_get_text(GTK_ENTRY(entry));

    if (strlen(needle) > gLimit) {
        search_process(needle);
    }

    return FALSE;
}

// https://stackoverflow.com/questions/36920396/gtk-treeview-add-key-press-event-signal
// https://stackoverflow.com/questions/45899746/difference-between-connect-and-connect-after-in-pygtk
gboolean
key_pressed_listview(GtkWidget *notebook, GdkEventKey *event, gpointer userdata) {
    gchar c = (gchar)event->keyval;
    guint len;

    const gchar *phrase = gtk_entry_get_text(userdata);
    len = strlen(phrase);
    snprintf(entry_buffer, SIZE, "%s%c", phrase, c);
    gtk_entry_set_text(userdata, entry_buffer);

    //gtk_entry_set_position(userdata, 0);
    //g_object_set_property (G_OBJECT (userdata), "cursor-position", 0);

    //g_print("char: %c \n", c);
    //if (g_ascii_isprint(c)) {
    //    g_print("is alnum \n");
        gtk_entry_grab_focus_without_selecting(GTK_ENTRY(userdata));
    //    return TRUE;
    //}

    return FALSE;
}

void
row_activated (GtkTreeView       *treeview,
               GtkTreePath       *path,
               GtkTreeViewColumn *column,
               gpointer           user_data) {
    GtkTreeSelection *selection; 
    GtkTreeIter iter;
    GtkTreeModel *model;
    gchar *value;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

    if (gtk_tree_selection_get_selected(
        GTK_TREE_SELECTION(selection), &model, &iter)) {

        gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
        g_print("%s\n", value);
        g_free(value);
    }
}

int main(int argc, char *argv[]) {

  GtkWidget *window;
  GtkWidget *listview;
  GtkWidget *entry;
  GtkWidget *scroll;

  GtkWidget *vbox;
  GtkWidget *label;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  listview = gtk_tree_view_new();

  init_list(listview);

  gtk_window_set_title(GTK_WINDOW(window), "List view");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 1000);

  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(listview), FALSE);

  vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  entry = gtk_search_entry_new(); //gtk_entry_new();
  scroll = gtk_scrolled_window_new(NULL, NULL);

  gtk_container_add (GTK_CONTAINER (scroll), GTK_WIDGET (listview));

  gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 2);
  gtk_box_pack_start(GTK_BOX(vbox), scroll, TRUE, TRUE, 2);

  gtk_container_add(GTK_CONTAINER(window), vbox);

  g_signal_connect (G_OBJECT (entry), "changed", G_CALLBACK (key_pressed), listview);
  g_signal_connect (G_OBJECT (listview), "row-activated", G_CALLBACK (row_activated), NULL);
  g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  //g_signal_connect_after (G_OBJECT (listview), "key-press-event", G_CALLBACK (key_pressed_listview), entry);

  gtk_widget_show_all(window);

  g_timeout_add (400, (GSourceFunc) _main_loop, listview);

  gtk_main();

  return 0;
}
