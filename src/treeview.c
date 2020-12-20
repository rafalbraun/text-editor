#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include "list.h"
#include "config.h"
#include "treeview.h"



/*
gchar* 
get_selection(GtkWidget * treeview) {
    GtkTreeIter iter;
    GtkTreeModel *model;
    GtkTreeSelection *selection;
    gchar *value;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));

    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)) {
        gtk_tree_model_get(model, &iter, COLUMN, &value, -1);
        return value;
    }
    return NULL;
}
*/
void
popup_menu(GtkWidget *treeview, GdkEventButton *event, gpointer userdata) {
    GtkTreeModel     *tree_model;
    GtkTreeSelection *selection;
    GtkTreePath      *treepath;
    GtkTreeIter       parent;
    GList            *rows;
    GtkMenu          *menu;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

    if (gtk_tree_selection_count_selected_rows (selection) == 1) {

        tree_model = gtk_tree_view_get_model (GTK_TREE_VIEW(treeview));
        gtk_tree_selection_get_selected (selection, &tree_model, &parent);

        if (gtk_tree_model_iter_has_child (tree_model, &parent)) 
        {
            gtk_tree_selection_select_path (selection, treepath);
            rows = gtk_tree_selection_get_selected_rows (selection, &tree_model);
            treepath = (GtkTreePath*) g_list_first (rows)->data;

            if (gtk_tree_view_row_expanded (GTK_TREE_VIEW(treeview), treepath)) {
                //menu = get_treeview_menu_collapse(userdata); OD ODKOMENTOWANIA
            } else {
                //menu = get_treeview_menu_expand(userdata); OD ODKOMENTOWANIA
            }
        } else {
            //menu = get_treeview_menu(userdata); OD ODKOMENTOWANIA
        }
    }

    gtk_widget_show_all(GTK_WIDGET(menu));
    gtk_menu_popup_at_pointer(GTK_MENU(menu), (GdkEvent * ) event);
}

/*
void
on_changed(GtkWidget * widget, gpointer statusbar) {

    GtkTreeIter iter;
    GtkTreeModel * model;
    gchar * value;

    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), & model, & iter)) {

        gtk_tree_model_get(model, & iter, COLUMN, & value, -1);
        gtk_statusbar_push(GTK_STATUSBAR(statusbar),
            gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), value), value);
        g_free(value);
    }
}
*/

//int cmpfunc (const void * a, const void * b) {
    //return ( *(int*)a - *(int*)b );
//}

int cmpfunc (const void *a, const void *b) 
{
    return g_strcmp0(((struct dirent *)a)->d_name, ((struct dirent *)b)->d_name);
}

void
fill_treestore(const gchar * filepath, GtkTreeStore * treestore, GtkTreeIter toplevel) 
{
    DIR             *dir;
    GtkTreeIter      child;
    struct dirent   *entry;
    struct dirent   *entries;
    int              count = 0, i = 0;
    gchar            path[SIZE];

    if (!(dir = opendir(filepath))) {
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        //if (entry->d_type == DT_REG) { /* If the entry is a regular file */
            count++;
        //}
    }
    closedir(dir);

    dir = opendir(filepath); /* There should be error handling after this */
    //count = count_files_dirent (filepath);
    entries = g_new0 (struct dirent, count);

    while ((entry = readdir(dir)) != NULL) {
        // HERE ADD FILTER FUNCTION
        if ( (strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0) && (entry->d_name[0] != '.') ) {
            entries[i++] = *entry;
        }
    }
    qsort (entries, count, sizeof(struct dirent), cmpfunc);

    for (int i=0; i<count; i++) {
        entry = &(entries[i]);
        if (entry->d_type == DT_DIR) {
            snprintf(path, sizeof(path), "%s/%s", filepath, entry->d_name);
            gtk_tree_store_append(treestore, &child, &toplevel);
            gtk_tree_store_set(treestore, &child, COLUMN, entry->d_name, -1);
            fill_treestore(path, treestore, child);
        }
        if (entry->d_type == DT_REG) {
            gtk_tree_store_append(treestore, &child, &toplevel);
            gtk_tree_store_set(treestore, &child, COLUMN, entry->d_name, -1);
        }
    }

    g_free (entries);
    closedir(dir);
}

