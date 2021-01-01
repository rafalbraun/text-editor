#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include "stack.h"

/* PRIVATE */
int compare (s_change* change, gchar* hash)
{
	return (strcmp(change->hash, hash)==0);
}

s_change* new_change(long offset, long length, gchar* string, gchar* hash) {
	s_change* new_change = (s_change*)malloc(sizeof(s_change));
	new_change->hash = hash;
	new_change->string = string;
	new_change->offset = offset;
	new_change->length = length;
	return new_change;
}

int l_insert(s_node **head, s_change* data) {
	s_node *current = *head;
	s_node *tmp;

	do {
		tmp = current;
		current = current->next;
	} while (current);

	/* create a new node after tmp */
	s_node *new = malloc(sizeof(s_node));
	if (!new) {
		fprintf(stderr, "Failed to insert a new element\n");
		return 1;
	}
	new->next = NULL;
	new->data = data;

	tmp->next = new;

	return 0;
}

s_node* l_at(s_node **head, int index) {
	return NULL;
}

int l_index_of(s_node **head, gchar* value) {
	s_node *current = *head;
	int index = -1;

	if (current == NULL) {
		return -1;
	}
	while (current) {
		index++;
		//g_print("aaaaaaaa %d \n", index);
		if (compare(current->data, value)) {
			return index;
		}
		current = current->next;
	}
	return -1;
}

int l_init(s_node **head, s_change* data) {
	*head = malloc(sizeof(s_node));
	if (!*head) {
		fprintf(stderr, "Failed to init a linked list\n");
		return 1;
	}
	(*head)->data = data;
	(*head)->next = NULL;

	return 0;	
}

int l_append(s_node **head, s_change* data) {
	int index = l_index_of(head, data->hash);
	if (index != -1) {
		return index;
	}
	if (*head == NULL) {
		if (l_init(head, data) != 0) {
			fprintf(stderr, "Failed to init a new linked list");
			exit(1);
		}
		return -1;
	} else {
		l_insert(head, data);
		return -1;
	}
}

void l_print(s_node **head) {
	s_node *current = *head;
	while (current) {
		printf("[current data] string:%s, offset: %d, length: %d, next: %p, address: %p\n", 
				current->data->string,
				current->data->offset,
				current->data->length,
				current->next,
		       	current);
		current = current->next;
	}
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
