#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>

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
