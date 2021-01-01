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




































/*

typedef struct piece_table {
    t_change** changes_vec;

    gchar   *original;
    s_node  *next;
} s_piece_table;


s_piece_table* s_init (gchar* text) {
    s_piece_table* p = (s_piece_table*)malloc(s_piece_table);
    t_change** c = (t_change**)malloc(t_change**); //????
    p->next = NULL;
    p->original = text;
    return p;
}

void s_apply_change (s_piece_table* p) {
    t_change* c = (t_change*)malloc(t_change);

}




/////////////////////////////

typedef enum node_type {
    INSERT,
    UPDATE,		// INSERT + DELETE
    DELETE
} s_node_type;

typedef struct xnode {
    s_node_type 	type;
    long 		 	offset;
    long 		 	length;

	struct xnode 	*prev;
} s_node;

typedef struct piece_table {
	gchar	*original;
	gchar	*added;
	s_node  *tail;	
} s_piece_table;


int l_init(s_node **head, s_node* data);
*/

/*

typedef struct Node {
    t_NodeType 	 type;
    long 		 offset;
    long 		 length;

    struct Node *prev;
} t_Node;

typedef struct PieceTable {
	gchar	*original;
	gchar	*added;
	t_Node  *nodes;	
} t_PieceTable;


void push_change (Node* last, t_NodeType type, long offset, long length, gchar* delta) {

}

void pop_change (Node* last) {

}
*/




#endif