void
fill_treeview(GtkTreeView * treeview, const char * pathname) 
{
    GtkTreeStore    *treestore = NULL;
    GtkTreePath     *treepath;
    GtkTreeIter      toplevel;

    treestore = GTK_TREE_STORE(gtk_tree_view_get_model(treeview));

    gtk_tree_store_append(treestore, &toplevel, NULL);
    gtk_tree_store_set(treestore, &toplevel, COLUMN, pathname, -1);

    fill_treestore(pathname, treestore, toplevel);

    // Expand top tree node
    treepath = gtk_tree_path_new_from_string("0");
    gtk_tree_view_expand_row(GTK_TREE_VIEW(treeview), treepath, FALSE);

    g_signal_connect (G_OBJECT (treeview), "key-press-event", G_CALLBACK (key_pressed_treeview), NULL);
    g_signal_connect (G_OBJECT (treeview), "button-press-event", G_CALLBACK (on_button_pressed), NULL);

}


gboolean
on_button_pressed(GtkWidget *treeview, GdkEventButton *event, gpointer userdata) 
{
  GtkTreeSelection *selection;
  GtkTreeModel     *model;
  GtkTreeIter       child;
  gchar            *path;
  
  path = "";

  if (event->type == GDK_2BUTTON_PRESS) 
  {
      selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

      if (gtk_tree_selection_count_selected_rows(selection) == 1) {

        model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));

        validate_file (path, model, selection);

        //g_free(path);
      }

      return TRUE;
    } else if (event->type == GDK_BUTTON_PRESS && event->button == 3) 
    {
        selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
        if (gtk_tree_selection_count_selected_rows(selection) <= 1) 
        {
            GtkTreePath * path;

            if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(treeview),
                event->x, event->y, &path, NULL, NULL, NULL)) {
                gtk_tree_selection_unselect_all(selection);
                gtk_tree_selection_select_path(selection, path);
                gtk_tree_path_free(path);
            }
    }
    //popup_menu(treeview, event, userdata);
    return TRUE;
  }
  return FALSE;
}


void validate_file(gchar* path, GtkTreeModel *model, GtkTreeSelection *selection) {
        GtkTreeIter       child, parent;
        gboolean          hasParent;
        gchar            *name, *parent_name;
        
        parent_name = "";

        gtk_tree_selection_get_selected(selection, &model, &child);
        gtk_tree_model_get (model, &child, COLUMN, &name, -1);
        while ( (hasParent = gtk_tree_model_iter_parent(model, &parent, &child)) == TRUE ) {
          if ( hasParent == TRUE ) {
            gtk_tree_model_get (model, &parent, COLUMN, &parent_name, -1);
            path = g_strconcat(parent_name, "/", path, NULL);
            g_free(parent_name);
            child = parent;
          }
        }

        path = g_strconcat(path, name, NULL);
        g_free(name);


        if ( g_file_test(path, G_FILE_TEST_IS_DIR) == FALSE ) {
              if ( g_file_test(path, G_FILE_TEST_EXISTS) == TRUE ) {
                    //open_file (userdata, path);
                    g_print("open_file");
              } else {
                //show_error(get_window(userdata), "no file under filepath");
                   g_print("show_error");
              }
        } else {
            //show_error(get_window(userdata), "filepath is directory");
        }

        g_free(path);

}


gboolean
key_pressed_treeview(GtkWidget *treeview, GdkEventKey *event, gpointer userdata) {
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

////////////////////////////////////////////
/*
GtkTreeModel *
create_and_fill_model(const char * pathname) {
    GtkTreeIter toplevel;
    GtkTreeStore * treestore;

    treestore = gtk_tree_store_new(NUM_COLS,
        G_TYPE_STRING);

    gtk_tree_store_append(treestore, & toplevel, NULL);
    gtk_tree_store_set(treestore, & toplevel, COLUMN, pathname, -1);

    fill_treestore(pathname, treestore, toplevel);

    return GTK_TREE_MODEL(treestore);
}



void
create_view_and_model(gchar * filepath, GtkWidget * treeview) {
    GtkCellRenderer * renderer;
    GtkTreeViewColumn * col;
    GtkTreePath * treepath;
    GtkTreeModel * model;

    gtk_tree_view_set_enable_search(GTK_TREE_VIEW(treeview), FALSE);

    col = gtk_tree_view_column_new();
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);

    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_add_attribute(col, renderer, "text", COLUMN);

    model = create_and_fill_model(filepath);
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), model);
    g_object_unref(model);

    // Expand top tree node
    treepath = gtk_tree_path_new_from_string("0");
    gtk_tree_view_expand_row(GTK_TREE_VIEW(treeview), treepath, FALSE);
}
*/