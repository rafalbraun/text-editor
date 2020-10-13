#include <stdio.h>
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

	return 0;
}