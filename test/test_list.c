#include "../src/list.h"
/*
char _30[] = "30";
char _55[] = "55";
char _210[] = "210";
char _1000000[] = "1000000";
*/
t_tab *_30, *_55, *_210, *_1000000; 

int main() {
	struct node *head = NULL;

	_30 = new_tab ("30");
	_55 = new_tab ("55");
	_210 = new_tab ("210");
	_1000000 = new_tab ("1000000");


	l_append(&head, _30);
	l_append(&head, _30);
	l_append(&head, _55);
	l_append(&head, _210);
	l_append(&head, _210);
	l_append(&head, _210);
	l_append(&head, _1000000);

	puts("\nPrint the linked list:");
	l_print(&head);

	//delete_at(&head, 0);
	//delete_at(&head, 3);
	puts("\nDelete _30:");
	l_delete_value(&head, _30->title);

	puts("\nPrint the linked list:");
	l_print(&head);

	puts("\nPrint element _210:");
	int index = l_index_of(&head, _210->title);
	g_print("%d -> %s \n", index, _210->title);

}

