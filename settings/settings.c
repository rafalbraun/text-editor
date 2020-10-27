#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>

#include <git2.h>

#define REPO "/home/rafal/IdeaProjects/gtksourceview-my-ide/application"
#define SIZE 1024

gchar* launch_command(gchar* command) {
    gchar* fname = "/tmp/clipboard";
    gchar *contents;
    gsize len;
    GError *err = NULL;

    // "git show HEAD~%d:%s > %s \n"
    gchar* tmp = g_strdup_printf("%s > %s", command, fname);
    system(tmp);

    if (g_file_get_contents(fname, &contents, &len, &err) == FALSE) {
        g_error("error reading %s: %s", fname, err->message);
        return NULL;
    }

    return contents;
}

void add_to_list(GtkListStore *liststore, const gchar *str0, const guint num1, const gchar *str2) {
  GtkTreeIter iter;

  gtk_list_store_append(liststore, &iter);
  gtk_list_store_set(liststore, &iter, 0, str0, 1, num1, 2, str2, -1);
}

// https://ben.straub.cc/2013/10/02/revwalk/
static int 
git_init(GtkListStore* liststore) {
    //git_libgit2_init();
    gchar* result;
    gchar* command;
    gchar* hash;
    gchar* message;
    gchar* date;
    gchar* ago;
    gchar buffer[SIZE];

    result = launch_command ("git rev-list --no-merges --count HEAD");
    gint result_int = g_ascii_strtoll (result, NULL, 10);

    for (int i=0; i<result_int; i++) {
        command = g_strdup_printf("git rev-parse HEAD~%d", i);
        hash = launch_command (command);
        hash[40] = '\0';

        command = g_strdup_printf("git log --format=%B -n 1 HEAD~%d", i);
        message = launch_command (command);

        command = g_strdup_printf("git log --format=%%ai -n 1 HEAD~%d", i);
        date = launch_command (command);
        date[20] = '\0';

        command = g_strdup_printf("git log --format=%%ar -n 1 HEAD~%d", i);
        ago = launch_command (command);
        ago[strlen(ago)-1] = '\0';

        snprintf(buffer, SIZE, "%s (%s) \n [HEAD~%d] %s", date, ago, i, message);
        add_to_list (GTK_LIST_STORE(liststore), buffer, i, "Makefile");
    }

    return result_int;
}

void on_changed(GtkTreeSelection *selection, gpointer sourcebuffer) {
    GtkTreeIter iter;
    GtkTreeModel *model;
    gchar *contents;
    gchar *command;
    gchar *value;
    gchar *fname;
    guint index;
    gsize len;
    GError *err = NULL;

    if (gtk_tree_selection_get_selected(
        GTK_TREE_SELECTION(selection), &model, &iter)) {

        gtk_tree_model_get(model, &iter, 0, &value,  -1);
        g_free(value);
    }
}


int
main (int argc, char *argv[])
{
  GtkBuilder *builder;
  GObject *window;
  GObject *liststore;
  GObject *treeview;
  GObject *textbuffer;
  GtkTreeSelection *selection; 
  GError *error = NULL;
  int commits_num;

  gtk_init (&argc, &argv);

  /* Construct a GtkBuilder instance and load our UI description */
  builder = gtk_builder_new ();
  if (gtk_builder_add_from_file (builder, "settings.ui", &error) == 0)
    {
      g_printerr ("Error loading file: %s\n", error->message);
      g_clear_error (&error);
      return 1;
    }

  /* Connect signal handlers to the constructed widgets. */
  window = gtk_builder_get_object (builder, "window");
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  liststore = gtk_builder_get_object (builder, "liststore");
  //commits_num = git_init (GTK_LIST_STORE(liststore));

  textbuffer = gtk_builder_get_object (builder, "textbuffer");
  treeview = gtk_builder_get_object (builder, "treeview");
  //g_signal_connect (G_OBJECT (treeview), "row-activated", G_CALLBACK (row_activated), textbuffer);

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
  g_signal_connect(selection, "changed", G_CALLBACK(on_changed), textbuffer);

  gtk_main ();

  return 0;
}

// git show HEAD~80:view_commits/view_commits.c



