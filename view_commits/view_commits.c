#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>

#include <git2.h>

#define REPO "/home/rafal/IdeaProjects/gtksourceview-my-ide/application"
#define SIZE 1024

static void
print_hello (GtkWidget *widget,
             gpointer   data)
{
  g_print ("Hello World\n");
}

// https://stackoverflow.com/questions/15717625/how-to-get-the-last-commit-from-head-in-a-git-repository-using-libgit2
// https://stackoverflow.com/questions/18134627/how-much-of-a-git-sha-is-generally-considered-necessary-to-uniquely-identify-a
// https://ben.straub.cc/2013/10/02/revwalk/
// https://github.com/libgit2/libgit2/tree/master/examples
// https://stackoverflow.com/questions/17914375/listing-all-commits-in-a-branch-using-libgit2
// https://libgit2.org/docs/guides/101-samples/
// https://libgit2.org/libgit2/ex/HEAD/general.html
// https://www.chromium.org/developers/fast-intro-to-git-internals

static git_commit*
git_get_head(git_repository *repo) {
    int rc;
    git_commit * commit = NULL; /* the result */
    git_oid oid_parent_commit;  /* the SHA1 for last commit */

    /* resolve HEAD into a SHA1 */
    rc = git_reference_name_to_id( &oid_parent_commit, repo, "HEAD" );
    if ( rc == 0 )
    {
      /* get the actual commit structure */
      rc = git_commit_lookup( &commit, repo, &oid_parent_commit );
      if ( rc == 0 )
      {
        return commit;
      }
    }

    return NULL;
}

void add_to_list(GtkListStore *liststore, const gchar *str0, const guint num1) {
  GtkTreeIter iter;

  gtk_list_store_append(liststore, &iter);
  gtk_list_store_set(liststore, &iter, 0, str0, 1, num1, -1);
}

void visit(git_commit *commit, GtkListStore* liststore, int* level)
{
  size_t i, num_parents = git_commit_parentcount(commit);
  gchar buffer[SIZE];

  /* Print some stuff about this commit */
  char oidstr[42] = {0};
  git_oid_tostr(oidstr, 41, git_commit_id(commit));
  //printf("%s\n%s\n\n", oidstr, git_commit_message(c));
  //printf("%s :: %s", oidstr, git_commit_message(c));

  const git_signature *author, *cmtter;
  gchar *message, *s_time;
  time_t time;

  message  = (gchar*)git_commit_message(commit);
  author   = git_commit_author(commit);
  cmtter   = git_commit_committer(commit);
  time     = git_commit_time(commit);

  message[strlen(message)-1] = '\0';
  s_time = ctime(&time);
  s_time[strlen(s_time)-1] = '\0';
  //index = g_strdup_printf("%d", *level);

  //snprintf(buffer, SIZE, "%s :: %s", oidstr, git_commit_message(c));
  //snprintf(buffer, SIZE, "%s", git_commit_message(commit));
  //printf("%s %s\n", message, s_time);
  //printf("%s %s\n", git_commit_message(commit), ctime(&time));
  snprintf(buffer, SIZE, "%s \n [HEAD~%d] %s", s_time, *level, message);
  add_to_list (GTK_LIST_STORE(liststore), buffer, *level);


  //printf("%s %s \n", oidstr, ctime(&time));
  /*
  printf("Author: %s (%s)\nCommitter: %s (%s)\nDate: %s\nMessage: %s\n",
    author->name, author->email,
    cmtter->name, cmtter->email,
    ctime(&time), message);
  */
  *level += num_parents;

  for (i=0; i<num_parents; i++) {
    /* Do the same for the parents */
    git_commit *p;
    if (!git_commit_parent(&p, commit, i)) {
      visit(p, liststore, level);
    }
    git_commit_free(p);
  }
}

// https://ben.straub.cc/2013/10/02/revwalk/
static int 
git_init(GtkListStore* liststore) {
    git_libgit2_init();

    git_repository *repo = NULL;
    int error = git_repository_open(&repo, REPO);
    int level = 0;

    if (error < 0) {
        //const git_error *e = git_error_last();
        const git_error *e = giterr_last();
        printf("Error %d/%d: %s\n", error, e->klass, e->message);
        exit(error);
    }

    git_commit * commit = git_get_head(repo);
    visit (commit, liststore, &level);
    git_commit_free (commit);

    g_print("level %d \n", level);




    git_object *obj = NULL;
    error = git_revparse_single(&obj, repo, "HEAD^{tree}");
    git_tree *tree = (git_tree *)obj;

/*
    size_t count = git_tree_entrycount(tree);
    g_print ("count %d \n", count);

    for (int i=0; i<count; i++) {
        const git_tree_entry *entry = git_tree_entry_byindex(tree, i);
        //const char *name = git_tree_entry_name(entry);
        //g_print("entry: %s \n", entry);
        git_otype objtype = git_tree_entry_type(entry);
        g_print("%d -> %d \n", i, objtype);
    }

    g_print("GIT_OBJ_COMMIT:  %d \n", GIT_OBJ_COMMIT);
    g_print("GIT_OBJ_TREE:    %d \n", GIT_OBJ_TREE);
*/


    git_tree_entry *entry = NULL;
    error = git_tree_entry_bypath(&entry, tree, "README.md");
    git_tree_entry_free(entry); /* caller has to free this one */

    const char *name = git_tree_entry_name(entry);
    g_print("name <%s> \n", name);



    return level;
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
    guint index;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

    if (gtk_tree_selection_get_selected(
        GTK_TREE_SELECTION(selection), &model, &iter)) {

        gtk_tree_model_get(model, &iter, 0, &value,  -1);
        gtk_tree_model_get(model, &iter, 1, &index,  -1);

        g_print("%s :: %d \n", value, index);
        g_free(value);
    }
}

/**
 * git commands:
 * 
 * https://stackoverflow.com/questions/424071/how-to-list-all-the-files-in-a-commit
 * git ls-tree --name-only -r cef053f45f5c0d60244c8f3f4a52f8d76a376dfa
 * 
 * https://stackoverflow.com/questions/338436/how-can-i-view-an-old-version-of-a-file-with-git
 * git show HEAD~4:notebook.c
 * 
 */
int
main (int argc, char *argv[])
{
  GtkBuilder *builder;
  GObject *window;
  GObject *liststore;
  GObject *treeview;
  GError *error = NULL;
  int commits_num;

  gtk_init (&argc, &argv);

  /* Construct a GtkBuilder instance and load our UI description */
  builder = gtk_builder_new ();
  if (gtk_builder_add_from_file (builder, "view_commits.ui", &error) == 0)
    {
      g_printerr ("Error loading file: %s\n", error->message);
      g_clear_error (&error);
      return 1;
    }

  /* Connect signal handlers to the constructed widgets. */
  window = gtk_builder_get_object (builder, "window");
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  liststore = gtk_builder_get_object (builder, "liststore");
  commits_num = git_init (GTK_LIST_STORE(liststore));

  treeview = gtk_builder_get_object (builder, "treeview");
  g_signal_connect (G_OBJECT (treeview), "row-activated", G_CALLBACK (row_activated), NULL);

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
