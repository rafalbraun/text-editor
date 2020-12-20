#include "../src/list.h"

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

