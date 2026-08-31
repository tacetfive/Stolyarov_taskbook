#include <fcntl.h> /* open() */
#include <unistd.h> /* read() */
#include <stdio.h>

int main(int argc, char **argv)
{
    int i;
    int line_counter = 0;
    char buf[1024];
    char *filename;
    int  red_bytes;
    int fd;
    if ( argc != 2 ) {
        fprintf(stderr, "Incorrect parameters\n");
        return 1;
    }
    filename = argv[1];
    fd = open(filename, O_RDONLY);
    if ( fd == -1 ) {
        fprintf(stderr, "Error while open file \"%s\": ", filename);
        perror("");
        return 1;
    }
    while( (red_bytes = read(fd, buf, 1024)) ) {
        for(i = 0; i<red_bytes; i++)
            if ( buf[i] == '\n' ) line_counter++;
    }
    close(fd);
    printf("%d\n", line_counter);
    return 0;
}
