#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>

typedef struct _UserData {
	/* main window data */
	GObject* 		window;					// main app window
	GObject*		treeview;				// directory navigation in treeview on the left side of window
	GObject*		notebook;				// notebook with file contents and tabs in the center of window
	GObject*		buffer;					// buffer for file contents
	gchar*			font;					// font name as char array
	gchar*			homedir;				// directory that app is opened in
	GObject*		treestore;

	t_node			*head;

} UserData;


GtkWindow* get_window(UserData* userdata) {
	return GTK_WINDOW(userdata->window);
}
GtkTreeView* get_treeview(UserData* userdata) {
	return GTK_TREE_VIEW(userdata->treeview);
}
GtkNotebook* get_notebook(UserData* userdata) {
	return GTK_NOTEBOOK(userdata->notebook);
}
GtkSourceBuffer* get_buffer(UserData* userdata) {
	return GTK_SOURCE_BUFFER(userdata->buffer);
}

t_node** get_list(UserData* userdata) {
	return &userdata->head;
}

static gchar*
get_text_from_eventbox(GtkWidget* widget) {
    GList* list = gtk_container_get_children(GTK_CONTAINER(widget));
    GtkLabel* label = ((GtkLabel*) list->data);
    gchar* title = (gchar *)gtk_label_get_text(GTK_LABEL(label));
    return title;
}
