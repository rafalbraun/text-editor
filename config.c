#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>

#include "list.h"
#include "config.h"

UserData *
cast_to_ud (gpointer userdata)
{
	return (UserData *) userdata;
}

void ud_init (UserData** userdata) {
	*userdata = g_new0 (UserData, 1);
	cast_to_ud(*userdata)->head = NULL;
	cast_to_ud(*userdata)->session_info = "~session-info";
	cast_to_ud(*userdata)->filepath = "/home/rafal/Desktop/gtksourceview-4.0.3";

	separator = "\n";

	g_print("%s \n", cast_to_ud(*userdata)->filepath);
}

GtkWindow* get_window (UserData* userdata)
{
	return GTK_WINDOW(userdata->window);
}

GtkTreeView* get_treeview (UserData* userdata)
{
	return GTK_TREE_VIEW(userdata->treeview);
}

GtkNotebook* get_notebook (UserData* userdata)
{
	return GTK_NOTEBOOK(userdata->notebook);
}

GtkSourceBuffer* get_buffer (UserData* userdata)
{
	return GTK_SOURCE_BUFFER(userdata->buffer);
}

GtkMenu* get_treeview_menu (UserData* userdata)
{
	return GTK_MENU(userdata->treeview_menu);
}

GtkMenu* get_treeview_menu_expand (UserData* userdata)
{
	return GTK_MENU(userdata->treeview_menu_expand);
}

GtkMenu* get_treeview_menu_collapse (UserData* userdata)
{
	return GTK_MENU(userdata->treeview_menu_collapse);
}

gchar*
get_text_from_eventbox(GtkWidget* widget)
{
	GList* list = gtk_container_get_children(GTK_CONTAINER(widget));
	GtkLabel* label = ((GtkLabel*) list->data);
	gchar* title = (gchar *)gtk_label_get_text(GTK_LABEL(label));
	return title;
}

void set_language(GObject* buffer) {
  GtkSourceLanguageManager *manager = gtk_source_language_manager_new ();
  gchar **lang_dirs;
  lang_dirs = g_new0 (gchar *, 6);
  lang_dirs[0] = "/home/rafal/.local/share/gtksourceview-4/language-specs ";
  lang_dirs[1] = "/home/rafal/.local/share/flatpak/exports/share/gtksourceview-4/language-specs ";
  lang_dirs[2] = "/var/lib/flatpak/exports/share/gtksourceview-4/language-specs ";
  lang_dirs[3] = "/usr/local/share/gtksourceview-4/language-specs ";
  lang_dirs[4] = "/usr/share/gtksourceview-4/language-specs";
  lang_dirs[5] = "/home/rafal/IdeaProjects/gtksourceview-my-ide/application/langs";
  lang_dirs[6] = NULL;
  gtk_source_language_manager_set_search_path (manager, lang_dirs);

  const gchar *const *lang_ids;
  lang_ids = gtk_source_language_manager_get_language_ids (manager);
  // for (int i=0; *(lang_ids+i); i++) {
  //    g_print("%d -> %s \n", i, *(lang_ids+i));
  // }
  gtk_source_buffer_set_language (GTK_SOURCE_BUFFER (buffer),
                  gtk_source_language_manager_get_language (manager,
                                        lang_ids
                                        [23]));
  //gtk_source_buffer_set_language (GTK_SOURCE_BUFFER(buffer), gtk_source_language_manager_get_language(manager, lang_ids[44]));
  gtk_source_buffer_set_highlight_syntax (GTK_SOURCE_BUFFER (buffer), TRUE);

}

void set_buffer_scheme (GObject* buffer) {
  GtkSourceStyleSchemeManager *sm = gtk_source_style_scheme_manager_new ();
  gchar **scheme_dirs;
  scheme_dirs = g_new0 (gchar *, 6);
  scheme_dirs[0] = "/home/rafal/.local/share/gtksourceview-4/styles ";
  scheme_dirs[1] =
    "/home/rafal/.local/share/flatpak/exports/share/gtksourceview-4/styles ";
  scheme_dirs[2] = "/var/lib/flatpak/exports/share/gtksourceview-4/styles ";
  scheme_dirs[3] = "/usr/local/share/gtksourceview-4/styles ";
  scheme_dirs[4] = "/usr/share/gtksourceview-4/styles";
  scheme_dirs[5] =
    "/home/rafal/IdeaProjects/gtksourceview-my-ide/application/schemes";
  scheme_dirs[6] = NULL;
  gtk_source_style_scheme_manager_set_search_path (sm, scheme_dirs);

//  set_scheme (GTK_SOURCE_BUFFER (buffer), sm);
//set_scheme (GtkSourceBuffer * buffer, GtkSourceStyleSchemeManager * sm)

  GtkSourceStyleScheme *style_scheme = NULL;
  //GtkSourceStyleSchemeManager *sm;
  const gchar *const *schemes;

  //sm = gtk_source_style_scheme_manager_get_default ();
  schemes = gtk_source_style_scheme_manager_get_scheme_ids (sm);
  schemes += 4;
  style_scheme = gtk_source_style_scheme_manager_get_scheme (sm, *schemes);
  gtk_source_buffer_set_style_scheme (GTK_SOURCE_BUFFER(buffer), style_scheme);

}


