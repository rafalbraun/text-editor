#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#ifndef CALLBACK_H
#define CALLBACK_H

#include <gtksourceview/gtksource.h>
#include <gtk/gtk.h>

#include "list.h"
#include "config.h"

gboolean
key_pressed_treeview(GtkWidget *treeview, GdkEventKey *event, gpointer userdata);

gboolean
key_pressed_window(GtkWidget *widget, GdkEventKey *event, gpointer userdata);

gboolean
on_button_pressed(GtkWidget *treeview, GdkEventButton *event, gpointer userdata);

#endif

