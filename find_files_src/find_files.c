#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtk/gtk.h>

#define BUFFER_SIZE 1048576     		// 1 MB
#define SIZE 1000

gchar buffer [BUFFER_SIZE];

int check_regex(gchar* pattern) {
    GRegex *regex;
    GError *err = NULL;
    regex = g_regex_new (pattern, 0, 0, &err);

    if (err != NULL) {
        g_error("Not valid regex expression.\n");
        exit(0);
    }
}

void scan_line(gchar const* const line, gchar const* const pattern) {
    GMatchInfo *match_info = NULL;
    gint match_num, start_pos, end_pos;
    GRegex *regex;
    GError *err = NULL;

    regex = g_regex_new (pattern, 0, 0, &err);

    g_regex_match (regex, line, 0, &match_info);
    if (g_match_info_matches (match_info))
    {
        gchar *word = g_match_info_fetch (match_info, 0);

        g_match_info_fetch_pos(match_info, 0, &start_pos, &end_pos);
        g_print("%s\n", line);

        g_free (word);
        g_match_info_next (match_info, NULL);
    }
    g_match_info_free (match_info);
    g_regex_unref (regex);
}

void scan_dir(gchar const* const topdir, gchar const* const pattern) {
    struct dirent * entry;
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
            scan_dir(path, pattern);
        } else if (entry -> d_type == DT_REG) {
            gchar path[SIZE];

  			snprintf(path, sizeof(path), "%s/%s", topdir, entry->d_name);
        	scan_line(path, pattern);
        }
    }
    closedir(dir);
}

/*

./find_files "/usr/include" "gtk"
/home/rafal/IdeaProjects/gtksourceview-my-ide/application/find_files /usr/include gtk

*/
int main(int argc, char* argv[]) {

	gchar* dirname = argv[1];
	gchar* pattern = argv[2];

    check_regex(pattern);
	scan_dir(dirname, pattern);

	return 0;
}






