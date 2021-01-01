#include "../src/stack.h"

s_change *tmp;

int main() {
	s_node *head = NULL;

	tmp = new_change (0, 0, "1000000", "asa");
	l_append(&head, tmp);
	tmp = new_change (10, 20, "999", "sds");
	l_append(&head, tmp);
	tmp = new_change (10, -10, "aaaaaaa", "aedef");
	l_append(&head, tmp);
	tmp = new_change (2320, 20, "dwefwqdwef", "ewwfef");
	l_append(&head, tmp);
	tmp = new_change (330, -10, "_______", "w22e");
	l_append(&head, tmp);

	l_print(&head);













}
