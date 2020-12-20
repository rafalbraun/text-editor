#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>

typedef gchar datatype;
typedef struct node {
	struct node *next;
	datatype* data;

} t_node;

int l_init(struct node **head, datatype* data);

int l_insert(struct node **head, datatype* data);

int l_index_of(struct node **head, datatype* value);

int l_delete_value(struct node **head, datatype* data);

struct node* l_at(struct node **head, int index);

int l_append(struct node **head, datatype* data);

void l_print(struct node **head);

#endif

