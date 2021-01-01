#include "../src/stack.h"

#define BUFFER_SIZE 1024*1024

// how to remember stash in git ???? - czysta ciekawość

s_change *tmp;
gchar buffer[BUFFER_SIZE];

int main() {
	s_node *head = NULL;

	// original
	tmp = new_change (0, 0, "original", "asa2");
	l_append(&head, tmp);

	// original999
	tmp = new_change (7, 0, "999", "sds4");
	l_append(&head, tmp);

	// original999aaaaaaa
	tmp = new_change (10, 0, "aaaaaaa", "a23d");
	l_append(&head, tmp);

	// original999a1234567890aaaaaa
	tmp = new_change (11, 0, "1234567890", "e23a");
	l_append(&head, tmp);

	// orig_____inal999a1234567890aaaaaa
	tmp = new_change (3, 0, "_____", "w22e");
	l_append(&head, tmp);


	l_print(&head);


	// jeśli offset wiekszy niz dlugosc poprzedniego -> error

	l_apply_changes(&head, buffer);
	printf("%s\n", buffer);








}
