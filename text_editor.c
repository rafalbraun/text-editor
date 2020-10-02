#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#define TREEVIEW 1
#define NOTEBOOK 1
#define FULL_SEARCH 1

#include <gtksourceview/gtksource.h>
#include <gtk/gtk.h>

GObject *buffer;
GObject *window;

void show_error(GObject* window, gchar* message) {
  
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            message);
  gtk_window_set_title(GTK_WINDOW(dialog), "Error");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

#include "treeview.c"
#include "notebook.c"

gboolean
key_pressed_treeview(GtkWidget *notebook, GdkEventKey *event, gpointer userdata) 
{
  return FALSE;
}

gboolean
key_pressed_window(GtkWidget *notebook, GdkEventKey *event, gpointer userdata) 
{
  if (event->state & GDK_CONTROL_MASK && event->keyval == 'F') {
      g_print("ctrl + shift + f \n");
  }

  return FALSE;
}


static void
guess_language(GtkSourceBuffer* buffer, gchar* filepath) {
    GtkSourceLanguageManager *manager;
    GtkSourceLanguage *lang = NULL;
    gboolean result_uncertain;
    gchar *content_type;

    manager = gtk_source_language_manager_get_default ();
    content_type = g_content_type_guess (filepath, NULL, 0, &result_uncertain);
    if (result_uncertain) {
        g_free (content_type);
        return;
    }

    lang = gtk_source_language_manager_guess_language (manager, filepath, content_type);
    if (lang != NULL) {
        g_print("lang recognized: %s \n", gtk_source_language_get_name(lang));
        gtk_source_buffer_set_language (buffer, lang);
        g_free (content_type);
    }
}

void clear_buffer(GtkSourceBuffer* buffer) {
    GtkTextIter iter_start, iter_end;

    gtk_text_buffer_get_start_iter (GTK_TEXT_BUFFER (buffer), &iter_start);
    gtk_text_buffer_get_end_iter   (GTK_TEXT_BUFFER (buffer), &iter_end);
    gtk_text_buffer_delete ( GTK_TEXT_BUFFER( buffer ), &iter_start, &iter_end );
}

GtkWidget*
sourceview_new(GtkSourceBuffer* buffer) {
    GtkWidget *scroll, *sourceview;

    scroll = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    sourceview = gtk_source_view_new_with_buffer(buffer);
    gtk_source_view_set_show_right_margin(GTK_SOURCE_VIEW(sourceview), TRUE);
    gtk_source_view_set_show_line_numbers(GTK_SOURCE_VIEW(sourceview), TRUE);
    gtk_source_view_set_highlight_current_line(GTK_SOURCE_VIEW(sourceview), TRUE);
    gtk_container_add (GTK_CONTAINER (scroll), GTK_WIDGET (sourceview));

    /* change font */
    GtkCssProvider *provider = gtk_css_provider_new ();
    gtk_css_provider_load_from_data (provider,
                                     "textview { font-family: Monospace; font-size: 12pt; }",
                                     -1,
                                     NULL);
    gtk_style_context_add_provider (gtk_widget_get_style_context (sourceview),
                                    GTK_STYLE_PROVIDER (provider),
                                    GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref (provider);

    return scroll;
}

gboolean 
on_button_pressed(GtkWidget *treeview, GdkEventButton *event, gpointer userdata)
{
  GtkTreeSelection *selection;
  GtkTreeModel     *model;
  GtkTreeIter       child;
  GtkTreeIter       parent;
  gboolean          hasParent;
  gchar            *path, *name, *parent_name;
  GtkWidget* content;

  if (event->type == GDK_2BUTTON_PRESS) {
      GtkTreeSelection * selection;
      selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

      if (gtk_tree_selection_count_selected_rows(selection) == 1) {

        model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
        gtk_tree_selection_get_selected(selection, &model, &child);

        path = "";
        gtk_tree_model_get (model, &child, COLUMN, &name, -1);

        while ( (hasParent = gtk_tree_model_iter_parent(model, &parent, &child)) == TRUE ) {
          if ( hasParent == TRUE ) {
            parent_name = "";
            gtk_tree_model_get (model, &parent, COLUMN, &parent_name, -1);
            path = g_strconcat(parent_name, "/", path, NULL);
            g_free(parent_name);
            child = parent;
          }
        }

        path = g_strconcat(path, name, NULL);

        content = sourceview_new(GTK_SOURCE_BUFFER(buffer));
        guess_language(GTK_SOURCE_BUFFER(buffer), path);

      	if ( g_file_test(path, G_FILE_TEST_IS_DIR) == FALSE ) {
      		  if ( g_file_test(path, G_FILE_TEST_EXISTS) == TRUE ) {
              		open_file (GTK_NOTEBOOK(userdata), path, content, buffer);
      		  } else {
                show_error(window, "no file under filepath");
      		  }
      	} else {
            show_error(window, "filepath is directory");
      	}

        g_free(name);
        g_free(path);

      }

      return TRUE;
    } else if (event->type == GDK_BUTTON_PRESS && event->button == 3) {
        if (1) {
            GtkTreeSelection * selection;

            selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
            if (gtk_tree_selection_count_selected_rows(selection) <= 1) {
                GtkTreePath * path;

            if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(treeview),
                event->x, event->y, &path, NULL, NULL, NULL)) {
                gtk_tree_selection_unselect_all(selection);
                gtk_tree_selection_select_path(selection, path);
                gtk_tree_path_free(path);
            }
        }
    }

    popup_menu(treeview, event, userdata);

    return TRUE;
  }

  return FALSE;
}

int
main (int   argc,
      char *argv[])
{
  GtkBuilder *builder;
  GObject *button;
  GObject *treeview;
  GObject *treestore;
  GObject *notebook;
  GError *error = NULL;
  gchar* filepath = ".";

  gtk_init (&argc, &argv);

  /* Construct a GtkBuilder instance and load our UI description */
  builder = gtk_builder_new ();
  if (gtk_builder_add_from_file (builder, "text_editor.ui", &error) == 0)
  {
    g_printerr ("Error loading file: %s\n", error->message);
    g_clear_error (&error);
    return 1;
  }

  /* Connect signal handlers to the constructed widgets. */
  window = gtk_builder_get_object (builder, "window");
  buffer = gtk_builder_get_object (builder, "sourcebuffer");
  treeview = gtk_builder_get_object (builder, "treeview");
  treestore = gtk_builder_get_object (builder, "treestore");
  notebook = gtk_builder_get_object (builder, "notebook");

  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
  g_signal_connect (window, "key-press-event", G_CALLBACK (key_pressed_window), NULL);

  //create_view_and_model(filepath, GTK_WIDGET(treeview));
  fill_treestore_new(GTK_TREE_STORE(treestore), filepath);
  expand_top_node (treeview);

  //gtk_tree_view_set_enable_tree_lines(GTK_TREE_VIEW(treeview), TRUE);
  /*
                              <property name="enable-grid-lines">GTK_TREE_VIEW_GRID_LINES_BOTH</property>
  */
  gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(treeview), GTK_TREE_VIEW_GRID_LINES_BOTH);
  g_signal_connect (treeview, "key-press-event", G_CALLBACK (key_pressed_treeview), NULL);

  g_signal_connect (G_OBJECT (treeview), "button-press-event", G_CALLBACK (on_button_pressed), (gpointer)notebook);
  g_signal_connect (G_OBJECT (notebook), "switch-page", G_CALLBACK (switch_page), (gpointer)buffer);

  gtk_main ();

  return 0;
}
