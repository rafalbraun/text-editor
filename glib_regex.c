#include <stdio.h>

#include <glib.h>
#include <glib/gstdio.h>

#define BUFFER 1000

// https://stackoverflow.com/questions/9206091/going-through-a-text-file-line-by-line-in-c/9206332
// https://developer.gnome.org/glib/stable/glib-String-Utility-Functions.html#g-strsplit
// https://developer.gnome.org/glib/stable/glib-File-Utilities.html#GMappedFile
// https://developer.gnome.org/glib/stable/glib-Perl-compatible-regular-expressions.html#g_regex_match   !!!
int main(int argc, char* argv[])
{
    char const* const fileName = "read_lines.c"; /* should check that argc > 1 */
    FILE* file = fopen(fileName, "r");    /* should check the result */
    char line[BUFFER];
    int counter = 0;
    gboolean result;
    GMatchInfo *match_info = NULL;
    gint match_num, start_pos, end_pos;
    GRegex *regex;
    GError *err = NULL;

    regex = g_regex_new ("int", 0, 0, &err);

    while (fgets(line, sizeof(line), file)) {
        /* note that fgets don't strip the terminating \n, checking its
           presence would allow to handle lines longer that sizeof(line) */
        printf("%d:%s", counter, line); 

        g_regex_match (regex, line, 0, &match_info);
        while (g_match_info_matches (match_info))
        {
            gchar *word = g_match_info_fetch (match_info, 0);
            g_print ("Found: %s\n", word);

            g_match_info_fetch_pos(match_info, 0, &start_pos, &end_pos);
            g_print ("start: %d, end: %d \n", start_pos, end_pos);

            g_free (word);
            g_match_info_next (match_info, NULL);
        }
        g_match_info_free (match_info);



        counter++;
    }
    /* may check feof here to make a difference between eof and io failure -- network
       timeout for instance */

    g_regex_unref (regex);
    fclose(file);

    return 0;
}


































/*
 * gcc `pkg-config --cflags --libs glib-2.0` glib_regex.c
 *
 */
/*
gchar *contents;
gsize len;
GBytes *bytes;
GError *err = NULL;
const char* filename = "/home/rafal/IdeaProjects/gtksourceview-my-ide/highlight_syntax/highlight_syntax.c";

int main () {
	GError *err = NULL;
	GMatchInfo *matchInfo;
	GRegex *regex;
	gint match_num, start_pos, end_pos;

    if (g_file_get_contents(filename, &contents, &len, &err) == FALSE) {
        g_error("error reading %s: %s", filename, err->message);
    }

    regex = g_regex_new ("int", G_REGEX_DOTALL, 0, &err);
    g_regex_match (regex, contents, 0, &matchInfo);

	while (g_match_info_matches (matchInfo)) {
		gchar *result = g_match_info_fetch (matchInfo, 0);

		g_print ("mykey=%s\n", result);

		g_match_info_fetch_pos(matchInfo, 0, &start_pos, &end_pos);
		g_print ("start: %d, end: %d \n", start_pos, end_pos);

		g_match_info_next (matchInfo, &err);
		g_free (result);
	}

}
*/