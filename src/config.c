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

void ud_init (UserData** userdata_ptr, GtkBuilder* builder) {
	*userdata_ptr = g_new0 (UserData, 1);

	UserData* userdata = *userdata_ptr;

	//cast_to_ud(*userdata)->head = NULL;
	userdata->head = NULL;
	userdata->session_info = "~session-info";

	//cast_to_ud(*userdata)->filepath = "/home/rafal/Desktop/gtksourceview-4.0.3";
	//cast_to_ud(*userdata)->filepath = "/home/rafal/IdeaProjects/gtksourceview-my-ide/application";
	userdata->filepath = "/home/rafal/IdeaProjects/vault13";

    userdata->window 	= gtk_builder_get_object (builder, "window");
    //userdata->buffer 	= gtk_builder_get_object (builder, "sourcebuffer");
    userdata->treeview 	= gtk_builder_get_object (builder, "treeview");
    userdata->treestore = gtk_builder_get_object (builder, "treestore");
    userdata->notebook 	= gtk_builder_get_object (builder, "notebook");

	separator = "\n";

    userdata->untitled_files = 0;
/*
    gtk_text_buffer_create_tag (GET_TEXT_BUFFER (userdata), "blue", "background", "white", NULL);
    gtk_text_buffer_create_tag (GET_TEXT_BUFFER (userdata), "black", "foreground", "black", NULL);
    gtk_text_buffer_create_tag (GET_TEXT_BUFFER (userdata), "italic", "style", PANGO_STYLE_ITALIC, NULL);
    gtk_text_buffer_create_tag (GET_TEXT_BUFFER (userdata), "underline", "underline", PANGO_UNDERLINE_SINGLE, NULL);
*/
	g_print("%s \n", userdata->filepath);
}

void incr_untitled_files (gpointer user_data) {
    ((UserData*)user_data)->untitled_files++;
}

gint get_untitled_files (gpointer user_data) {
  return ((UserData*)user_data)->untitled_files;
}

GtkWindow* get_window (UserData* userdata) {
	return GTK_WINDOW(userdata->window);
}

GtkTreeView* get_treeview (UserData* userdata) {
	return GTK_TREE_VIEW(userdata->treeview);
}

GtkNotebook* get_notebook (UserData* userdata)
{
	return GTK_NOTEBOOK(userdata->notebook);
}
/*
GtkSourceBuffer* get_buffer (UserData* userdata)
{
	return GTK_SOURCE_BUFFER(userdata->buffer);
}*/

///////////////////
int check_not_null(GObject* ptr, gchar* fun_name) {
	if (ptr == NULL) {
		g_print("[ERROR] %s \n", fun_name);
		return 0;
	}
	return 1;
}
/*
GtkSourceBuffer* GET_SOURCE_BUFFER (UserData* userdata)
{
	return check_not_null(userdata->buffer, "GET_SOURCE_BUFFER") ? GTK_SOURCE_BUFFER(userdata->buffer) : NULL;
}*/
/*
GtkTextBuffer* GET_TEXT_BUFFER (UserData* userdata)
{
	return check_not_null(userdata->buffer, "GTK_TEXT_BUFFER") ? GTK_TEXT_BUFFER(userdata->buffer) : NULL;
}
*/
GtkTreeView* GET_TREE_VIEW(UserData* userdata) 
{
	return check_not_null(userdata->treeview, "GTK_TREE_VIEW") ? GTK_TREE_VIEW(userdata->treeview) : NULL;
}

gchar* GET_FILEPATH(UserData* userdata) 
{
	return userdata->filepath;
}

GtkWindow* GET_WINDOW(UserData* userdata) 
{
	return check_not_null(userdata->window, "GTK_WINDOW") ? GTK_WINDOW(userdata->window) : NULL;
}

GtkNotebook* GET_NOTEBOOK(UserData* userdata) 
{
	return check_not_null(userdata->notebook, "GTK_NOTEBOOK") ? GTK_NOTEBOOK(userdata->notebook) : NULL;
}

/////////////////////
/*

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
*/


int is_text_file(gchar* filepath) {
    gchar *text;
    gsize len;
    GError *err = NULL;

    if (g_file_get_contents(filepath, &text, &len, &err) == FALSE) {
        return 0;
    }
    if (!g_utf8_validate (text, len, NULL)) {
        return 0;
    }
    return 1;
}

////////////////////////////////////////////////////////////////
// error
////////////////////////////////////////////////////////////////

void
show_error (GtkWindow* window, gchar* message)
{
    GtkWidget *dialog;
    
    dialog = gtk_message_dialog_new (GTK_WINDOW (window),
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, message);
    
    gtk_window_set_title (GTK_WINDOW (dialog), "Error");
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

////////////////////////////////////////////////////////////////
// model
////////////////////////////////////////////////////////////////

// nothing special, just a new tab and new buffer
void
new_file_cb (gpointer userdata)
{
	/*
    gchar buffer[64];
    gchar *fname;
    g_print ("%d \n", cast_to_ud (userdata)->untitled_files_in_buffer_max);
    sprintf (buffer, "/tmp/Untitled %d",
       cast_to_ud (userdata)->untitled_files_in_buffer_max++);
    fname = g_strdup (buffer);
    g_creat (fname, S_IREAD | S_IWRITE);
    open_file (userdata, fname);
    */
}

// check if file exists and is text file
// should I return boolean???
void
open_file_cb (const gchar* filepath, gchar** text, gsize length, gpointer user_data) {
    GError *err = NULL;
    gchar* tmp = NULL;
    
    if (g_file_get_contents (filepath, &tmp, &length, &err) == FALSE) {
        show_error (GET_WINDOW (user_data), "error reading file");
        g_error ("[open_file_cb:g_file_get_contents] error reading %s: %s", filepath, err->message);
        return;
    }
    if (!g_utf8_validate (tmp, length, NULL)) {
        show_error (GET_WINDOW (user_data), "the file is binary");
        g_error ("[open_file_cb:g_utf8_validate] error reading %s: %s", filepath, err->message);
        return;
    }
    *text = g_strdup (tmp);
}

// ask if we really want to save file, then check if file exists and if we would overwrite
// should I return boolean???
void 
save_file_cb (const gchar* filepath, const gchar* text, gpointer userdata) {
    GError *err = NULL;
    GFileSetContentsFlags flags = G_FILE_SET_CONTENTS_CONSISTENT;

    int status, mode = 0666;
    gssize length = strlen(text);

    status = g_file_set_contents_full (filepath,
                                       text,
                                       length,
                                       flags,
                                       mode,
                                       &err);

    if (status == FALSE) {
        g_error("[save_file_cb:g_file_set_contents_full] Error saving %s: %s", filepath, err->message);
        return;
    }
}

// ask if file saved when we want to close tab
void 
close_file_cb (gpointer userdata) {
	
}

// after we go back to the file tab we need to make sure the file has not been changed and then load out app buffer
void 
load_file_cb (gpointer userdata) {
	
}




