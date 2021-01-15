#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#ifndef CONFIG_H
#define CONFIG_H

#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>

/* for file creation 
#include <stdio.h>
#include <glib/gstdio.h>
*/

//GTimer *timer;
//gboolean is_saved = FALSE;

int              tab_max;
const gchar     *separator;
gchar           *relative_path[128];
gchar           *absolute_path[128];
gchar           *file_contents[128]; // buffer for all files contents
gint            *is_file_saved[128];

GList*          head ;
//t_node          *head;

typedef struct t {
  gchar* title;
  gchar* relative_path;
  gchar* absolute_path;
  gint   is_file_saved;
  //gchar* tab_buffer;
  GtkWidget* buffer;

} t_tab;

t_tab* new_tab(gchar* title);
gint index_tab (GList *head, gchar* title);
t_tab* append_tab (GList **head, gchar* title);
gint delete_tab (GList **head, t_tab* data);
t_tab* get_nth (GList* head, gint index) ;





typedef struct _UserData
{
    /* main window data */

    //t_node		*head;

    GObject* 	window;
    GObject*	treeview;
    GObject*	notebook;
    GObject*	buffer;
    GObject*	treestore;
    GObject* 	treeview_menu;
    GObject* 	treeview_menu_collapse;
    GObject* 	treeview_menu_expand;

    gchar*		font;
    gchar*		homedir;
    //gchar*		session_info;
    gchar* 		filepath;
  
    GList*    expanded_rows_list;


    int             untitled_files;

} UserData;

GList** GET_EXPANDED_ROWS_LIST (UserData* user_data);
void SET_EXPANDED_ROWS_LIST (UserData* user_data, GList* list);

int get_untitled_files (gpointer user_data);
void incr_untitled_files (gpointer user_data);

UserData *
cast_to_ud (gpointer userdata);

void
ud_init (UserData** userdata_ptr, GtkBuilder* builder);

void 
ud_init_empty (UserData** userdata_ptr, gchar* filepath);

GtkWindow* 
get_window (UserData* userdata);

GtkTreeView* 
get_treeview (UserData* userdata);

GtkNotebook* 
get_notebook (UserData* userdata);

// GtkSourceBuffer* 
// get_buffer (UserData* userdata);

GtkMenu* 
get_treeview_menu (UserData* userdata);

GtkMenu* 
get_treeview_menu_expand (UserData* userdata);

GtkMenu* 
get_treeview_menu_collapse (UserData* userdata);


void
show_error (GtkWindow * window, gchar * message);

int
is_text_file(gchar* filepath);




GtkSourceBuffer* GET_SOURCE_BUFFER (UserData* userdata);
GtkTextBuffer* GET_TEXT_BUFFER (UserData* userdata);
void SET_SOURCE_BUFFER(UserData* user_data, GtkSourceBuffer* buffer);
void SET_TEXT_BUFFER(UserData* user_data, GtkTextBuffer* buffer);

GtkTreeView* GET_TREE_VIEW(UserData* userdata);
gchar* GET_FILEPATH(UserData* userdata);
GtkWindow* GET_WINDOW(UserData* userdata);
GtkNotebook* GET_NOTEBOOK(UserData* userdata) ;
/*
typedef enum EventType {
    OPEN_FILE,
    CLOSE_FILE,
    SAVE_FILE,
    LOAD_FILE,
    NEW_FILE,
    

    SHOW_ERROR
} EventType;

typedef struct _Event {
    EventType type;
    gchar*    data;
} Event;
*/





void
open_file_cb (const gchar* filepath, gchar** text, gsize length, gpointer user_data);

void 
save_file_cb (const gchar* filepath, const gchar* text, gpointer userdata);


#endif




















  /*
     GtkSourceStyleSchemeManager *sm = gtk_source_style_scheme_manager_get_default ();
     const gchar * const * search_path = gtk_source_style_scheme_manager_get_search_path(sm);
     int i=0;
     while (search_path[i]) {
     g_print("%s \n", search_path[i++]);
     }
   */



  /*
     const gchar * const * schemes = gtk_source_style_scheme_manager_get_scheme_ids (sm);
     for (int i=0; *(schemes+i); i++) {
     g_print("%d -> %s \n", i, *(schemes+i));
     }
   */


    // https://developer.gnome.org/gdk3/stable/GdkDisplay.html#gdk-display-beep
    // https://developer.gnome.org/gtk3/stable/gtk-running.html
    // https://github.com/Chai-YD/gitskills

    //list_schemes(sm);



  //gchar* filepath = ".";
  //gchar* filepath = "/home/rafal/Desktop/gtksourceview-4.0.3";
  //gchar* filepath = "/home/rafal/IdeaProjects/vamos-0.8.2-x86_64";
  //gchar* filepath = "/home/rafal/IdeaProjects/vdrift";
  //gchar* filepath = "/home/rafal/IdeaProjects";
  //gchar* filepath = "/home/rafal/IdeaProjects/gtksourceview-my-ide/application";
  //gchar* filepath = "/home/rafal/go/src/wykop.pl";
