#define _LARGEFILE64_SOURCE /* must locate before all #include */
#include <sys/types.h> /* \                        */ 
#include <unistd.h>    /* - require to use lseek64 */
#include <fcntl.h> /* required for open() and other control operations */
#include <stdio.h>

int main(int argc, char** argv)
{
    off64_t file_size;
    char *file_name = argv[1];
    int fd = open(file_name, O_RDONLY);
    if (fd == -1) {
        printf("Error while open file\n");
        return 1;
    }
    file_size = lseek64(fd, 0, SEEK_END);
    close(fd);
    printf("file size is %lld bytes\n", (long long int)file_size); 
    return 0;
}


