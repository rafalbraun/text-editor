#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>

typedef struct change {
    long            offset;
    long            length; // if + then insert, if - then overwrite
    gchar*          string;
    gchar*          hash;
} s_change;

typedef struct ynode {
    struct ynode *next;
    s_change* data;

} s_node;

s_change* new_change(long offset, long length, gchar* string, gchar* hash);

int l_init(s_node **head, s_change* data);

int l_insert(s_node **head, s_change* data);

int l_index_of(s_node **head, gchar* value);

s_node* l_at(s_node **head, int index);

int l_append(s_node **head, s_change* data);

void l_print(s_node **head);

void l_apply_changes(s_node **head, gchar* buffer);

#endif
