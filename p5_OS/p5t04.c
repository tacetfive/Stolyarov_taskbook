#include <fcntl.h> /* open() */
#include <unistd.h> /* read() */
#include <stdio.h>

int main(int argc, char **argv)
{
    int i;
    int line_counter = 0;
    char buf[1024];
    int  red_bytes;
    int fd = open(argv[1], O_RDONLY);
    if ( fd == -1 ) {
        perror("open file error");
        return 1;
    }
    while( (red_bytes = read(fd, buf, 1024)) ) {
        for(i = 0; i<red_bytes; i++)
            if ( buf[i] == '\n' ) line_counter++;
    }
    printf("%d\n", line_counter);
    return 0;
}
