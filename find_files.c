#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtk/gtk.h>

#define BUFFER_SIZE 1048576     		// 1 MB
#define SIZE 1000

//int stdout_fd = -1;
gchar buffer [BUFFER_SIZE];
FILE *fp;

void scan_line(gchar const* const line, gchar const* const pattern) {
    GMatchInfo *match_info = NULL;
    gint match_num, start_pos, end_pos;
    GRegex *regex;
    GError *err = NULL;

    regex = g_regex_new (pattern, 0, 0, &err);

    g_regex_match (regex, line, 0, &match_info);
    while (g_match_info_matches (match_info))
    {
        gchar *word = g_match_info_fetch (match_info, 0);

        g_match_info_fetch_pos(match_info, 0, &start_pos, &end_pos);
        //g_print ("%s\x1C%d\x1C%d\x1C%d\x1C%s", filename, linenum, start_pos, end_pos, line);
        g_print("%d / %d / %s \n", start_pos, end_pos, line);

        g_free (word);
        g_match_info_next (match_info, NULL);
    }
    g_match_info_free (match_info);
}

void scan_dir(gchar const* const topdir, gchar const* const pattern) {
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

            //g_print("DT_DIR: %s \n", path);

            scan_dir(path, pattern);
        } else if (entry -> d_type == DT_REG) {
            gchar path[SIZE];

  			snprintf(path, sizeof(path), "%s/%s", topdir, entry->d_name);

            //g_print("DT_REG: %s/%s \n", topdir, entry->d_name);

			//snprintf(path, sizeof(path), "%s/%s", topdir, entry -> d_name);
        	scan_line(path, pattern);

        } else {

        	//g_print("Not a dir nor a regular file: %s/%s \n", topdir, entry->d_name);
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

	scan_dir("/usr/include", "gtk");
	//scan("/");
	//scan("/home/rafal");
	
	fclose(fp);

	return 0;
}
