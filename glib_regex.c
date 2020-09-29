#include <stdio.h>

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include <glib.h>
#include <glib/gstdio.h>

#include <stdlib.h>
#include <pthread.h>

#define BUFFER 100

/**
 * https://codeforwin.org/2018/03/c-program-to-list-all-files-in-a-directory-recursively.html
 *
 */

typedef struct _search_data {
    char* dirname;
    char* pattern;
} search_data;

void scan_file(char const* const filename, char const* const pattern) {
    //char const* const fileName = "read_lines.c"; /* should check that argc > 1 */
    FILE* file = fopen(filename, "r");    /* should check the result */
    int linenum = 1;
    char line[BUFFER];
    GMatchInfo *match_info = NULL;
    gint match_num, start_pos, end_pos;
    GRegex *regex;
    GError *err = NULL;

    regex = g_regex_new (pattern, 0, 0, &err);
    //g_print("[FILE] %s \n", filename);

    while (fgets(line, sizeof(line), file)) {
        /* note that fgets don't strip the terminating \n, checking its
           presence would allow to handle lines longer that sizeof(line) */
        //printf("%d:%s", counter, line); 

        g_regex_match (regex, line, 0, &match_info);
        while (g_match_info_matches (match_info))
        {
            gchar *word = g_match_info_fetch (match_info, 0);

            g_match_info_fetch_pos(match_info, 0, &start_pos, &end_pos);

            //line[strlen(line)-1] = '\0'; // to overwrite \n with \0
            //g_print ("[MATCH] line: %d, \tstart: %d, \tend: %d, \t[%s][%s] \n", counter, start_pos, end_pos, filename, line);
            g_print ("%s\x1C%d\x1C%d\x1C%d\x1C%s", filename, linenum, start_pos, end_pos, line);

            g_free (word);
            g_match_info_next (match_info, NULL);
        }
        g_match_info_free (match_info);
        linenum++;
    }
    /* may check feof here to make a difference between eof and io failure -- network
       timeout for instance */

    g_regex_unref (regex);
    fclose(file);
}

int list_directory( char* dirname, char* pattern ) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL); 
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    //void list_directory(char const* const dirname, char const* const pattern) {
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
            //printf("%s\n", dp->d_name);

            // Construct new path from our base path
            strcpy(path, dirname);
            strcat(path, "/");
            strcat(path, dp->d_name);

            //printf("%s\n", path);
            scan_file(path, pattern);

            list_directory(path, pattern);
        }
    }

    closedir(dir);
}

void *topfun( void *ptr ) {
    char* dirname = ((search_data*)ptr)->dirname;
    char* pattern = ((search_data*)ptr)->pattern;
    list_directory(dirname, pattern);
}

// https://stackoverflow.com/questions/9206091/going-through-a-text-file-line-by-line-in-c/9206332
// https://developer.gnome.org/glib/stable/glib-String-Utility-Functions.html#g-strsplit
// https://developer.gnome.org/glib/stable/glib-File-Utilities.html#GMappedFile
// https://developer.gnome.org/glib/stable/glib-Perl-compatible-regular-expressions.html#g_regex_match   !!!
//
// lpthread keyboard press cancel thread
// https://www.quora.com/How-do-I-interrupt-an-infinite-loop-in-C++-using-keyboard-hit
/*
./glib_regex /usr/include/glib-2.0 int
/home/rafal/IdeaProjects/gtksourceview-my-ide/application/glib_regex /usr/include/glib-2.0 int



./glib_regex /home/rafal/IdeaProjects/gtksourceview-my-ide/application/search_path int


*/

#if !SEARCH_PATH

int main(int argc, char* argv[]) {
    pthread_t thread;
    //char *dirname = "/usr/include";
    //if (argc != 3) {
    //    g_print("Bad argument number (%d): %s %s %s \n", argc, argv[0], argv[1], argv[2]);
    //    exit(1);
    //}
    //char *dirname = "/usr/include/glib-2.0";
    //char *pattern = "int";
    void* ret; 
    char* dirname = argv[1];
    char* pattern = argv[2];
    //char c;

    search_data* data = g_new0(search_data, 1);;
    data->dirname = dirname;
    data->pattern = pattern;
    //struct DispatchData *data = g_new0(struct DispatchData, 1);

    if(pthread_create(&thread, NULL, topfun, (void*) data) != 0) {
        printf("pthread create failed\n"); 
        exit(1); 
    }
    if(pthread_join(thread, &ret)<0){
        perror("thread join failed\n");
        exit(1);
    }

    //printf("Thread 1 returns: %d\n",iret);

    
    // while(getchar()){
    //     if(pthread_cancel(thread)<0){
    //         perror("thread cancel failed");
    //         exit(1); 
    //     } 
    //     if(pthread_join(thread, &ret)<0){
    //         perror("thread join failed\n");
    //         exit(1); 
    //     } 
    //     if(ret == PTHREAD_CANCELED){ 
    //         printf("cancellation success\n");
    //         break; 
    //     } 
    //     else{ 
    //         printf("error in return status after cancellation\n"); 
    //         exit(1); 
    //     } 
    // } 

    //list_directory(".");
    //list_directory("/home/rafal/IdeaProjects/gtksourceview-my-ide/application");
    //list_directory("/home/rafal/IdeaProjects/gtksourceview-my-ide/application/search_path");
    //list_directory(data);

    return 0;
}

#endif
