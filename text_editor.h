#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <gtksourceview/gtksource.h>
#include <gtk/gtk.h>

#include <glib/gstdio.h>

#include <sys/stat.h>
#include <fcntl.h>

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

void
set_language(GObject* buffer);

void
set_buffer_scheme (GObject* buffer);

void
set_syntax_submenu (GtkMenuItem * menuitem);

void
list_schemes (GtkSourceStyleSchemeManager * sm);

void
set_langs_dir (GtkSourceBuffer * buffer);

void
full_search_cb (GtkWidget * widget, gpointer userdata);

void
find_files_cb (GtkWidget * widget, gpointer userdata);

gboolean
key_pressed_notebook (GtkWidget * notebook, GdkEventKey * event, gpointer userdata);


#endif
