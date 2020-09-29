#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#define TREEVIEW 1
#define NOTEBOOK 1

#include <gtksourceview/gtksource.h>
#include <gtk/gtk.h>

//static void open_file (GtkNotebook* notebook, gchar* filepath);

#include "treeview.c"
#include "notebook2.c"
/*
static void
print_hello (GtkWidget *widget,
             gpointer   data)
{
  GList* list = gtk_container_get_children(GTK_CONTAINER(widget));
  GtkLabel* label = ((GtkLabel*) list->data);
  const gchar* text = gtk_label_get_text(GTK_LABEL(label));
  g_print("label: %s \n", text);
}
*/

GObject *buffer;

gboolean
key_pressed_treeview(GtkWidget *notebook, GdkEventKey *event, gpointer userdata) 
{
  g_print("key_pressed_treeview \n");
  return FALSE;
}

gboolean
key_pressed_window(GtkWidget *notebook, GdkEventKey *event, gpointer userdata) 
{
  g_print("key_pressed_window \n");
  return FALSE;
}


GtkWidget*
sourceview_new(GtkSourceBuffer* buffer) {
    GtkWidget *scroll, *sourceview;

    scroll = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    sourceview = gtk_source_view_new_with_buffer(buffer);
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
        //g_print ("on_button_pressed: %s\n", path);

        content = sourceview_new(GTK_SOURCE_BUFFER(buffer));
        open_file (GTK_NOTEBOOK(userdata), path, content);

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
            event->x, event->y, &path, NULL, NULL, NULL))
        {
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
  GObject *window;
  GObject *button;
  GObject *treeview;
  GObject *notebook;
  GError *error = NULL;
  GtkWidget *event_box;
  GtkWidget *view;
  //gchar* filepath = "/home/rafal/IdeaProjects/gtksourceview-my-ide/application";
  gchar* filepath = ".";

  gtk_init (&argc, &argv);

  /* Construct a GtkBuilder instance and load our UI description */
  builder = gtk_builder_new ();
  if (gtk_builder_add_from_file (builder, "app.ui", &error) == 0)
  {
    g_printerr ("Error loading file: %s\n", error->message);
    g_clear_error (&error);
    return 1;
  }

  /* Connect signal handlers to the constructed widgets. */
  window = gtk_builder_get_object (builder, "window");
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
  g_signal_connect (window, "key-press-event", G_CALLBACK (key_pressed_window), NULL);

  buffer = gtk_builder_get_object (builder, "sourcebuffer");

  //event_box = gtk_event_box_new ();
  /*
  widget = gtk_builder_get_object (builder, "tab0");
  g_signal_connect (widget, "button-press-event", G_CALLBACK (print_hello), NULL);

  widget = gtk_builder_get_object (builder, "tab1");
  g_signal_connect (widget, "button-press-event", G_CALLBACK (print_hello), NULL);
  */

  treeview = gtk_builder_get_object (builder, "treeview");
  create_view_and_model(filepath, GTK_WIDGET(treeview));
  gtk_tree_view_set_enable_tree_lines(GTK_TREE_VIEW(treeview), TRUE);
  gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(treeview), GTK_TREE_VIEW_GRID_LINES_BOTH);
  g_signal_connect (treeview, "key-press-event", G_CALLBACK (key_pressed_treeview), NULL);

  notebook = gtk_builder_get_object (builder, "notebook");
  //open_file (GTK_NOTEBOOK(notebook), "/tmp/Makefile");

  g_signal_connect (treeview, "button-press-event", G_CALLBACK (on_button_pressed), (gpointer)notebook);

  gtk_main ();

  return 0;
}
