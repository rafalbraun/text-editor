#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#ifndef CONFIG_H
#define CONFIG_H

#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>

//GTimer *timer;
//gboolean is_saved = FALSE;

t_node   		*head;
const gchar     *separator;
gchar           *relative_path[128];
gchar           *absolute_path[128];
gchar           *file_contents[128]; // buffer for all files contents
gint            *is_file_saved[128];

typedef struct _UserData
{
    /* main window data */

    t_node		*head;

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
    gchar*		session_info;
    gchar* 		filepath;

    gint untitled_files_in_buffer_max;

} UserData;

UserData *
cast_to_ud (gpointer userdata);

void
ud_init (UserData** userdata);

GtkWindow* 
get_window (UserData* userdata);

GtkTreeView* 
get_treeview (UserData* userdata);

GtkNotebook* 
get_notebook (UserData* userdata);

GtkSourceBuffer* 
get_buffer (UserData* userdata);

GtkMenu* 
get_treeview_menu (UserData* userdata);

GtkMenu* 
get_treeview_menu_expand (UserData* userdata);

GtkMenu* 
get_treeview_menu_collapse (UserData* userdata);

gchar*
get_text_from_eventbox(GtkWidget* widget);

void
show_error (GtkWindow * window, gchar * message);

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
