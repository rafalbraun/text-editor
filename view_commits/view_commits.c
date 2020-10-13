#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>

#include <git2.h>
#define REPO "/home/rafal/IdeaProjects/gtksourceview-my-ide/application"

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

void visit(git_commit *c)
{
  size_t i, num_parents = git_commit_parentcount(c);

  /* Print some stuff about this commit */
  char oidstr[42] = {0};
  git_oid_tostr(oidstr, 41, git_commit_id(c));
  //printf("%s\n%s\n\n", oidstr, git_commit_message(c));
  printf("%s :: %s", oidstr, git_commit_message(c));

  for (i=0; i<num_parents; i++) {
    /* Do the same for the parents */
    git_commit *p;
    if (!git_commit_parent(&p, c, i)) {
      visit(p);
    }
    git_commit_free(p);
  }
}

// https://ben.straub.cc/2013/10/02/revwalk/
static void 
git_init() {
    git_libgit2_init();

    git_repository *repo = NULL;
    int error = git_repository_open(&repo, REPO);

    if (error < 0) {
        //const git_error *e = git_error_last();
        const git_error *e = giterr_last();
        printf("Error %d/%d: %s\n", error, e->klass, e->message);
        exit(error);
    }

    git_commit * commit = git_get_head(repo);
    visit (commit);
    git_commit_free (commit);
}

int
main (int   argc,
      char *argv[])
{
  GtkBuilder *builder;
  GObject *window;
  GObject *button;
  GError *error = NULL;

  gtk_init (&argc, &argv);
  git_init ();

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
