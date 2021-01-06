#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#ifndef CALLBACK_H
#define CALLBACK_H

#include <gtksourceview/gtksource.h>
#include <gtk/gtk.h>

gboolean print_word_under_mark (gpointer user_data) ;

gboolean
key_pressed_window(GtkWidget *widget, GdkEventKey *event, gpointer userdata);


#endif

