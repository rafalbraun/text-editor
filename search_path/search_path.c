#include <gtk/gtk.h>

#include "../glib_regex.c"

int counter = 0;

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

/*
void *topfun( void *userdata ) {
  printf("counting starting %d\n", c);
  for (int i=0;i<5;i++) {
    printf("adding %d::%d \n", c, i);
  }
  printf("counting ended %d\n", c);
  c++;
}
*/
/*
static void
child_exit_cb (GPid pid, gint status, gpointer data)
{
  //final_status = status;
  //g_main_loop_quit (main_loop);
  g_print("exit\n");
}*/

gboolean
_stop_main_loop (gpointer user_data)
{
  //g_main_loop_quit (loop);
  g_print("time %d\n", counter);
  counter++;

  return TRUE;
}

int spawn() {
  char *argv [15];
  int stdout_fd = -1;
  char buffer [512];
  //pid_t child_pid = 0;
  GPid child_pid;
  int status;

  memset (argv, 0, 15 * sizeof (char *));
  argv [0] = "ls";
  argv[1] = "-lR";
  argv[2] = "/home/rafal/IdeaProjects/gtksourceview-my-ide/application/search_path";
  status = g_spawn_async_with_pipes (NULL, argv, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, &child_pid, NULL, &stdout_fd, NULL, NULL);
  if (!status) {
    g_print("[FAILED] 1 Failed to run ls: %d \n", status);
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

  while (read (stdout_fd, buffer, 512) > 0) {
    g_print("[BUFFER]\n%s\n***", buffer);
  }

  close (stdout_fd);

  return 0;

  /*
  gchar *out;
  gchar *err;
  gint status = -1;
  GError *error = NULL;

  if (!g_spawn_command_line_sync ("ls -lR /home/rafal", &out, &err, &status, &error)) {
    g_print("[FAILED] Error executing 'ls' \n");
    return 1;
  }

  if (status != 0) {
    g_print("[FAILED] Status is %d \n", status);
    return 1;
  }

  if (out == NULL || strlen (out) == 0) {
    g_print("[FAILED] Didn't get any output from ls!? \n");
    return 1;
  }

  g_print(out);

  g_free (out);
  g_free (err);
  return 0;
  */
}

void
preedit_changed (GtkEntry *widget, gpointer userdata)  {
  g_print("preedit_changed %s \n", gtk_entry_get_text(GTK_ENTRY(widget)));
  //add_to_list(userdata, "string000000000", "string11111111111");

  spawn();
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
  //g_signal_connect (window, "key-press-event", G_CALLBACK (key_pressed_window), NULL);

  treeview = gtk_builder_get_object(builder, "treeview1");
  g_signal_connect(treeview, "row-activated", G_CALLBACK(row_activated), NULL);

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
  g_signal_connect(selection, "changed", G_CALLBACK(on_changed), NULL);

  entry = gtk_builder_get_object (builder, "entry");
  g_signal_connect (entry, "changed", G_CALLBACK (preedit_changed), treeview);

  add_to_list(treeview, "string000000000", "string11111111111");
  add_to_list(treeview, "string000000000", "string11111111111");


  g_timeout_add (100,
           (GSourceFunc) _stop_main_loop,
           NULL);






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
