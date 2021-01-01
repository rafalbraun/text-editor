#include "../src/stack.h"

#define BUFFER_SIZE 1024

s_change *tmp;
gchar buffer[BUFFER_SIZE];

int main() {
	s_node *head = NULL;

	// original
	tmp = new_change (0, 0, "original", "asa");
	l_append(&head, tmp);

	// original999
	tmp = new_change (7, 0, "999", "sds");
	l_append(&head, tmp);

	// original999aaaaaaa
	tmp = new_change (10, 0, "aaaaaaa", "aedef");
	l_append(&head, tmp);

	// original999a1234567890aaaaaa
	tmp = new_change (11, 0, "1234567890", "ewwfef");
	l_append(&head, tmp);

	// orig_____inal999a1234567890aaaaaa
	tmp = new_change (3, 0, "_____", "w22e");
	l_append(&head, tmp);

	l_print(&head);


	// jeśli offset wiekszy niz dlugosc poprzedniego -> error

	l_apply_changes(&head, buffer);
	printf("%s\n", buffer);








}
