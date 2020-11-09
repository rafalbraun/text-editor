#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include "list.h"
#include "config.h"
#include "sourceview.h"
#include "treeview.h"
#include "notebook.h"
#include "callback.h"
#include "text_editor.h"

// https://en.wikibooks.org/wiki/GTK%2B_By_Example/Tree_View/Tree_Models
int
main (int argc, char *argv[])
{
    GtkBuilder *builder;
    GObject *buffer;
    GObject *window;
    GObject *button;
    GObject *treeview;
    GObject *treestore;
    GObject *notebook;
    GError *error = NULL;
    UserData* userdata;

    // Init GTK
    gtk_init (&argc, &argv);
    ud_init (&userdata);

    // Construct a GtkBuilder instance and load our UI description
    builder = gtk_builder_new ();
    if (gtk_builder_add_from_file (builder, "text_editor.ui", &error) == 0) {
        g_printerr ("Error loading file: %s\n", error->message);
        g_clear_error (&error);
        return 1;
    }

/*
    userdata = g_new0 (UserData, 1);
    cast_to_ud(userdata)->head = NULL;
    cast_to_ud(userdata)->session_info = "~session-info";
    cast_to_ud(userdata)->filepath = "/home/rafal/Desktop/gtksourceview-4.0.3";
*/
    // Connect signal handlers to the constructed widgets
    window = gtk_builder_get_object (builder, "window");
    buffer = gtk_builder_get_object (builder, "sourcebuffer");
    treeview = gtk_builder_get_object (builder, "treeview");
    treestore = gtk_builder_get_object (builder, "treestore");
    notebook = gtk_builder_get_object (builder, "notebook");

    cast_to_ud(userdata)->window = window;
    cast_to_ud(userdata)->buffer = buffer;
    cast_to_ud(userdata)->treeview = treeview;
    cast_to_ud(userdata)->treestore = treestore;
    cast_to_ud(userdata)->notebook = notebook

    cast_to_ud(userdata)->treeview_menu = gtk_builder_get_object (builder, "treeview_context_menu");
    cast_to_ud(userdata)->treeview_menu_expand = gtk_builder_get_object (builder, "treeview_context_menu_expand");
    cast_to_ud(userdata)->treeview_menu_collapse = gtk_builder_get_object (builder, "treeview_context_menu_collapse");

    gtk_text_buffer_create_tag (GTK_TEXT_BUFFER (buffer), "blue", "background", "white", NULL);
    gtk_text_buffer_create_tag (GTK_TEXT_BUFFER (buffer), "black", "foreground", "black", NULL);
    gtk_text_buffer_create_tag (GTK_TEXT_BUFFER (buffer), "italic", "style", PANGO_STYLE_ITALIC, NULL);
    gtk_text_buffer_create_tag (GTK_TEXT_BUFFER (buffer), "underline", "underline", PANGO_UNDERLINE_SINGLE, NULL);

    set_language (buffer);
    set_buffer_scheme (buffer);


    /*
    // THE + BUTTON
    GtkWidget *action_widget = gtk_button_new_from_icon_name ("list-add-symbolic", GTK_ICON_SIZE_BUTTON);
    //g_signal_connect (action_widget, "clicked", G_CALLBACK (print_hello), notebook);
    gtk_notebook_set_action_widget (GTK_NOTEBOOK (notebook), action_widget, GTK_PACK_END);
    gtk_widget_show (action_widget);
    */

    fill_treestore_new (GTK_TREE_STORE (treestore), cast_to_ud(userdata)->filepath);
    expand_top_node (treeview);
    //gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(treeview), GTK_TREE_VIEW_GRID_LINES_BOTH);

    //g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), (gpointer)userdata);
    g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (on_main_quit), (gpointer) userdata);
    g_signal_connect (G_OBJECT (window), "key-press-event", G_CALLBACK (key_pressed_window), userdata);
    g_signal_connect (G_OBJECT (treeview), "key-press-event", G_CALLBACK (key_pressed_treeview), userdata);
    g_signal_connect (G_OBJECT (treeview), "button-press-event", G_CALLBACK (on_button_pressed), (gpointer) userdata);
    g_signal_connect (G_OBJECT (notebook), "switch-page", G_CALLBACK (switch_page), (gpointer) userdata);
    //g_signal_connect (G_OBJECT (window), "key-press-event", G_CALLBACK (key_pressed_notebook), NULL);

    button = gtk_builder_get_object (builder, "file_new");
    g_signal_connect (button, "activate", G_CALLBACK (new_file_cb), userdata);

    button = gtk_builder_get_object (builder, "edit_cut");
    g_signal_connect (button, "activate", G_CALLBACK (show_langs), NULL);

    button = gtk_builder_get_object (builder, "edit_fullsearch");
    g_signal_connect (button, "activate", G_CALLBACK (full_search_cb), NULL);

    button = gtk_builder_get_object (builder, "edit_findfiles");
    g_signal_connect (button, "activate", G_CALLBACK (find_files_cb), NULL);

    //GObject* syntax_menuitem = gtk_builder_get_object (builder, "syntax");
    //set_syntax_submenu(GTK_MENU_ITEM(syntax_menuitem));

    //timer = g_timer_new ();
    //g_timer_start (timer);

    // DO ODKOMENTOWANIA
    //g_timeout_add (80, (GSourceFunc) _check_timeout_since_last_keypress, userdata);

    open_files_from_last_session (userdata);

    gtk_main ();

    return 0;
}

