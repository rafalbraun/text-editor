#include <gtk/gtk.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_SIZE 20000
#define SEARCH_PATH 1

#include "../glib_regex.c"

int counter = 0;

// needed by spawn
int stdout_fd = -1;
char buffer [BUFFER_SIZE];

gchar* 
extract_filename(gchar* filepath) {
  gchar*  filename;
  gchar** tokens;
  int     tokennum;

  tokens = g_strsplit(filepath, "/", -1);
  tokennum = g_strv_length(tokens);
  filename = g_strdup(tokens[tokennum-1]);
  g_strfreev(tokens);

  return filename;
}

gchar* 
read_file(gchar* filename) {
  gchar *contents;
  gsize len;
  GError *err = NULL;

  if (g_file_get_contents(filename, &contents, &len, &err) == FALSE) {
    g_error("error reading %s: %s", filename, err->message);
  }
  //bytes = g_bytes_new_take(contents, len);

  return contents;
}

// widget is of type GtkTreeView
void add_to_list(GObject* widget, gchar *str0, gchar *str1, gchar* str2) {
  GtkTreeView *treeview = NULL;
  GtkTreeModel *model = NULL;
  GtkListStore *liststore = NULL;
  GtkTreeIter iter;

  treeview = GTK_TREE_VIEW(widget);
  model = gtk_tree_view_get_model(treeview);
  liststore = GTK_LIST_STORE(model);

  gtk_list_store_append(liststore, &iter);
  gtk_list_store_set(liststore, &iter, 0, str0, 1, str1, 2, str2, -1);
}

void clear_list(GObject* widget) {
  GtkTreeView *treeview = NULL;
  GtkTreeModel *model = NULL;
  GtkListStore *liststore = NULL;

  treeview = GTK_TREE_VIEW(widget);
  model = gtk_tree_view_get_model(treeview);
  liststore = GTK_LIST_STORE(model);

  gtk_list_store_clear(liststore);
}

gboolean
_stop_main_loop (gpointer treeview)
{
  char** lines;
  char** line;
  char*  contents;
  int    bytes;

  //g_print("time %d \n", counter);
  counter++;

  bytes = read (stdout_fd, buffer, BUFFER_SIZE);
  buffer[bytes-1] = '\0';
  if (bytes > 0) {
    lines = g_strsplit(buffer, "\n", -1);
    for (int i=0 ; i<g_strv_length(lines); i++) {
      line = g_strsplit(lines[i], "\x1C", 0);
      //add_to_list(treeview, line[4], extract_filename(line[0]), line[1]);
      add_to_list(treeview, line[4], (line[0]), line[1]);
      g_strfreev(line);
    }
    g_strfreev(lines);
  }
  memset(buffer, 0, sizeof(buffer));

  return TRUE;
}

int spawn() {
  char *argv [15];
  GPid child_pid;
  int status;

  memset (argv, 0, 15 * sizeof (char *));
  argv[0] = "/home/rafal/IdeaProjects/gtksourceview-my-ide/application/glib_regex";
  argv[1] = "/home/rafal/IdeaProjects/gtksourceview-my-ide/application/search_path";
  argv[2] = "int";
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

void
preedit_changed (GtkEntry *widget, gpointer treeview)  {
  //g_print("preedit_changed %s \n", gtk_entry_get_text(GTK_ENTRY(widget)));
  //add_to_list(userdata, "string000000000", "string11111111111");
  clear_list(treeview);

  gchar* needle = (gchar*)gtk_entry_get_text(GTK_ENTRY(widget));
  if (strlen(needle)>2) {
    spawn(needle);
  }
}

void on_changed(GtkTreeSelection *widget, gpointer textbuffer) {
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *filename;
  gchar *contents;
  gsize len;
  GError *err = NULL;

  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(widget), &model, &iter)) {

    gtk_tree_model_get(model, &iter, 1, &filename,  -1);
    //gtk_label_set_text(GTK_LABEL(label), value);
    g_print("value: %s \n", filename);

    if (g_file_get_contents(filename, &contents, &len, &err) == FALSE) {
      g_error("error reading %s: %s", filename, err->message);
    }

    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(textbuffer), contents, len);
    g_free(filename);
  }
}

void
row_activated (GtkTreeView       *tree_view,
               GtkTreePath       *path,
               GtkTreeViewColumn *column,
               gpointer           user_data) {
  g_print("row activated \n");
}
/*
gboolean
key_pressed_window(GtkWidget *notebook, GdkEventKey *event, gpointer userdata) 
{
  g_print("key_pressed: GtkWindow \n");
  return FALSE;
}
*/
int
main (int   argc,
      char *argv[])
{
  GtkBuilder *builder;
  GObject *window, *entry, *treeview, *textbuffer;
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
  //g_signal_connect (window, "key-press-event", G_CALLBACK (key_pressed_window), NULL);

  treeview = gtk_builder_get_object(builder, "treeview1");
  g_signal_connect(treeview, "row-activated", G_CALLBACK(row_activated), NULL);

  textbuffer = gtk_builder_get_object (builder, "textbuffer");
  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
  g_signal_connect(selection, "changed", G_CALLBACK(on_changed), textbuffer);

  entry = gtk_builder_get_object (builder, "entry");
  g_signal_connect (entry, "changed", G_CALLBACK (preedit_changed), treeview);


  g_timeout_add (100,
           (GSourceFunc) _stop_main_loop,
           treeview);






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
