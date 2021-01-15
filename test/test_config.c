#include "../src/config.h"

//t_tab *_30, *_55, *_210, *_1000000; 

void
print_list (gpointer data, gpointer user_data) 
{
    g_print("[ %s ] \n", ((t_tab*)data)->title);
}

int main() {
	GList *list = NULL;
	gint index;

	/*
	_30 = new_tab ("30");
	_55 = new_tab ("55");
	_210 = new_tab ("210");
	_1000000 = new_tab ("1000000");
	*/


	append_tab(&list, "_30");
	append_tab(&list, "_30");
	append_tab(&list, "_55");
	append_tab(&list, "_210");
	append_tab(&list, "_210");
	append_tab(&list, "_210");
	append_tab(&list, "_1000000");

	puts("\nPrint the linked list:");
    g_list_foreach (list, (GFunc) print_list, NULL);

	puts("\nPrint 2nd element:");
	t_tab* data = get_nth (list, 2);
	g_print("%s\n", data->title);

	puts("\nDelete 2nd element:");
	delete_tab (&list, data);
	puts("\nPrint the linked list:");
    g_list_foreach (list, (GFunc) print_list, NULL);

	puts("\nPrint index of tab _210:");
	index = index_tab (list, "_210");
	g_print("Index: %d\n", index);

	puts("\nPrint index of tab aaa:");
	index = index_tab (list, "aaa");
	g_print("Index: %d\n", index);

/*
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
*/
}