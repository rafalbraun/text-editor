#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>

void add_to_list(GtkListStore *liststore, const gchar *str0) {
  GtkTreeIter iter;

  gtk_list_store_append(liststore, &iter);
  gtk_list_store_set(liststore, &iter, 0, str0, -1);
}

/*
void clear_list(GtkWidget* widget) {

  GtkTreeView  *treeview = GTK_TREE_VIEW(widget);
  GtkTreeModel *model = gtk_tree_view_get_model(treeview);
  GtkListStore *liststore = GTK_LIST_STORE(model);

  gtk_list_store_clear(liststore);
}
*/
int list_directory (gchar* dirname, GtkListStore *liststore) {

    struct dirent *dp;
    DIR *dir = opendir(dirname);

    // Unable to open directory stream
    if (!dir) {
        return 0;
    }

    while ((dp = readdir(dir)) != NULL)
    {
        if (g_strcmp0(dp->d_name, ".") != 0 && g_strcmp0(dp->d_name, "..") != 0 && (dp->d_name[0]!='.'))
        {
            //snprintf(path, SIZE, "%s/%s", dirname, dp->d_name); // create name of subdirectory
            //g_print ("%s \n", dp->d_name);
            add_to_list (liststore, dp->d_name);
        }
    }

    closedir(dir);
}

void on_changed(GtkTreeSelection *selection, GtkListStore liststore) {
    GtkTreeIter iter;
    GtkTreeModel *model;
    gchar *value;
    GError *err = NULL;

    if (gtk_tree_selection_get_selected(
        GTK_TREE_SELECTION(selection), &model, &iter)) {

        gtk_tree_model_get(model, &iter, 0, &value,  -1);
        g_print("%s \n", value);
        g_free(value);
    }
}

// git show HEAD~80:view_commits/view_commits.c

/*

GDateTime *dt = g_date_time_new_from_unix_utc (time_val);
iso8601_string = g_date_time_format_iso8601 (dt);
g_date_time_unref (dt);

https://developer.gnome.org/glib/stable/glib-Date-and-Time-Functions.html#g-time-val-to-iso8601

*/
int
main (int argc, char *argv[])
{
  GtkBuilder *builder;
  GObject *window;
  GObject *liststore;
  GObject *treeview;
  GtkTreeSelection *selection; 
  GError *error = NULL;
  int commits_num;

  gtk_init (&argc, &argv);

  /* Construct a GtkBuilder instance and load our UI description */
  builder = gtk_builder_new ();
  if (gtk_builder_add_from_file (builder, "history.ui", &error) == 0)
    {
      g_printerr ("Error loading file: %s\n", error->message);
      g_clear_error (&error);
      return 1;
    }

  /* Connect signal handlers to the constructed widgets. */
  window = gtk_builder_get_object (builder, "window");
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  liststore = gtk_builder_get_object (builder, "liststore");
  treeview = gtk_builder_get_object (builder, "treeview");
  //g_signal_connect (G_OBJECT (treeview), "row-activated", G_CALLBACK (row_activated), textbuffer);

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
  g_signal_connect(selection, "changed", G_CALLBACK(on_changed), liststore);


    gtk_list_store_clear(GTK_LIST_STORE(liststore));
    list_directory ("./backup", GTK_LIST_STORE(liststore));

  gtk_main ();

  return 0;
}
