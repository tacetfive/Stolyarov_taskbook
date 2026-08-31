#include <fcntl.h> /* open() */
#include <unistd.h> /* lseek */
#include <stdio.h>
#include <stdlib.h> /* strtol */

#define BUFFER_SIZE 32

int main(int argc, char **argv)
{
    int i, fd, init_pos, segment_length;
    char byte_to_write;
    int write_size;
    char buf[BUFFER_SIZE];
    char *filename;
    if ( argc != 5 ) {
        fprintf(stderr, "Incorrect parameters\n");
        return 1;
    }
    filename = argv[1];
    fd = open(filename, O_WRONLY);
    if ( fd == -1 ) {
        fprintf(stderr, "Error while open file \"%s\": ", filename);
        perror("");
        return 1;
    }
    init_pos = strtol(argv[2], NULL, 10);
    segment_length = strtol(argv[3], NULL, 10);
    byte_to_write = argv[4][0];
    lseek(fd, init_pos, SEEK_SET);

    write_size = BUFFER_SIZE < segment_length ? BUFFER_SIZE : segment_length; 
    for(i = 0; i < write_size; ++i)
        buf[i] = byte_to_write;
    
    while( 1 ) {
        if ( write(fd, buf, write_size) == -1 ) {
            perror("write error");
            return 1;
        }
        if ( BUFFER_SIZE > segment_length ) break;
        segment_length -= write_size;
        write_size = write_size < segment_length ? write_size : segment_length; 
    }
    return 0;
}
