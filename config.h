#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>

typedef struct _PageInfo {
	gchar* 		filename;
	gchar* 		filepath;
	guint 		pagenum;
	
} PageInfo;

typedef struct _UserData {
	/* main window data */
	GObject* 		window;					// main app window
	GObject*		treeview;				// directory navigation in treeview on the left side of window
	GObject*		notebook;				// notebook with file contents and tabs in the center of window
	GObject*		buffer;					// buffer for file contents
	GList* 			filenames;				// list of PageInfo structures
	guint 			tabnum;					// number of tabs
	gchar*			font;					// font name as char array
	gchar*			homedir;				// directory that app is opened in
	GObject*		treestore;

	/* full search window settings */
	int 			stdout_fd;				// file descriptor opened by glib_regex process that works as queue for search results
	gchar*			filetypes;				// filetypes (extension) that should be filtered in full search window
	gchar* 			full_search_buffer;		// buffer that contains search results, the contents come from ..........
	guint 			limit;					// number of chars that need to be in entry field before searching starts


} UserData;

void init_user_data(UserData* userdata) {
	userdata->limit = 3;
}

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
guint get_tabnum(UserData* userdata) {
	return userdata->tabnum;
}
void tabnum_incr(UserData* userdata) {
	userdata->tabnum++;
}
void tabnum_decr(UserData* userdata) {
	userdata->tabnum--;
}



// function checks if all is in place:
//	o glib_regex program is in place
//	o all *.ui files are in place and ready to read
//	o 
/*
static void test_app () {

}




// notebook
static void open_file (GtkNotebook* notebook, gchar* filepath, GtkWidget* content, GObject* buffer)
static void close_file(GtkNotebook* notebook, gchar* filepath);
static void open_file_dialog (GtkWidget *widget, GtkWidget* notebook);
static gboolean notebook_tab_clicked(GtkWidget *widget, GdkEventButton *event, gpointer notebook);
static gboolean key_pressed(GtkWidget *notebook, GdkEventKey *event, gpointer userdata);

//treeview
void create_view_and_model(gchar* filepath, GtkWidget *treeview);
void fill_treestore(const char *pathname, GtkTreeStore *treestore, GtkTreeIter toplevel);
void on_changed(GtkWidget *widget, gpointer statusbar);
void popup_menu(GtkWidget *treeview, GdkEventButton *event, gpointer userdata);
void popup_menu_on_DoSomething(GtkWidget *menuitem, gpointer userdata);
GtkTreeModel* create_and_fill_model(const char *pathname);
*/