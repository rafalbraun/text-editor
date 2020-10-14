#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtk/gtk.h>

#define BUFFER_SIZE 1048576     // 1 MB
#define SIZE 1000

//int stdout_fd = -1;
gchar buffer [BUFFER_SIZE];

FILE *fp;

void scan(gchar* topdir) {
    struct dirent * entry;
    //GtkTreeIter child;
    DIR * dir;

    if (!(dir = opendir(topdir))) {
        return;
    }

    while ((entry = readdir(dir)) != NULL) {

        if (entry -> d_name[0] == '.') {
            continue;
        }

        if (entry -> d_type == DT_DIR) {
            gchar path[SIZE];

            if (g_strcmp0(entry -> d_name, ".") == 0 || g_strcmp0(entry -> d_name, "..") == 0) {
                continue;
            }

            snprintf(path, sizeof(path), "%s/%s", topdir, entry -> d_name); // create name of subdirectory

            //gtk_tree_store_append(treestore, &child, &toplevel);
            //gtk_tree_store_set(treestore, &child, COLUMN, entry -> d_name, -1);

            scan(path);
        } else if (entry -> d_type == DT_REG) {

            //gtk_tree_store_append(treestore, &child, &toplevel);
            //gtk_tree_store_set(treestore, &child, COLUMN, entry -> d_name, -1);

            //g_print("DT_REG: %s/%s \n", topdir, entry->d_name);

  			fprintf(fp, "%s/%s\n", topdir, entry->d_name);

        } else {

        	g_print("Not a dir nor a regular file.\n");
        }
    }
    closedir(dir);
}

/*
#define BUFFER 500

int list_directory( char* dirname, char* pattern ) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL); 
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(dirname);

    // Unable to open directory stream
    if (!dir) {
        return 0;
    }

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0 && (dp->d_name[0]!='.'))
        {
            // Construct new path from our base path
            strcpy(path, dirname);
            strcat(path, "/");
            strcat(path, dp->d_name);

            scan_file(path, pattern);

            list_directory(path, pattern);
        }
    }

    closedir(dir);
}
*/

int main() {

	fp = fopen("/home/rafal/test.txt", "w+");
	
	scan("/home/rafal");
	
	fclose(fp);

	return 0;
}