#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include "list.h"
#include "config.h"
#include "notebook.h"
#include "treeview.h"
#include "callback.h"
#include "text_editor.h"

#define COLUMN 0

//g_printerr("%s\n",
//       gdk_keyval_name (event->keyval));

gboolean
key_pressed_treeview(GtkWidget *treeview, GdkEventKey *event, gpointer userdata) 
{
    GtkTreeModel     *tree_model;
    GtkTreeSelection *selection;
    GtkTreePath      *treepath;
    GtkTreeIter       parent;
    GList            *rows;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

    if (gtk_tree_selection_count_selected_rows (selection) == 1) 
    {
        if (event->keyval == GDK_KEY_Return) 
        {
            tree_model = gtk_tree_view_get_model (GTK_TREE_VIEW(treeview));
            gtk_tree_selection_get_selected (selection, &tree_model, &parent);

            if (gtk_tree_model_iter_has_child (tree_model, &parent)) 
            {
                gtk_tree_selection_select_path (selection, treepath);
                rows = gtk_tree_selection_get_selected_rows (selection, &tree_model);
                treepath = (GtkTreePath*) g_list_first (rows)->data;

                if (gtk_tree_view_row_expanded (GTK_TREE_VIEW(treeview), treepath)) {
                    gtk_tree_view_collapse_row (GTK_TREE_VIEW(treeview), treepath);
                } else {
                    gtk_tree_view_expand_row (GTK_TREE_VIEW(treeview), treepath, FALSE);
                }
            }
        }
    }

    return FALSE;
}

gboolean
key_pressed_window(GtkWidget *widget, GdkEventKey *event, gpointer userdata) {
    if (event->state & GDK_CONTROL_MASK && event->keyval == 'F') {
        g_print ("ctrl + shift + f \n");
        full_search_cb (widget, userdata);
    }
    if (event->state & GDK_CONTROL_MASK && event->keyval == 'N') {
        g_print ("ctrl + shift + n \n");
        find_files_cb (widget, userdata);
    }
    if (event->state & GDK_CONTROL_MASK && event->keyval == 's') {
        g_print ("ctrl + s \n");
        save_file_default (userdata);
    }

    return FALSE;
}

gboolean
on_button_pressed(GtkWidget *treeview, GdkEventButton *event, gpointer userdata) {
  GtkTreeSelection *selection;
  GtkTreeIter       child, parent;
  GtkTreeModel     *model;
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

      	if ( g_file_test(path, G_FILE_TEST_IS_DIR) == FALSE ) {
      		  if ( g_file_test(path, G_FILE_TEST_EXISTS) == TRUE ) {
              		open_file (userdata, path);
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
