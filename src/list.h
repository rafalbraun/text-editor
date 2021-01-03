#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>

typedef struct t {
	gchar* title;
	gchar* relative_path;
	gchar* absolute_path;
	gint   is_file_saved;
	//gchar* tab_buffer;
	GtkWidget* buffer;

} t_tab;

//typedef t_tab datatype;

typedef struct ynode {
	struct ynode *next;
	t_tab* data;

} t_node;

t_tab* new_tab(gchar* title);

int l_init(t_node **head, t_tab* data);

int l_insert(t_node **head, t_tab* data);

int l_index_of(t_node **head, gchar* value);

int l_delete_value(t_node **head, gchar* data);

t_node* l_at(t_node **head, int index);

int l_append(t_node **head, t_tab* data);

void l_print(t_node **head);

#endif

