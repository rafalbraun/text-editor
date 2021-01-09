#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include "list.h"
#include "config.h"
#include "treeview.h"
#include "notebook.h"
#include "sourceview.h"

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
void open_file_cb() {

}
*/

static GtkActionEntry buffer_action_entries[] = {
    //{ "New", "document-open", "_Open", "<control>O", "Open a file", G_CALLBACK (create_empty_tab) },
    
    
    //{ "Collapse", "document-open", "_Open", "<control>O", "Open a file", G_CALLBACK (open_file_cb) },
    //{ "Varnames", "document-open", "_Open", "<control>O", "Open a file", G_CALLBACK (underline_varnames) },
    /*
    { "Copy", "document-open", "_Open", "<control>O", "Open a file", NULL },
    { "Paste", "document-open", "_Open", "<control>O", "Open a file", NULL },
    { "Filter", "document-open", "_Open", "<control>O", "Open a file", NULL },
    { "Mark", "document-open", "_Open", "<control>O", "Open a file", NULL },
    { "Rename", "document-open", "_Open", "<control>O", "Open a file", NULL },
    { "Delete", "document-open", "_Open", "<control>O", "Open a file", NULL },
    { "Show", "document-open", "_Open", "<control>O", "Open a file", NULL },
    { "Open", "document-open", "_Open", "<control>O", "Open a file", NULL },
    */
    { "Quit", "document-open", "_Quit", "<control>Q", "Exit the application", G_CALLBACK (gtk_main_quit) }
};

GtkWidget*
build_menu (PopupMenu menuType, gpointer user_data) {
    GtkWidget *menu;
    GtkWidget *menuItem;
    GtkActionEntry *actionEntry;

    menu = gtk_menu_new();

    // name, callback, accelerator, stock_id, callback, label, tooltip
    for (int i=0; i<G_N_ELEMENTS (buffer_action_entries); i++) {
        actionEntry = &(buffer_action_entries[i]);
        menuItem = gtk_menu_item_new_with_label(actionEntry->name);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);
        g_signal_connect (G_OBJECT(menuItem), "activate", G_CALLBACK (actionEntry->callback), user_data);
    }
    
    gtk_widget_show_all(menu);

    return menu;
}

void
popup_menu(GtkWidget *treeview, GdkEventButton *event, gpointer user_data) {
    GtkTreeModel     *tree_model;
    GtkTreeSelection *selection;
    GtkTreePath      *treepath;
    GtkTreeIter       parent;
    GList            *rows;
    GtkWidget        *menu;

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
                menu = build_menu(COLLAPSE, user_data);
            } else {
                //menu = get_treeview_menu_expand(userdata); OD ODKOMENTOWANIA
                menu = build_menu(COLLAPSE, user_data);
            }
        } else {
            //menu = get_treeview_menu(userdata); OD ODKOMENTOWANIA
            menu = build_menu(COLLAPSE, user_data);
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
fill_treeview(gpointer user_data) 
{
    GtkTreeStore    *treestore = NULL;
    GtkTreePath     *treepath;
    GtkTreeIter      toplevel;
    GtkTreeView     *treeview;
    gchar           *pathname;

    treeview = GET_TREE_VIEW (user_data);
    pathname = GET_FILEPATH (user_data);
    treestore = GTK_TREE_STORE(gtk_tree_view_get_model(treeview));

    gtk_tree_store_append(treestore, &toplevel, NULL);
    gtk_tree_store_set(treestore, &toplevel, COLUMN, pathname, -1);

    fill_treestore(pathname, treestore, toplevel);

    // Expand top tree node
    treepath = gtk_tree_path_new_from_string("0");
    gtk_tree_view_expand_row(GTK_TREE_VIEW(treeview), treepath, FALSE);

    g_signal_connect (G_OBJECT (treeview), "key-press-event", G_CALLBACK (key_pressed_treeview), user_data);
    g_signal_connect (G_OBJECT (treeview), "button-press-event", G_CALLBACK (on_button_pressed), user_data);

}

static gboolean
for_each_func_walk (GtkTreeModel *tree_model,
                            GtkTreePath *path,
                            GtkTreeIter *iter,
                            gpointer user_data) {

    GtkTreeView* tree_view = GET_TREE_VIEW(user_data);
    //GList expanded_rows_list = GET_EXPANDED_ROWS_LIST(user_data);
    /*
    if (gtk_tree_view_row_expanded (tree_view, path)) {
        data.push_back();
    }*/
    g_print("path:%s\n", gtk_tree_path_to_string(path));
    /*
    if (gtk_tree_model_iter_has_child(tree_model, iter)) {
        return FALSE;
    }

    return TRUE;
    */
    return FALSE;
}

void 
walk_tree_model (gpointer user_data) {
    GtkTreeModel* tree_model;
    GtkTreeView* tree_view;
    GtkTreeIter iter;

    tree_view = GET_TREE_VIEW (user_data);
    tree_model = gtk_tree_view_get_model(tree_view);

    if( gtk_tree_model_get_iter_first(tree_model, &iter) ) {
        gtk_tree_model_foreach (tree_model, (GtkTreeModelForeachFunc) for_each_func_walk, user_data);
    }

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

        validate_file (model, selection, userdata);

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
    popup_menu(treeview, event, userdata);
    return TRUE;
  }
  return FALSE;
}

static void
result_func (
    GtkSourceFileLoader * loader,
    GAsyncResult *        res,
    gpointer *            user_data)
{
    gboolean success = FALSE;

    success = gtk_source_file_loader_load_finish (loader, res, NULL);

    // do usuniecia !!!!!!!
    //underline_varnames (GTK_TEXT_BUFFER(user_data));

    g_return_if_fail (success);
}

void load_file (GtkSourceBuffer* buffer, gchar* path, gpointer user_data) 
{
    GtkSourceFile * src_file = gtk_source_file_new ();
    GFile * file = g_file_new_for_path (path);
    gtk_source_file_set_location (src_file, file);
    g_object_unref (file);

    GtkSourceFileLoader * file_loader = gtk_source_file_loader_new (buffer, src_file);
    gtk_source_file_loader_load_async (file_loader, G_PRIORITY_DEFAULT, NULL, NULL, NULL, NULL, (GAsyncReadyCallback) result_func, buffer);

}

gchar* translate_gtk_path_to_string (GtkTreeModel *model, GtkTreeSelection *selection) {
    GtkTreeIter       child, parent;
    gboolean          hasParent;
    gchar            *name, *parent_name, *path;

    parent_name = "";
    path = "";

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

    return path;    
}

// https://developer.gnome.org/gtksourceview/stable/GtkSourceFileLoader.html
void validate_file(GtkTreeModel *model, GtkTreeSelection *selection, gpointer user_data) {
        GtkSourceBuffer  *buffer;

        gchar* path = translate_gtk_path_to_string(model, selection);

        if ( g_file_test(path, G_FILE_TEST_IS_DIR) == FALSE ) {
              if ( g_file_test(path, G_FILE_TEST_EXISTS) == TRUE ) {
                    g_print("[TEST] create_tab: %s \n", path);

                    walk_tree_model (user_data);

                    buffer = create_tab (user_data, path);
                    load_file(buffer, path, user_data);

              } else {
                    //show_error(get_window(userdata), "no file under filepath");
                    g_print("show_error: %s \n", path);
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
