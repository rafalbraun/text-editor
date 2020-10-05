/* 
	(c) 2017, Arkadiusz Drabczyk, arkadiusz@drabczyk.org 
	source: https://gist.github.com/ardrabczyk/1cdc413b9eaa9a9a0c4be71d3fc9bc37
*/

#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <stdio.h>
#include <stdlib.h>

#include <gtk/gtk.h>

typedef gchar datatype;
struct node {
	struct node *next;
	datatype* data;

};

/*
typedef struct node {
    int val;
    struct node* next;
} node_t;
*/

/* PRIVATE */
int init(struct node **head, datatype* data)
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
int insert(struct node **head, datatype* data)
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
void delete(struct node **head, datatype* data)
{
	struct node *current = *head;
	struct node *prev = NULL;

	do {
		if (current->data == data) {
			break;
		}
		prev = current;
		current = current->next;
	} while (current);

	// if the first element 
	if (current == *head) {
		// reuse prev 
		prev = *head;
		*head = current->next;
		free(prev);
		return;
	}

	// if the last element 
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
struct node* at(struct node **head, int index) {
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
int index_of(struct node **head, gchar* value) {
	struct node *current = *head;
	int index = 0;

	while ( strcmp(current->data, value) == 0 ) {
		if (current->data) {
			return index;
		}
		current = current->next;
		index++;
	}
	return -1;
}

/* PUBLIC */
void delete_at(struct node **head, int index) {
	struct node* item = at(head, index);
	datatype* data = item->data;
	delete(head, data);
}

/* PUBLIC */
void append(struct node **head, datatype* data) {
	if (*head == NULL) {
		if (init(head, data) != 0) {
			fprintf(stderr, "Failed to init a new linked list");
			exit(1);
		}
	} else {
		insert(head, data);
	}
}

/* PUBLIC */
void print(struct node **head)
{
	struct node *current = *head;
	while (current) {
		printf("current data: %s, address: %p\n", current->data,
		       current);
		current = current->next;
	}
}

#if !LIST

int main() {
	struct node *head;
	/*
	if (init(&head, 25) != 0) {
		fprintf(stderr, "Failed to init a new linked list");
		return EXIT_FAILURE;
	}
	insert(&head, 30);
	insert(&head, 55);
	insert(&head, 210);
	insert(&head, 1000000);
	*/

	append(&head, "30");
	append(&head, "30");
	append(&head, "55");
	append(&head, "210");
	append(&head, "1000000");

	puts("Print the linked list:");
	print(&head);

	//for (int i=0; i<5; i++) {
	//	fprintf(stdout, "Print element %d : %d \n", i, at(&head, i)->data);
	//}

	delete_at(&head, 0);
	delete_at(&head, 3);

	puts("Print the linked list:");
	print(&head);

}

#endif
