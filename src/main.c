#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include "list.h"
#include "config.h"
#include "sourceview.h"
#include "treeview.h"
#include "notebook.h"
#include "callback.h"
#include "text_editor.h"

#define UI_DIR "/home/rafal/IdeaProjects/gtksourceview-my-ide/application/ui/text_editor.ui"

void connect_signals (UserData* userdata) {
    g_signal_connect (GET_WINDOW(userdata), "destroy", G_CALLBACK (on_main_quit), (gpointer) userdata);
    g_signal_connect (GET_WINDOW(userdata), "key-press-event", G_CALLBACK (key_pressed_window), userdata);
    g_signal_connect (GET_NOTEBOOK(userdata), "switch-page", G_CALLBACK (switch_page), (gpointer) userdata);

}

// https://en.wikibooks.org/wiki/GTK%2B_By_Example/Tree_View/Tree_Models
int
main (int argc, char *argv[])
{
    GtkBuilder *builder;
    UserData* user_data;
    GError *error = NULL;

    // Init GTK
    gtk_init (&argc, &argv);

    // Construct a GtkBuilder instance and load our UI description
    builder = gtk_builder_new ();
    if (gtk_builder_add_from_file (builder, UI_DIR, &error) == 0) {
        g_printerr ("Error loading file: %s\n", error->message);
        g_clear_error (&error);
        return 1;
    }

    ud_init (&user_data, builder);

    // Connect signal handlers to the constructed widgets

    //set_language (buffer);
    //set_buffer_scheme (buffer);


    /*
    // THE + BUTTON
    GtkWidget *action_widget = gtk_button_new_from_icon_name ("list-add-symbolic", GTK_ICON_SIZE_BUTTON);
    //g_signal_connect (action_widget, "clicked", G_CALLBACK (print_hello), notebook);
    gtk_notebook_set_action_widget (GTK_NOTEBOOK (notebook), action_widget, GTK_PACK_END);
    gtk_widget_show (action_widget);
    */

    fill_treeview (user_data);

    //expand_top_node (treeview);
    //gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(treeview), GTK_TREE_VIEW_GRID_LINES_BOTH);

    //g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), (gpointer)userdata);

    connect_signals (user_data);

    //create_tab (user_data, "aaaaaaaa", "", 0);


    //open_file (user_data, "/home/rafal/IdeaProjects/vault13/README.MD");


    //g_signal_connect (G_OBJECT (window), "key-press-event", G_CALLBACK (key_pressed_notebook), NULL);
/*
    DO ODKOMENTOWANIA

    button = gtk_builder_get_object (builder, "file_new");
    g_signal_connect (button, "activate", G_CALLBACK (new_file_cb), userdata);

    button = gtk_builder_get_object (builder, "edit_cut");
    g_signal_connect (button, "activate", G_CALLBACK (show_langs), NULL);

    button = gtk_builder_get_object (builder, "edit_fullsearch");
    g_signal_connect (button, "activate", G_CALLBACK (full_search_cb), NULL);

    button = gtk_builder_get_object (builder, "edit_findfiles");
    g_signal_connect (button, "activate", G_CALLBACK (find_files_cb), NULL);

*/
    //GObject* syntax_menuitem = gtk_builder_get_object (builder, "syntax");
    //set_syntax_submenu(GTK_MENU_ITEM(syntax_menuitem));
    list_schemes();
    list_langs();

    load_opened_files_from_file (user_data);


    //timer = g_timer_new ();
    //g_timer_start (timer);

    // DO ODKOMENTOWANIA
    //g_timeout_add (80, (GSourceFunc) _check_timeout_since_last_keypress, userdata);

    //open_files_from_last_session (user_data);

    gtk_main ();

    return 0;
}

