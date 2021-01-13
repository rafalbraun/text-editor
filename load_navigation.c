#include <stdio.h>
#include <glib/gstdio.h>

#define FILENAME "/home/rafal/IdeaProjects/gtksourceview-my-ide/application/code_info.txt"
#define BUFFER 128

// gcc load_navigation.c `pkg-config --cflags --libs glib-2.0`

typedef struct node
{
    int start, end;

    char* function_name;
    char* source_file;
    char* type; // i for implementation, d for definition

} t_node;

static
int sort_func (gconstpointer a, gconstpointer b) 
{
    t_node *t1 = (t_node *) a;
    t_node *t2 = (t_node *) b;

    if (g_strcmp0 (t1->function_name, t2->function_name) >= 0) 
    {
        return 1;
    } else {
        return 0;
    }
}

GList* list = NULL;

void
print_list (gpointer data, gpointer user_data) 
{

    t_node* node_data = (t_node*)data;

    g_print("[ %s ] %s \n", node_data->type, node_data->function_name);

}

int main(int argc, char* argv[])
{
    char const* const fileName = FILENAME; /* should check that argc > 1 */
    FILE* file = fopen(fileName, "r"); /* should check the result */
    char line[256];
    int eof = 0;
    char src_name[BUFFER], type[BUFFER], func[BUFFER];
    int rows, start, end;
    t_node* data;

    while (eof != 1) 
    {
        fscanf (file, "%d %s\n", &rows, &src_name);
        printf ("rows: %d, src_name: %s \n", rows, src_name);
        if (rows == 0) 
        {
            break;
        } else 
        {
            for (int i=0; i<rows; i++)
            {
                fscanf (file, "%d\t%d\t%s\t%s\n", &start, &end, &type, &func);
                printf("\tstart %d, end %d, type %s, func %s\n", start, end, type, func);

                //list = g_list_insert_sorted (list, data, (GCompareFunc) sort_func);
                data = (t_node*) g_new0 (t_node, 1);

                data->function_name = g_strdup (func);
                data->type = g_strdup (type);

                list = g_list_prepend (list, data);
            }
        }
    }

    list = g_list_sort (list, sort_func);

    g_list_foreach (list, (GFunc) print_list, NULL);

    /*
    while (fgets(line, sizeof(line), file)) {
        // note that fgets don't strip the terminating \n, checking its
        //   presence would allow to handle lines longer that sizeof(line)
        printf("%s", line); 
    }*/
    /* may check feof here to make a difference between eof and io failure -- network
       timeout for instance */

    fclose(file);

    return 0;
}