static void
set_syntax_submenu (GtkMenuItem * menuitem)
{

  GtkWidget *fileMenu;
  GtkWidget *item;
  GtkSourceLanguageManager *manager;
  const gchar *const *lang_ids;

  manager = gtk_source_language_manager_get_default ();
  lang_ids = gtk_source_language_manager_get_language_ids (manager);

  fileMenu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem), fileMenu);
  for (int i = 0; *(lang_ids + i); i++) {
    //g_print("%d -> %s \n", i, *(lang_ids+i));
    const gchar *lang_name = *(lang_ids + i);
    item = gtk_check_menu_item_new_with_label (lang_name);
    gtk_menu_shell_append (GTK_MENU_SHELL (fileMenu), item);
  }

/*
  GtkWidget *quitMi;
  GtkWidget *imprMi;
  GtkWidget *sep;
  GtkWidget *fileMenu;

  quitMi = gtk_menu_item_new_with_label("Quit");
  imprMi = gtk_menu_item_new_with_label("Import");
  sep = gtk_separator_menu_item_new();

  fileMenu = gtk_menu_new();
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), fileMenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), imprMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), sep);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), quitMi);
*/
  gtk_widget_show_all (GTK_WIDGET (menuitem));
}

void
list_schemes (GtkSourceStyleSchemeManager * sm)
{
  GtkSourceStyleScheme *style_scheme = NULL;
  const gchar *const *schemes;

  //sm = gtk_source_style_scheme_manager_get_default ();
  schemes = gtk_source_style_scheme_manager_get_scheme_ids (sm);
  g_print ("Available style schemes:\n");
  while (*schemes != NULL) {
    const gchar *const *authors;
    gchar *authors_str = NULL;

    style_scheme = gtk_source_style_scheme_manager_get_scheme (sm, *schemes);

    authors = gtk_source_style_scheme_get_authors (style_scheme);
    if (authors != NULL)
      authors_str = g_strjoinv (", ", (gchar **) authors);

    g_print (" - [%s] %s: %s\n",
	     gtk_source_style_scheme_get_id (style_scheme),
	     gtk_source_style_scheme_get_name (style_scheme),
	     gtk_source_style_scheme_get_description (style_scheme) ?
	     gtk_source_style_scheme_get_description (style_scheme) : "");

    if (authors_str) {
      g_print ("   by %s\n", authors_str);
      g_free (authors_str);
    }

    ++schemes;
  }
  g_print ("\n");
}

void
set_langs_dir (GtkSourceBuffer * buffer)
{
  GtkSourceLanguageManager *manager;
  const gchar *const *search_path;

  manager = gtk_source_language_manager_get_default ();
  search_path = gtk_source_language_manager_get_search_path (manager);

  int i = 0;
  while (search_path[i]) {
    g_print ("%s \n", search_path[i++]);
  }

}


void
full_search_cb (GtkWidget * widget, gpointer userdata)
{
  char *argv[15];
  int status;
  GPid child_pid;

  memset (argv, 0, 15 * sizeof (char *));
  argv[0] = "./full_search";
  argv[1] = ".";
  status =
    g_spawn_async_with_pipes (NULL, argv, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL,
			      &child_pid, NULL, NULL, NULL, NULL);

  if (!status) {
    g_print ("[FAILED][%s] 1 Failed to run %s: %d \n", g_get_application_name (), argv[0], status);
  }
  if (child_pid == 0) {
    g_print ("[FAILED][%s] 2 child pid not returned \n", g_get_application_name ());
  }
}

void
find_files_cb (GtkWidget * widget, gpointer userdata)
{
  char *argv[15];
  int status;
  GPid child_pid;

  memset (argv, 0, 15 * sizeof (char *));
  argv[0] = "./find_files_gui";

  status = g_spawn_async_with_pipes (NULL, argv, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL,
                  &child_pid, NULL, NULL, NULL, NULL);

  if (!status) {
    g_print ("[FAILED][%s] 1 Failed to run %s: %d \n", g_get_application_name (), argv[0], status);
  }
  if (child_pid == 0) {
    g_print ("[FAILED][%s] 2 child pid not returned \n", g_get_application_name ());
  }
}

/*
gboolean
key_pressed_notebook (GtkWidget * notebook, GdkEventKey * event, gpointer userdata)
{
  g_timer_reset (timer);
  is_saved = FALSE;
  g_print ("[INFO] Timer reset.\n");
}
*/

void
show_error (GtkWindow * window, gchar * message)
{

  GtkWidget *dialog;
  dialog = gtk_message_dialog_new (GTK_WINDOW (window),
				   GTK_DIALOG_DESTROY_WITH_PARENT,
				   GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, message);
  gtk_window_set_title (GTK_WINDOW (dialog), "Error");
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}
