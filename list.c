/* 
	(c) 2017, Arkadiusz Drabczyk, arkadiusz@drabczyk.org 
	source: https://gist.github.com/ardrabczyk/1cdc413b9eaa9a9a0c4be71d3fc9bc37
*/

#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>

//#define g_print printf
//typedef char gchar;

typedef gchar datatype;
typedef struct node {
	struct node *next;
	datatype* data;

} t_node;

/*
typedef struct node {
    int val;
    struct node* next;
} node_t;
*/

/* PRIVATE */
int l_init(struct node **head, datatype* data)
{
	*head = malloc(sizeof(struct node));
	if (!*head) {
		fprintf(stderr, "Failed to init a linked list\n");
		return 1;
	}

	(*head)->data = data;
	(*head)->next = NULL;

	return 0;
}

/* PRIVATE */
int l_insert(struct node **head, datatype* data)
{
	struct node *current = *head;
	struct node *tmp;

	do {
		tmp = current;
		current = current->next;
	} while (current);

	/* create a new node after tmp */
	struct node *new = malloc(sizeof(struct node));
	if (!new) {
		fprintf(stderr, "Failed to insert a new element\n");
		return 1;
	}
	new->next = NULL;
	new->data = data;

	tmp->next = new;

	return 0;
}

/* PUBLIC */
/*
void deinit(struct node **head)
{
	struct node *node = *head;
	do {
		struct node *tmp;
		tmp = node;
		node = node->next;
		free(tmp);
	} while (node);
}
*/


/* PUBLIC */
void l_delete_value(struct node **head, datatype* data)
{
	struct node *current = *head;
	struct node *prev = NULL;

	do {
		if (strcmp(current->data, data)==0) {
			break;
		}
		prev = current;
		current = current->next;
	} while (current);

	/* if the first element */
	if (current == *head) {
		/* reuse prev */
		prev = *head;
		*head = current->next;
		free(prev);
		return;
	}

	/* if the last element */
	if (current->next == NULL) {
		prev->next = NULL;
		free(current);
		return;
	}

	prev->next = current->next;
	free(current);
	return;
}


/* PUBLIC */
struct node* l_at(struct node **head, int index) {
	struct node *current = *head;
	int iter = 0;

	while (current) {
		//printf("current data: %d, address: %p\n", current->data, current);
		if (iter == index) {
			return current;
		} else {
			current = current->next;
			iter++;
		}
	}

	if ((current == NULL) && (index > iter)) {
		fprintf(stderr, "Failed to access element: index out of bounds\n");
		exit(1);
	}
}

/* PUBLIC */
int l_index_of(struct node **head, datatype* value) {
	struct node *current = *head;
	int index = -1;

	if (current == NULL) {
		return -1;
	}
	while (current) {
		index++;
		//g_print("aaaaaaaa %d \n", index);
		if (strcmp(current->data, value) == 0) {
			return index;
		}
		current = current->next;
	}
	return -1;
}

/* PUBLIC */
/*
void delete_at(struct node **head, int index) {
	struct node* item = at(head, index);
	datatype* data = item->data;
	delete_value(head, data);
}
*/
/* PUBLIC */
int l_append(struct node **head, datatype* data) {
	int index = l_index_of(head, data);
	if (index != -1) {
		return index;
	}
	if (*head == NULL) {
		if (l_init(head, data) != 0) {
			fprintf(stderr, "Failed to init a new linked list");
			exit(1);
		}
	} else {
		l_insert(head, data);
		return -1;
	}
}

/* PUBLIC */
void l_print(struct node **head)
{
	struct node *current = *head;
	while (current) {
		printf("current data: %s, address: %p\n", current->data,
		       current);
		current = current->next;
	}
}

#if !LIST

char _30[] = "30";
char _55[] = "55";
char _210[] = "210";
char _1000000[] = "1000000";

int main() {
	struct node *head = NULL;

	l_append(&head, _30);
	l_append(&head, _30);
	l_append(&head, _55);
	l_append(&head, _210);
	l_append(&head, _210);
	l_append(&head, _210);
	l_append(&head, _1000000);

	puts("Print the linked list:");
	l_print(&head);

	//delete_at(&head, 0);
	//delete_at(&head, 3);
	l_delete_value(&head, _30);

	puts("Print the linked list:");
	l_print(&head);

	puts("Print the linked list:");
	int index = l_index_of(&head, _210);
	g_print("%d -> %s \n", index, _210);

}

#endif
