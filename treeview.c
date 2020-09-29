#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtk/gtk.h>

enum {
  COLUMN = 0,
  NUM_COLS
};

void 
popup_menu_on_DoSomething(GtkWidget *menuitem, gpointer userdata)
{
  /*we passed the view as userdata when we connected the signal */
  GtkTreeView *treeview = GTK_TREE_VIEW(userdata);

  g_print("Do something!\n");
}

void
popup_menu(GtkWidget *treeview, GdkEventButton *event, gpointer userdata)
{
  GtkWidget *menu, *menuitem;

  menu = gtk_menu_new();
  menuitem = gtk_menu_item_new_with_label("Do something");

  g_signal_connect(menuitem, "activate", (GCallback) popup_menu_on_DoSomething, treeview);

  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
  gtk_widget_show_all(menu);

  gtk_menu_popup_at_pointer (GTK_MENU(menu), (GdkEvent*) event);
}
/*
gboolean 
on_button_pressed(GtkWidget *treeview, GdkEventButton *event, gpointer userdata)
{
  GtkTreeSelection *selection;
  GtkTreeModel     *model;
  GtkTreeIter       child;
  GtkTreeIter       parent;
  gboolean          hasParent;
  gchar            *path, *name, *parent_name;

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
        g_print ("on_button_pressed: %s\n", path);

        g_free(name);
        g_free(path);

      }
    return TRUE;
  }

  // single click with the right mouse button?
  else if (event->type == GDK_BUTTON_PRESS && event->button == 3) {
    g_print("Single right click on the tree view.\n");

    // optional: select row if no row is selected or only
    //           one other row is selected (will only do something
    //           if you set a tree selection mode as described later
    //           in the tutorial)
    if (1) {
      GtkTreeSelection * selection;

      selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

      // Note: gtk_tree_selection_count_selected_rows() does not
      //       exist in gtk+-2.0, only in gtk+ >= v2.2 !
      if (gtk_tree_selection_count_selected_rows(selection) <= 1) {
        GtkTreePath * path;

        // Get tree path for row that was clicked
        if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(treeview),
            event->x, event->y, &path, NULL, NULL, NULL))
        {
          gtk_tree_selection_unselect_all(selection);
          gtk_tree_selection_select_path(selection, path);
          gtk_tree_path_free(path);
        }
      }
    } // end of optional bit

    popup_menu(treeview, event, userdata);

    return TRUE; //we handled this 
  }

  return FALSE; //we did not handle this 
}
*/
void 
on_changed(GtkWidget *widget, gpointer statusbar) {
    
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *value;

  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(widget), &model, &iter)) {

    gtk_tree_model_get(model, &iter, COLUMN, &value,  -1);
    gtk_statusbar_push(GTK_STATUSBAR(statusbar),
        gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), 
            value), value);
    g_free(value);
  }
}

void 
fill_treestore(const char *pathname, GtkTreeStore *treestore, GtkTreeIter toplevel) {
    DIR *dir;
    struct dirent *entry;
    GtkTreeIter child;

    if (!(dir = opendir(pathname))) {
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", pathname, entry->d_name); // create name of subdirectory

            gtk_tree_store_append(treestore, &child, &toplevel);
            gtk_tree_store_set(treestore, &child,
                     COLUMN, entry->d_name,
                     -1);

            fill_treestore(path, treestore, child);
        } else {
            gtk_tree_store_append(treestore, &child, &toplevel);
            gtk_tree_store_set(treestore, &child,
                     COLUMN, entry->d_name,
                     -1);
        }
    }
    closedir(dir);
}

GtkTreeModel*
create_and_fill_model(const char *pathname) {
  GtkTreeIter toplevel;
  GtkTreeStore *treestore;

  treestore = gtk_tree_store_new(NUM_COLS,
                  G_TYPE_STRING);

  gtk_tree_store_append(treestore, &toplevel, NULL);
  gtk_tree_store_set(treestore, &toplevel, COLUMN, pathname, -1);
  
  fill_treestore (pathname, treestore, toplevel);

  return GTK_TREE_MODEL(treestore);
}

void
create_view_and_model(gchar* filepath, GtkWidget *treeview) {
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *col;
  GtkTreePath* treepath;
  GtkTreeModel *model;

  //view = gtk_tree_view_new();

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Programming languages");
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer, 
      "text", COLUMN);

  model = create_and_fill_model(filepath);
  gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), model);
  g_object_unref(model); 

  /* Expand top tree node */
  treepath = gtk_tree_path_new_from_string("0");
  gtk_tree_view_expand_row(GTK_TREE_VIEW(treeview), treepath, FALSE);

  //g_signal_connect(view, "button-press-event", (GCallback) on_button_pressed, NULL);

}

#if !TREEVIEW

int main(int argc, char *argv[]) {
    
  GtkWidget *window;
  GtkWidget *swindow;
  GtkWidget *view;
  GtkTreeSelection *selection; 
  GtkWidget *vbox;
  GtkWidget *statusbar;
  gchar* filepath = "/home/rafal/IdeaProjects/gtksourceview-my-ide/application";

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "Tree view");
  gtk_widget_set_size_request(window, 400, 900);

  vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  view = gtk_tree_view_new();
  create_view_and_model(filepath, view);
  swindow = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (swindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_add (GTK_CONTAINER (swindow), view);

  gtk_box_pack_start(GTK_BOX(vbox), swindow, TRUE, TRUE, 1);

  statusbar = gtk_statusbar_new();
  gtk_box_pack_start(GTK_BOX(vbox), statusbar, FALSE, TRUE, 1);

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
  g_signal_connect(selection, "changed", G_CALLBACK(on_changed), statusbar);

  g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}

#endif