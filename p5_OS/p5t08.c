#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h> /* PATH_MAX */
#include <string.h> /* strcmp() */
#include <stdlib.h> /* calloc() */

/* to recursively file search, I don't actually need distinguish file
 * and directory. */

void find_file(const char *filename, const char *dirname)
{
    DIR *d;
    struct dirent *dent; /* "directory entry" */
    char filepath[PATH_MAX];
    d = opendir(dirname);
    if(!d) /* if d is file, d has not permission, other errors. */
        return;
    /* compose paths of subdirectories and call function recursively */
    while ( (dent = readdir(d)) != NULL ) {
        if ( !strcmp(dent->d_name, ".") || !strcmp(dent->d_name, "..") )
            continue;
        snprintf(filepath, PATH_MAX, "%s/%s", dirname, dent->d_name);
        find_file(filename, filepath);
        if ( !strcmp(filename, dent->d_name) ){
            printf("%s\n", filepath);
        }
    }
}

int main(int argc, char **argv)
{
    const char *filename = argv[1];
    find_file(filename, ".");
    return 0;
}

