#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include "list.h"

/* PRIVATE */
int compare (t_tab* tab, gchar* title)
{
	return (strcmp(tab->title, title)==0);
}

/* PUBLIC */
t_tab* new_tab(gchar* title, gchar* text) 
{
	t_tab* new_tab = (t_tab*)malloc(sizeof(t_tab));
	new_tab->title = g_strdup(title);
	new_tab->tab_buffer = (gchar*)malloc(1024*1024);
	strcpy(new_tab->tab_buffer, text);
	return new_tab;
}

/* PRIVATE */
int l_init(t_node **head, t_tab* data)
{
	*head = malloc(sizeof(t_node));
	if (!*head) {
		fprintf(stderr, "Failed to init a linked list\n");
		return 1;
	}

	(*head)->data = data;
	(*head)->next = NULL;

	return 0;
}

/* PRIVATE */
int l_insert(t_node **head, t_tab* data)
{
	t_node *current = *head;
	t_node *tmp;

	do {
		tmp = current;
		current = current->next;
	} while (current);

	/* create a new node after tmp */
	t_node *new = malloc(sizeof(t_node));
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
int l_index_of(t_node **head, gchar* value) {
	t_node *current = *head;
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
/*
int l_index_by_title() {

}
*/

/* PUBLIC */
int l_delete_value(t_node **head, gchar* value)
{
	t_node *current = *head;
	t_node *prev = NULL;
	int index = l_index_of(head, value);

	g_print("%s :: %s \n", current->data->title, value);

	do {
		if (compare(current->data, value)) {
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
		free(prev->data->tab_buffer);
		free(prev->data->title);
		free(prev);
		return index;
	}

	/* if the last element */
	if (current->next == NULL) {
		prev->next = NULL;
		free(current->data->tab_buffer);
		free(current->data->title);
		free(current);
		return index;
	}

	prev->next = current->next;
	free(current->data->tab_buffer);
	free(current->data->title);
	free(current);
	return index;
}


/* PUBLIC */
t_node* l_at(t_node **head, int index) {
	t_node *current = *head;
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
/*
void delete_at(struct node **head, int index) {
	struct node* item = at(head, index);
	datatype* data = item->data;
	delete_value(head, data);
}
*/
/* PUBLIC */
int l_append(t_node **head, t_tab* data) {
	int index = l_index_of(head, data->title);
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

/* PUBLIC */
void l_print(t_node **head)
{
	t_node *current = *head;
	while (current) {
		printf("current data: %s, address: %p\n", current->data->title,
		       current);
		current = current->next;
	}
}

