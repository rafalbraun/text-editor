#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#ifndef SOURCEVIEW_H
#define SOURCEVIEW_H

#include <gtksourceview/gtksource.h>
#include <gtk/gtk.h>

GtkTextIter start, end;
int active;

//int active; = 0; // potrzebne przy najeżdżaniu kursorem na słowo

void
guess_language(GtkSourceBuffer* buffer, gchar* filepath);

void show_langs();

void
clear_buffer(GtkSourceBuffer* buffer);

gboolean
is_valid_string(gchar* line);

gchar* 
extract_word(gchar* line, gint offset, GtkTextIter* iter, GtkTextBuffer* sourcebuff, GtkWidget* scroll);

gboolean
mouse_moved(GtkWidget *widget, GdkEvent *event, gpointer scroll);

GtkWidget*
sourceview_new(GtkSourceBuffer* buffer);

void set_language (GtkSourceBuffer* buffer, const char* lang);

#endif




