#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include "list.h"
#include "config.h"
#include "notebook.h"
#include "treeview.h"
#include "callback.h"
#include "text_editor.h"

#define COLUMN 0

//g_printerr("%s\n",
//       gdk_keyval_name (event->keyval));


gboolean
key_pressed_window(GtkWidget *widget, GdkEventKey *event, gpointer userdata) {
    if (event->state & GDK_CONTROL_MASK && event->keyval == 'F') {
        g_print ("ctrl + shift + f \n");
        full_search_cb (widget, userdata);
    }
    if (event->state & GDK_CONTROL_MASK && event->keyval == 'N') {
        g_print ("ctrl + shift + n \n");
        find_files_cb (widget, userdata);
    }
    if (event->state & GDK_CONTROL_MASK && event->keyval == 's') {
        g_print ("ctrl + s \n");
        save_file_default (userdata);
        //save_file ("/home/rafal/IdeaProjects/gtksourceview-my-ide/application/aaa.c", "Aaaaaaaaaaaaaaaaaaaaa");

    }

    return FALSE;
}
