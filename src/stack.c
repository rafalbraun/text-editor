#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include "stack.h"

/* PRIVATE */
int l_init(s_node **head, s_node* data)
{
	*head = malloc(sizeof(s_node));
	if (!*head) {
		fprintf(stderr, "Failed to init a linked list\n");
		return 1;
	}

	//(*head)->data = data;
	(*head)->prev = NULL;

	return 0;
}














/*
void create_node () {
	
	return;
}

void apply_change () {

}

void load_node () {

}

void show_nodes () {
	
}
*/
