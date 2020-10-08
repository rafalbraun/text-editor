#include <gtksourceview/gtksource.h>
#include <gtk/gtk.h>

gboolean
key_pressed_treeview(GtkWidget *notebook, GdkEventKey *event, gpointer userdata) {
  return FALSE;
}

gboolean
key_pressed_window(GtkWidget *notebook, GdkEventKey *event, gpointer userdata) {
  if (event->state & GDK_CONTROL_MASK && event->keyval == 'F') {
      g_print("ctrl + shift + f \n");
  }

  return FALSE;
}

gboolean 
on_button_pressed(GtkWidget *treeview, GdkEventButton *event, gpointer userdata) {
  GtkTreeSelection *selection;
  GtkTreeIter       child, parent;
  GtkTreeModel     *model;
  GtkWidget        *content;
  gboolean          hasParent;
  gchar            *path, *name, *parent_name;

  if (event->type == GDK_2BUTTON_PRESS) {
      GtkTreeSelection *selection;
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

        content = sourceview_new(GTK_SOURCE_BUFFER(get_buffer(userdata)));
        guess_language(GTK_SOURCE_BUFFER(get_buffer(userdata)), path);

      	if ( g_file_test(path, G_FILE_TEST_IS_DIR) == FALSE ) {
      		  if ( g_file_test(path, G_FILE_TEST_EXISTS) == TRUE ) {
              		open_file (userdata, path, content);
      		  } else {
                show_error(get_window(userdata), "no file under filepath");
      		  }
      	} else {
            show_error(get_window(userdata), "filepath is directory");
      	}

        g_free(name);
        //g_free(path);

      }

      return TRUE;
    } else if (event->type == GDK_BUTTON_PRESS && event->button == 3) {
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

    popup_menu(treeview, event, userdata);

    return TRUE;
  }

  return FALSE;
}
