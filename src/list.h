#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>

typedef struct t {
	gchar* title;
} t_tab;

//typedef t_tab datatype;

typedef struct node {
	struct node *next;
	t_tab* data;

} t_node;

t_tab* new_tab(gchar* title);

int l_init(struct node **head, t_tab* data);

int l_insert(struct node **head, t_tab* data);

int l_index_of(struct node **head, gchar* value);

int l_delete_value(struct node **head, gchar* data);

struct node* l_at(struct node **head, int index);

int l_append(struct node **head, t_tab* data);

void l_print(struct node **head);

#endif

