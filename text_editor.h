#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <gtksourceview/gtksource.h>
#include <gtk/gtk.h>

#include <glib/gstdio.h>

#include <sys/stat.h>
#include <fcntl.h>

#include "config.h"
#include "sourceview.h"
#include "treeview.h"
#include "notebook.h"
#include "callback.h"
#include "text_editor.h"

/*
gchar *open_files ();
void open_file (gpointer userdata, gchar * filepath);
void full_search_cb (GtkWidget * widget, gpointer userdata);
void find_files_cb (GtkWidget * widget, gpointer userdata);
*/

void
on_main_quit (GtkWidget * widget, gpointer userdata);

void
new_file_cb (GtkButton * widget, gpointer userdata);

void
open_files_from_last_session (gpointer userdata);

#endif
